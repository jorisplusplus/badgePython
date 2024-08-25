import rgb, usb, time

rgb.scrolltext("BruCON 0x10 badge by ankhaneko.art & curious.supplies")

while not usb.cdc_connected():
      time.sleep(0.5)

print("\r\n\r\nThe BruCON 0x10 badge was made for you with love by:\r\n")
print("Joris Witteman / curious.supplies - hardware, software")
print("Tom Clement / curious.supplies - software")
print("Nikolett S. / ankhaneko.art - PCB art")
print("Norbert / Allnet China - sourcing & production\r\n\r\n")