import uos, gc, sys
from flashbdev import bdev

try:
    if bdev:
        uos.mount(bdev, "/")
except OSError:
    import inisetup

    vfs = inisetup.setup()

gc.collect()

folders = ['lib', 'apps', 'cache', 'cache/woezel', 'config']
print("Running _boot.py")
for folder in folders:
    try:
        uos.mkdir(folder)
    except Exception as error:
        pass

sys.path.append('/apps')
sys.path.append('/lib')

del folders, uos
gc.collect(); gc.mem_free()
