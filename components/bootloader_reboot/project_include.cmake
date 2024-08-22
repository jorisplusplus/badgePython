#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/modbootloader.c"
)

#Define the name of your module here
set(mod_name "bootloader")
set(mod_register "bootloader")

message(STATUS "bootloader enabled")
set(EXTMODS_INIT "${EXTMODS_INIT}" "\"${mod_name}\"@\"${mod_register}\"^" CACHE INTERNAL "")
set(MICROPY_SOURCE_USERMOD "${MICROPY_SOURCE_USERMOD}" "${mods}" CACHE INTERNAL "")


