import system, time, rtcmem, esp32, nvs

def bootloader_enter(dfu_mode=False):
	import bootloader
	bootloader.enter(dfu_mode)

app = ""

# Special boot mode apps for first time boot
if nvs.get_int("system", "factory_checked") != 2:
	# Factory check mode
	# Direct import because of GPIO0 strapped value after initial flash, we can't deepsleep without
	# going back into bootloader mode
	import factory_checks
	while True:
		# Force factory flasher to see that we're alive
		print(">>> ")
		time.sleep(0.5)
elif nvs.get_int("system", "first_powerup") != 1:
	nvs.set_int("system", "first_powerup", 1)
	app = "powerup"
elif nvs.get_int("system", "splash_played") != 1:
	nvs.set_int("system", "splash_played", 1)
	app = "bootsplash"
else:
	# Default app
	app = nvs.get_str("system", "boot_app")
	if not app:
		app = nvs.get_str("system", "default_app")
		if not app:
			app = "launcher"

if app and app != "shell":
	try:
		# Set app back to launcher in case of a crash
		print("Starting app '%s'..." % app)
		system.__current_app__ = app
		if app:
			module = __import__(app)
			for name in dir(module):
				globals()[name] = getattr(module, name)
			del module
	except KeyboardInterrupt:
		system.shell()
	except BaseException as e:
		sys.print_exception(e)
		if not nvs.get_int("system", "ignore_crash"):
			print("Fatal exception in the running app!")
			system.crashedWarning()
			time.sleep(3)
			system.launcher()

if app and app == "shell":
	import rgb, usb, gc

	wait_times = 0
	while not usb.cdc_connected():
		if wait_times == 2:
			rgb.scrolltext("USB Serial python shell active")
		time.sleep(0.5)
		wait_times += 1

	rgb.clear()
	del rgb, usb, wait_times
	gc.collect()
	del gc

	print("\nWelcome to the python shell of your badge!")
