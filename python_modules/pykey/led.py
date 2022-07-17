import neopixel
import led_keys
import machine

NUMKEYS = 99

np = neopixel.NeoPixel(machine.Pin(13), NUMKEYS)

def setKey(key, color, flush=True):
    np[key] = (color[0], color[1], color[2])
    if flush:
        np.write()

def keyFlush():
    np.write()

def allKeys(color):
    for i in range(0, NUMKEYS):
        np[i] = color
    np.write()