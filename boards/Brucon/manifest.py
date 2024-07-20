freeze("../../python_modules/", ("flashbdev.py", "inisetup.py", "system.py", "version.py", "term.py", "term_menu.py",
                                 "display.py", "wifi.py", "valuestore.py", "appconfig.py", "nvs.py", "virtualtimers.py"))
freeze("./python_modules/")
# Useful networking-related packages.
require("bundle-networking")

# Require some micropython-lib modules.
# require("aioespnow")
require("dht")
require("ds18x20")
require("neopixel")
require("onewire")
require("upysh")
