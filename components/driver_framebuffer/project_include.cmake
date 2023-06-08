#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/moddisplay.c"
)

#Define the name of your module here
set(mod_name "udisplay")
set(driver_name "framebuffer")
set(mod_register "framebuffer DISPLAY")

if(CONFIG_DRIVER_FRAMEBUFFER_ENABLE)
    message(STATUS "framebuffer enabled")
    set(EXTMODS_INIT "${EXTMODS_INIT}" "\"${driver_name}\"@\"${mod_register}\"^" CACHE INTERNAL "")
    set(MICROPY_SOURCE_USERMOD "${MICROPY_SOURCE_USERMOD}" "${mods}" CACHE INTERNAL "")
else()
    message(STATUS "framebuffer disabled")
endif()