import machine, sys, system, time, rtcmem, esp32, nvs

rtcmem.write(0,0)
rtcmem.write(1,0)

# Default app
app = rtcmem.read_string()
if not app:
	#app = nvs.nvs_getstr("system", 'default_app')
	#if not app:
    app = "launcher"

# # Override with special boot mode apps if necessary
if nvs.get_int("system", "factory_checked") != 2:
	# Factory check mode
	app = "factory_checks"
elif nvs.get_int("system", "splash_played") != 1:
	nvs.set_int("system", "splash_played", 1)
	# Boot splash screen
	app = "bootsplash"

if app and not app == "shell":
	try:
		print("Starting app '%s'..." % app)
		system.__current_app__ = app
		if app:
			__import__(app)
	except KeyboardInterrupt:
		system.launcher()
	except BaseException as e:
		sys.print_exception(e)
		if not nvs.get_int("system", "ignore_crash"):
			print("Fatal exception in the running app!")
			system.crashedWarning()
			time.sleep(3)
			system.launcher()

if app and app == "shell":
	print("\nWelcome to the python shell of your badge!\nCheck out https://pixel.curious.supplies/docs/ for instructions.")