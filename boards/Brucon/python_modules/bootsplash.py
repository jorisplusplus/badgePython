import rgb, time, system, nvs
from default_icons import icon_beer

for _ in range(2):
    for x in range(31):
        rgb.disablecomp()
        rgb.clear()
        rgb.pixel(pos=(x, 0))
        rgb.pixel(pos=(x + 1, 0))
        rgb.pixel(pos=(30 - x, 18))
        rgb.pixel(pos=(31 - x, 18))
        rgb.enablecomp()
        time.sleep(0.05)

    rgb.disablecomp()
    rgb.clear()
    rgb.pixel(pos=(31, 0))
    rgb.pixel(pos=(0, 19))
    rgb.enablecomp()
    time.sleep(0.05)

    for y in range(18):
        rgb.disablecomp()
        rgb.clear()
        rgb.pixel(pos=(31, y))
        rgb.pixel(pos=(31, y + 1))
        rgb.pixel(pos=(0, 17 - y))
        rgb.pixel(pos=(0, 18 - y))
        rgb.enablecomp()
        time.sleep(0.05)

    rgb.disablecomp()
    rgb.clear()
    rgb.pixel(pos=(31, 18))
    rgb.pixel(pos=(0, 0))
    rgb.enablecomp()
    time.sleep(0.05)

rgb.clear()
time.sleep(0.5)
rgb.setfont(1)
rgb.brightness(0)
rgb.text("brucon", pos=(4,2), color=(180, 56, 54))
rgb.text("0X10", pos=(9,11), color=(180, 56, 54))

for brightness in range(4, 31):
    time.sleep(0.05)
    rgb.brightness(brightness)

time.sleep(5)

for brightness in range(30, 0, -1):
    time.sleep(0.05)
    rgb.brightness(brightness)

time.sleep(1)
rgb.clear()
rgb.brightness(30)

for i in range(5):
    rgb.clear()
    rgb.gif(icon_beer[0], ((rgb.screenwidth-8)//2, (rgb.screenheight-8)//2), (8,8), icon_beer[1])
    time.sleep(0.5)
    rgb.clear()
    rgb.gif(icon_beer[0], ((rgb.screenwidth-8)//2, (rgb.screenheight-12)//2), (8,8), icon_beer[1])
    time.sleep(0.5)

time.sleep(1)

rgb.clear()
rgb.setfont(0)
rgb.scrolltext("BruCON 0x10 badge by ankhaneko.art & curious.supplies")

time.sleep(16)
nvs.set_int("system", "splash_played", 1)
system.launcher()


