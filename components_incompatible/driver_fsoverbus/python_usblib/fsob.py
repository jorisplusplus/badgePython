from operator import inv
import threading
import usb.core
import usb.util
import struct
from crc import Crc32, CrcCalculator
from queue import Queue
import time
import threading
import logging

fsob_logger = logging.Logger("FSOB")

class fsob_packet():
    packetId = 1
    ackId = 0
    def __init__(self, data=None, newMessage=False, endMessage=False, reset=False):
        self.TXHEADER = 0xAFFA
        self.RXHEADER = 0xFAAF
        self.FLAGNEWMESSAGE = 0x01
        self.FLAGENDMESSAGE = 0x02
        self.FLAGRESET = 0x04
        self.invalid = False

        if data != None and len(data) == 80:     #Init from packet
            (header, flags, crc, packetId, ackId, payload) = struct.unpack("<HHIII64s", data)
            if header != self.RXHEADER:
                fsob_logger.warning(f"Header incorrect: {header}")
                self.invalid = True
                return
            data = struct.pack("<HHIII64s", header, flags, 0, packetId, ackId, payload)
            crc_calculator = CrcCalculator(Crc32.CRC32)
            crcCalc = crc_calculator.calculate_checksum(data)
            if crc != crcCalc:
                fsob_logger.warning("CRC Check failed")
                self.invalid = True
                return
            self.flags = flags
            self.packetId = packetId
            self.ackId = ackId
            lenpayload = self.flags >> 9
            if lenpayload > 0:
                self.payload = payload[0:lenpayload]
            else:
                self.payload = None
        elif data == None or len(data) <= 64:   #Create packet for data
            self.payload = bytes()
            self.payloadBytes = None
            if data != None:
                self.payload = data
            self.packetId = -1
            self.flags = 0
            if newMessage:
                self.flags |= self.FLAGNEWMESSAGE
            if endMessage:
                self.flags |= self.FLAGENDMESSAGE
            if reset:
                self.flags |= self.FLAGRESET
            self.flags |= len(self.payload) << 9
            self.packettime = 0
    
    def txPacket(self, resend=False):
        if self.payloadBytes == None:
            if len(self.payload) == 0:  #No data, so nack packet
                self.packetId = 0
            else:
                self.packetId = fsob_packet.packetId
                fsob_packet.packetId += 1            
                
        if self.payloadBytes == None or resend:
            crc = 0
            payloadBytes = struct.pack("<HHIII64s", self.TXHEADER, self.flags, crc, self.packetId, fsob_packet.ackId, self.payload)
            crc_calculator = CrcCalculator(Crc32.CRC32)
            crc = crc_calculator.calculate_checksum(payloadBytes)
            self.payloadBytes = struct.pack("<HHIII64s", self.TXHEADER, self.flags, crc, self.packetId, fsob_packet.ackId, self.payload)
            self.packettime = time.time()
        return self.payloadBytes

