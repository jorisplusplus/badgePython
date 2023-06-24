import udisplay
import framebuf

class display():
    def __init__(self):
        self.buf = bytearray(udisplay.WIDTH*udisplay.HEIGHT*2)
        self.frame = framebuf.FrameBuffer(self.buf, udisplay.WIDTH, udisplay.HEIGHT, framebuf.RGB565)
    
    def flush(self):
        udisplay.flush(self.buf)