#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/modusb.c"
)

#Define the name of your module here
set(mod_name "usb")
set(mod_register "usb")

set(MICROPY_SOURCE_USERMOD "${MICROPY_SOURCE_USERMOD}" "${mods}" CACHE INTERNAL "")
