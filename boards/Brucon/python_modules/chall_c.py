import flags, rawexec, uinterface, rgb, usb, time


def pass_flag(instructions):
    """
    *** SOLUTION ***

    A possible solution to this challenge can be to create a c file called solution.c
    with the following contents:

        typedef int (*printf_ptr) (const char *, ...);

        void func(const char *str, printf_ptr call_printf) {
            call_printf(str);
        }

    And then compile this file using the Xtensa ESP32 Toolchain:

    $ xtensa-esp32-elf-gcc -c solution.c

    Then dissamble the created object file by running:

    $ xtensa-esp32-elf-objdump -d solution.o

    Which will give you the required Xtensa Tensilica machine code:

       0:	006136      entry	a1, 48
       3:	017d      	mov.n	a7, a1
       5:	0729      	s32i.n	a2, a7, 0
       7:	1739      	s32i.n	a3, a7, 4
       9:	1728      	l32i.n	a2, a7, 4
       b:	07a8      	l32i.n	a10, a7, 0
       d:	0002e0      callx8	a2
      10:	f03d      	nop.n
      12:	f01d      	retw.n

    Which needs to be properly put in a python bytestring using the right alignment and then
    used in the call to `pass_flag()`:

    >>> pass_flag(b"\x36\x61\x00\x7D\x01\x29\x07\x39\x17\x28\x17\xa8\x07\xe0\x02\x00\x3d\xf0\x1d\xf0")
    assembly_len: 20, assembly location: 0x4008c8fc, first dword: 7D006136
    0x4008c8fc: esp_flash_user_start at modesp.c:?

    CTF{0a977ec472ed1d54a0408fd9f46dd76cb85196c4d3175384}
    """
    # enc = bytes([c^(b'\x08\xe6\xe8\x92\xe2/\xb6\x82\x9e)\xa7\xbd\x8d\xe8c\x11%\x18"\xcc/\x86\xd7\x97f\xbaH:\x8f(\x82\x17'[i%32]) for i, c in enumerate(b'BRUCON{32c05da00f411469d907bfec518ff7fd}')])
    flag = ''.join([chr(c^(b'\x08\xe6\xe8\x92\xe2/\xb6\x82\x9e)\xa7\xbd\x8d\xe8c\x11%\x18"\xcc/\x86\xd7\x97f\xbaH:\x8f(\x82\x17'[i%32])) for i, c in enumerate(b'J\xb4\xbd\xd1\xada\xcd\xb1\xacJ\x97\x88\xe9\x89S!C,\x13\xfd\x1b\xb0\xee\xf3_\x8a\x7fX\xe9M\xe1"9\xde\x8e\xf4\xd5I\xd2\xff\x94')])+"\n"
    rawexec.call(flag, instructions)


_message = "Get the flag by calling pass_flag(<instructions>).\n\n" + \
            "Instructions should be sent as a bytestring of Xtensa Tensilica machine code " + \
            "that gets executed as a C function: instructions(flag, printf)\n\n" + \
            "You can paste snippets using CTRL+E and CTRL+D.\n\n" + \
            "You can submit the flag by calling flags.submit_flag('BRUCON{xxxx}')."

x=dir(flags)  ## ensure import doesn't get optimised out

time.sleep(1)
if not usb.cdc_connected():
    uinterface.loading_text("Connect via USB, serial 115200 baud")
    while not usb.cdc_connected():
        time.sleep(0.5)

rgb.clear()
rgb.scrolltext("BruCON 0x10 CTF chall 3")
print(_message)
