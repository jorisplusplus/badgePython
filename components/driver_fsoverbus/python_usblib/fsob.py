from operator import inv
import threading
import usb.core
import usb.util
import struct
from crc import Crc32, CrcCalculator
from queue import Queue
import time
import threading

class fsob_packet():
    packetId = 1
    ackId = 0
    def __init__(self, data=None, newMessage=False, endMessage=False):
        self.TXHEADER = 0xAFFA
        self.RXHEADER = 0xFAAF
        self.FLAGNEWMESSAGE = 0x01
        self.FLAGENDMESSAGE = 0x02
        self.invalid = False

        if data != None and len(data) == 80:     #Init from packet
            (header, flags, crc, packetId, ackId, payload) = struct.unpack("<HHIII64s", data)
            if header != self.RXHEADER:
                print(f"Header incorrect: {header}")
                self.invalid = True
                return
            data = struct.pack("<HHIII64s", header, flags, 0, packetId, ackId, payload)
            crc_calculator = CrcCalculator(Crc32.CRC32)
            crcCalc = crc_calculator.calculate_checksum(data)
            if crc != crcCalc:
                print("CRC Check failed")
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
            self.flags |= len(self.payload) << 9
            print(len(self.payload))
            self.packettime = 0
    
    def txPacket(self, resend=False):
        if self.payloadBytes == None:
            self.packetId = fsob_packet.packetId
            if len(self.payload) > 0:
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
        print(packet.flags)
        if packet.flags & packet.FLAGNEWMESSAGE and len(self.responseData) != 0:
            print("Error start of message already received")
        self.responseData += packet.payload
        if packet.flags & packet.FLAGENDMESSAGE:
            if self.parseResponse():
                self.succesCB(self)
            else:
                self.errorCB(self)
            print("Called cb")
            return True
        return False
    
    def parseResponse(self):
        if len(self.responseData) < 12:
            return False
        (command, messageLen, verif, mId) = struct.unpack("<HIHI", self.responseData[0:12])
        if verif != 44510:
            print(f"Failed message verification, {verif}")
            return False
        if mId != self.messageId:
            print("Message id is not equal")
            return False
        if command != self.command:
            print("Command is not correct")
            return False
        if messageLen != (len(self.responseData)-12):
            print("Not all data recevied")
            return False
        (command, messageLen, verif, mId, payload) = struct.unpack(f"<HIHI{messageLen}s", self.responseData)
        self.response = payload
        return True


class fsob():
    def __init__(self, vendor=0xcafe, device=0x4021, epAddr=3):
        self.PACKETTIMEOUT = 0.10        
        self.dev = usb.core.find(idVendor=vendor, idProduct=device)
        if self.dev == None:
            print("Failed to connect to device")
        else:
            cfg = self.dev.get_active_configuration()
            #print(cfg)
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
    
    def sendMessage(self, message: fsob_message):
        self.messageQueue.put(message)
        print("Message queued")

    def write(self, payload: bytes):
        self.dev.write(self.epOut, payload)

    def read(self):
        try:
            return self.dev.read(self.epIn, 80, 5)
        except:
            return None

    def txNackPacket(self, packet: fsob_packet):
        fsob_packet.ackId = self.rxPacketID
        self.write(packet.txPacket())

    def txPacket(self, packet: fsob_packet):
        fsob_packet.ackId = self.rxPacketID
        self.write(packet.txPacket())
        self.packetsSend.append(packet)
        print(f"Packet send {packet.packetId}")

    def handleAck(self, ackId: int):
        print(f"Acking {ackId}")
        while len(self.packetsSend) > 0 and self.packetsSend[0].packetId <= ackId:
            self.packetsSend.pop()

    def handlePacket(self, packet: fsob_packet):
        res = False
        if packet.packetId > (self.rxPacketID + 1) or packet.packetId < self.rxPacketID:
            print("Unexpected packet")
            return False
        self.handleAck(packet.ackId)
        if packet.payload and packet.packetId != self.rxPacketID: #If payload is available handle message receive call
            print("Handle receive")
            res = self.activeMessage.receiveResponse(packet)
        self.rxPacketID = packet.packetId
        return res

    def run(self):
        self.running = True
        print("Starting thread")
        while self.running:
            #Check if queue holds a message a no message is in transit
            #print(f"{self.activeMessage} {not self.messageQueue.empty()}")
            if self.activeMessage == None and (not self.messageQueue.empty()):
                self.activeMessage = self.messageQueue.get()
                self.packetsToSend.extend(self.activeMessage.generatePackets())
                print("Message loaded")
            
            #Check if space is available and packets ready to send
            while len(self.packetsSend) < 5 and len(self.packetsToSend) > 0:
                self.txPacket(self.packetsToSend.pop())

            #Ack check on send packets
            for packet in self.packetsSend:
                if (time.time() - packet.packettime) > self.PACKETTIMEOUT:
                    #pass
                    print("Packet timeout")

            #Read packet
            data = self.read()
            if data != None:
                print(f"Package received {len(data)}")
                rxPacket = fsob_packet(data)
                if not rxPacket.invalid:
                    res = self.handlePacket(rxPacket)
                    if res:
                        self.activeMessage = None
                else:
                    print("Packet invalid")
            
            #Send blank ack
            if (self.rxPacketID - fsob_packet.ackId) > 2:
                self.txNackPacket(fsob_packet(None))
    
    def start(self):
        self.thread = threading.Thread(target=self.run, daemon=True)
        self.thread.start()
    def stop(self):
        self.running = False