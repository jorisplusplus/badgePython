import machine, rgb, time
from uinterface import CONFIRMATION_YES_IMAGE

width, height = CONFIRMATION_YES_IMAGE['width'], CONFIRMATION_YES_IMAGE['height']
rgb.gif(CONFIRMATION_YES_IMAGE['data'], pos=((rgb.screenwidth-width)//2, (rgb.screenheight-height)//2), size=(4, 4))

time.sleep(5)
machine.deepsleep(0)