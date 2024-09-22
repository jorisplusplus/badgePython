#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/modrawexec.c"
)

#Define the name of your module here
set(mod_name "rawexec")
set(mod_register "rawexec")

set(MICROPY_SOURCE_USERMOD "${MICROPY_SOURCE_USERMOD}" "${mods}" CACHE INTERNAL "")
