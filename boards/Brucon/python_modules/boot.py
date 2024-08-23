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
	app = "shell"
elif nvs.get_int("system", "splash_played") != 1:
	nvs.set_int("system", "splash_played", 1)
	# Boot splash screen
	app = "nyan"
else:
	# Default app
	app = nvs.get_str("system", "boot_app")
	if not app:
		app = nvs.get_str("system", "default_app")
		if not app:
			app = "launcher"

if app and app != "shell":
	try:
		print("Starting app '%s'..." % app)
		system.__current_app__ = app
		if app:
			__import__(app)
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
	print("\nWelcome to the python shell of your badge!")
