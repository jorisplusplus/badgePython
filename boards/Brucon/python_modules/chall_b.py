import virtualtimers, flags, uinterface, rgb, usb, time


def prep():
    virtualtimers.begin(500)
    with open('/proc/uid', 'wt') as file:
        file.write('1000')


prep()
del prep
x=dir(flags)  ## ensure import doesn't get optimised out

def print_flag():
    def actual_print():
        # enc = bytes([c^(b'\x9f\x8c*\xb0l\x83\xf0<\x9c\xbak\xe6\x0e!\xc9\xd1#\xf6\xe6\x8c\x00N\xa7\xd0" \xf6\x1b\xef\x11I\r'[i%32]) for i, c in enumerate(b'BRUCON{0742dd60841fb53b0b2095047615eeea}')])
        flag = ''.join([chr(c^(b'\x9f\x8c*\xb0l\x83\xf0<\x9c\xbak\xe6\x0e!\xc9\xd1#\xf6\xe6\x8c\x00N\xa7\xd0" \xf6\x1b\xef\x11I\r'[i%32])) for i, c in enumerate(b'\xdd\xde\x7f\xf3#\xcd\x8b\x0c\xab\x8eY\x82j\x17\xf9\xe9\x17\xc7\x80\xee5}\xc5\xe0@\x12\xc6"\xda!}:\xa9\xbd\x1f\xd5\t\xe6\x91A')])

        try:
            with open('/proc/uid', 'rt') as file:
                uid = int(file.read())
        except:
            print('Debug: /proc/uid is corrupt: does not contain textual integer user ID.')
            return -1
        if uid != 0:
            print('Only the root user is allowed to print the flag.')
            return -1
        else:
            print(f'Congratulations, here\'s your flag: {flag}')
        return -1

    print('Debug: Verifying integrity of /proc/uid..')
    time.sleep(1)
    try:
        with open('/proc/uid', 'rt') as file:
            uid = int(file.read())
    except:
        print('Debug: /proc/uid is corrupt: does not contain textual integer user ID.')
        return

    if uid != 1000:
        print('User ID has been tampered with! Exiting.')
        return
    else:
        print('Debug: User ID integrity checks out. Yielding the processor to be sociable to other processes before printing flag..')
        virtualtimers.new(500, actual_print)


_message = 'Use print_flag() to obtain the flag. You can paste snippets using CTRL+E and CTRL+D.\n\n' + \
           'You can submit the flag by calling flags.submit_flag("BRUCON{xxxx}").'

time.sleep(1)
if not usb.cdc_connected():
    uinterface.loading_text("Connect via USB, serial 115200 baud")
    while not usb.cdc_connected():
        time.sleep(0.5)

rgb.clear()
rgb.scrolltext("BruCON 0x10 CTF chall 2")
print(_message)