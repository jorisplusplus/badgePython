import upysh, flags, uinterface, rgb, usb, time


def prep():
    # enc = bytes([c^(b'?r3\x84\xcd\x93\xaa\xe5\xa7\x80\x8d<\x82\xdct\x94!\x10\xb8\x981Z\xa3\x11ng\xa8\x87\xf0\xd9B\xb8'[i%32]) for i, c in enumerate(b'BRUCON{734bb84563502de4daf59746b8d57324}')])
    flag = ''.join([chr(c^(b'?r3\x84\xcd\x93\xaa\xe5\xa7\x80\x8d<\x82\xdct\x94!\x10\xb8\x981Z\xa3\x11ng\xa8\x87\xf0\xd9B\xb8'[i%32])) for i, c in enumerate(b'} f\xc7\x82\xdd\xd1\xd2\x94\xb4\xef^\xba\xe8A\xa2\x12%\x88\xaaU?\x97u\x0f\x01\x9d\xbe\xc7\xedt\xda\x07\x16\x06\xb3\xfe\xa1\x9e\x98')])

    with open('/private/system/flag.txt', 'wt') as file:
        file.write(flag)

prep()
del prep
x=dir(flags)  ## ensure import doesn't get optimised out

for name in dir(upysh):
    globals()[name] = getattr(upysh, name)

_message = 'You\'re now in a Python shell with "upysh" commandline-like functions.\n' + \
      'Type "man" to see what you can do, and hunt for a file called flag.txt.\n\n' + \
      'You can submit the flag by calling flags.submit_flag("BRUCON{xxxx}").'


time.sleep(1)
if not usb.cdc_connected():
    uinterface.loading_text("Connect via USB, serial 115200 baud")
    while not usb.cdc_connected():
        time.sleep(0.5)

rgb.clear()
rgb.scrolltext("BruCON 0x10 CTF chall 1")
print(_message)