import machine

bus = machine.SoftI2C(scl=machine.Pin(9), sda=machine.Pin(8), freq=100000)

# Copies all attributes from machine.SoftI2C,
# so you can use e.g. i2c.readfrom_mem()

for name in dir(bus):
    globals()[name] = getattr(bus, name)