import accel, fluidsim

accel.init()
while True:
    fluidsim.update(*[n/20000*9.81 for n in accel.get_xyz()])