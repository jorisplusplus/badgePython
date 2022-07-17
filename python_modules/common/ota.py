import rtcmem
import system
import httpsota
from time import sleep
screen = False

HEAPLIMIT = 60000

try:    #Try to import screen, used to display ota progress
    import otascreen
    screen = True
except:
    pass

def start():
    """
    Start the ota progress
    """
    rtcmem.write(2, HEAPLIMIT)
    system.start("ota")

def run():
    """
    Internal function that gets called when started in heaplimit
    """
    rtcmem.write(2, 0)
    httpsota.start()
    status = httpsota.status()
    while status != 1.0:
        if screen:
            otascreen.update(status)
        sleep(0.2)
        status = httpsota.status()

if rtcmem.read(2) == HEAPLIMIT:
    run()