import i2c, ustruct

DEV_ADDR = 0x19


def init():
    i2c.writeto_mem(DEV_ADDR, 0x20, b'\x5F')

    # BDU enabled (update only after low and high byte have been read), 2G full-scale
    i2c.writeto_mem(DEV_ADDR, 0x23, b'\x80')


def get_xyz():
    # Multi-byte reads return the first byte N times. Temp workaround is reading one byte at a time.
    x, y, z = ustruct.unpack('bbb', b''.join([i2c.readfrom_mem(DEV_ADDR, reg, 1) for reg in [0x29, 0x2B, 0x2D]]))

    return x << 8, y << 8, z << 8
