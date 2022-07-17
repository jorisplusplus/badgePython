import machine
from machine import UART, Timer

keyboard_handler = []
statusled_handler = []

RELEASED = 0
PRESSED = 1

keyboard_disabled = 0

def uart_callback(datastruct):
    global keyboard_handler, statusled_handler
    global keyboard_disabled
    message = datastruct
    if len(message) == 3:
        if int(message[0]) != 0xA7:
            platform_uart.read()
            return

        
        message = bytes(message, "ascii")
        command = int(message[1])
        payload = int(message[2])
        #print(f"command: {command}, payload: {payload}")
        if command == 0x02:
            keyboard_disabled = (payload & 0x04) > 0       
        elif command == 0x00: #release key
            for handler in keyboard_handler:
                try:
                    handler(RELEASED, payload)
                except Exception as e:
                    import sys
                    print("Exception in handler")
                    sys.print_exception(e)
        elif command == 0x01: #press key
            for handler in keyboard_handler:
                try:
                    handler(PRESSED, payload)
                except Exception as e:
                    import sys
                    print("Exception in handler")
                    sys.print_exception(e)
        elif command == 0x06: #led update
            for handler in statusled_handler:
                try:
                    handler(payload)
                except Exception as e:
                    import sys
                    print("Exception in handler")
                    sys.print_exception(e)
      
def receiver(tim):
    if platform_uart.any() >= 3:
        res = platform_uart.read(3)
        uart_callback(res)

def usb_hid(enable=True):
    global platform_uart
    if enable:
        platform_uart.write(bytes([0x7A, 0x02, 0x00]))
    else:
        platform_uart.write(bytes([0x7A, 0x03, 0x00]))

def usb_disabled():
    global keyboard_disabled
    return keyboard_disabled

def add_statusled_handler(handler):
    global statusled_handler
    statusled_handler.append(handler)

def remove_statusled_handler(handler):
    global statusled_handler
    for index, _handler in statusled_handler:
        if _handler == handler:
            del statusled_handler[index]
            break

def add_keyboard_handler(handler):
    global keyboard_handler
    keyboard_handler.append(handler)

def remove_keyboard_handler(handler):
    global keyboard_handler
    for index, _handler in keyboard_handler:
        if _handler == handler:
            del keyboard_handler[index]
            break

def force_update():
    platform_uart.write(bytes([0x04, 0x00]))

platform_uart = machine.UART(2)
platform_uart.init(115200, tx=23, rx=22)
platform_uart.write(bytes([0, 0, 0, 0])) #Purge
platform_tim = Timer(3)
platform_tim.init(mode=Timer.PERIODIC, callback=receiver, freq=100, tick_hz=100)

force_update()