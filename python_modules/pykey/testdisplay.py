from time import sleep
import display

for i in range(0, 128):
    display.drawFill(0)
    for j in range(0, i):
        display.drawPixel(j, 0, 0xFFFFFF)
    display.flush()
    sleep(0.1)