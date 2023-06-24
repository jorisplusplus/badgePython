#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/moddisplay.c"
)

#Define the name of your module here
set(mod_name "display")
set(mod_register "display")

if(CONFIG_DISPLAY_ENABLE)
    message(STATUS "display enabled")
    set(MICROPY_SOURCE_USERMOD "${MICROPY_SOURCE_USERMOD}" "${mods}" CACHE INTERNAL "")
    set(EXTMODS_INIT "${EXTMODS_INIT}" "\"${mod_name}\"@\"${mod_register}\"^" CACHE INTERNAL "")
endif()