class fsob_message():
    messageId = 1

    def __init__(self, command, payload, succesCB, errorCB):
        self.messageId = 0
        self.command = command
        if payload == None:
            self.payload = bytes()
        else:
            self.payload = payload
        self.response = None
        self.succesCB = succesCB
        self.errorCB = errorCB
        self.responseData = bytes()
        
    def generatePackets(self):
        """
        Generate fsob packets from fsob message
        """
        self.messageId = fsob_message.messageId
        fsob_message.messageId += 1
        messageLen = len(self.payload)
        if messageLen > 0:
            data = struct.pack(f"<HIHI{messageLen}s", self.command, messageLen, 57005, self.messageId, self.payload)
        else:
            data = struct.pack("<HIHI", self.command, messageLen, 57005, self.messageId)
        packets = list()
        for i in range(0, len(data), 64):
            packets.append(fsob_packet(data[i:i+64], i==0, i==((len(data)-1)//64)))
        return packets

    def receiveResponse(self, packet: fsob_packet):
        fsob_logger.debug(packet.flags)
        if packet.flags & packet.FLAGNEWMESSAGE and len(self.responseData) != 0:
            fsob_logger.error("Error start of message already received")
        self.responseData += packet.payload
        if packet.flags & packet.FLAGENDMESSAGE:
            if self.parseResponse():
                self.succesCB(self)
            else:
                self.errorCB(self)
            fsob_logger.debug("Called cb")
            return True
        return False
    
    def parseResponse(self):
        if len(self.responseData) < 12:
            return False
        (command, messageLen, verif, mId) = struct.unpack("<HIHI", self.responseData[0:12])
        if verif != 44510:
            fsob_logger.warning(f"Failed message verification, {verif}")
            return False
        if mId != self.messageId:
            fsob_logger.warning("Message id is not equal")
            return False
        if command != self.command:
            fsob_logger.warning("Command is not correct")
            return False
        if messageLen != (len(self.responseData)-12):
            fsob_logger.warning("Not all data recevied")
            return False
        (command, messageLen, verif, mId, payload) = struct.unpack(f"<HIHI{messageLen}s", self.responseData)
        self.response = payload
        return True


class fsob():
    def __init__(self, vendor=0xcafe, device=0x4021, epAddr=3):
        self.PACKETTIMEOUT = 0.10        
        self.dev = usb.core.find(idVendor=vendor, idProduct=device)
        if self.dev == None:
            fsob_logger.error("Failed to connect to device")
        else:
            cfg = self.dev.get_active_configuration()
            if cfg == None:
                self.dev.set_configuration()
        self.epOut = epAddr
        self.epIn = epAddr | 0x80
        self.messageQueue = Queue()
        self.activeMessage = None
        self.running = False
        self.packetsSend = list()       #List of packets that are in flight, have not been acked
        self.packetsToSend = list()     #List of packets that need to be send
        self.rxPacketID = 0             #Last received packet id
        self.lastAck = 0
        self.resetNext = True           #Send reset on startup
    
    def sendMessage(self, message: fsob_message):
        self.messageQueue.put(message)
        fsob_logger.debug("Message queued")

    def write(self, payload: bytes):
        self.dev.write(self.epOut, payload)

    def read(self):
        try:
            data = self.dev.read(self.epIn, 80, 100)
            if len(data) == 80:
                return data
            fsob_logger.warning(f"Packet incorrect length: {len(data)}")
            return None
        except:
            return None

    def txNackPacket(self, packet: fsob_packet):
        fsob_packet.ackId = self.rxPacketID
        self.lastAck = time.time()
        self.write(packet.txPacket())
        fsob_logger.info(f"Nack packet send {packet.packetId}, {packet.ackId}")

    def txPacket(self, packet: fsob_packet):
        fsob_packet.ackId = self.rxPacketID
        self.write(packet.txPacket())
        self.packetsSend.append(packet)
        self.lastAck = time.time()
        fsob_logger.info(f"Packet send {packet.packetId}")

    def handleAck(self, ackId: int):
        fsob_logger.info(f"Received ack for packet: {ackId}")
        while len(self.packetsSend) > 0 and self.packetsSend[0].packetId <= ackId:
            self.packetsSend.pop(0)

    def handlePacket(self, packet: fsob_packet):
        res = False
        if packet.packetId > (self.rxPacketID + 1) or packet.packetId < self.rxPacketID:
            fsob_logger.warning(f"Unexpected packet id: {packet.packetId}, last: {self.rxPacketID}")
            return False
        self.handleAck(packet.ackId)
        if packet.payload and packet.packetId != self.rxPacketID and self.activeMessage: #If payload is available handle message receive call
            fsob_logger.debug("Handle receive")
            res = self.activeMessage.receiveResponse(packet)
        self.rxPacketID = packet.packetId
        return res

    def reset(self):
        self.resetNext = True

    def run(self):
        self.running = True
        fsob_logger.info("Starting thread")
        while self.running:
            if self.resetNext:
                self.resetNext = False
                self.activeMessage = None
                fsob_message.messageId = 1
                fsob_packet.ackId = 0
                fsob_packet.packetId = 1
                self.txNackPacket(fsob_packet(None, reset=True))
            
            #Check if queue holds a message a no message is in transit
            if self.activeMessage == None and (not self.messageQueue.empty()):
                self.activeMessage = self.messageQueue.get()
                self.packetsToSend.extend(self.activeMessage.generatePackets())
                fsob_logger.info("Message loaded")
            
            #Check if space is available and packets ready to send
            while len(self.packetsSend) < 5 and len(self.packetsToSend) > 0:
                self.txPacket(self.packetsToSend.pop(0))

            #Ack check on send packets
            for packet in self.packetsSend:
                if (time.time() - packet.packettime) > self.PACKETTIMEOUT:
                    #pass
                    fsob_logger.info("Packet timeout")

            #Read packet
            data = self.read()
            if data != None:
                fsob_logger.info(f"Package received {len(data)}")
                rxPacket = fsob_packet(data)
                if not rxPacket.invalid:
                    res = self.handlePacket(rxPacket)
                    if res:
                        self.activeMessage = None
                else:
                    fsob_logger.info("Packet invalid")
            
            #Send blank ack
            if (self.rxPacketID - fsob_packet.ackId) > 2:
                fsob_logger.info("Acking packets")
                self.txNackPacket(fsob_packet(None))
            elif self.rxPacketID != fsob_packet.ackId and (time.time() - self.lastAck) > 0.2:
                fsob_logger.info("Acking packets on timeout")
                self.txNackPacket(fsob_packet(None))
    
    def start(self):
        self.thread = threading.Thread(target=self.run, daemon=True)
        self.thread.start()
    def stop(self):
        self.running = False