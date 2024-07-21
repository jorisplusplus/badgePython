#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/modhub75.c"
)

#Define the name of your module here
set(mod_name "driver_hub75")
set(mod_register "hub75")

if(CONFIG_DRIVER_HUB75_ENABLE)
    message(STATUS "hub75 enabled")
    set(EXTMODS "${EXTMODS}" "${mods}" CACHE INTERNAL "")
    set(EXTMODS_NAMES "${EXTMODS_NAMES}" "${mod_name}" CACHE INTERNAL "")
    set(EXTMODS_INIT "${EXTMODS_INIT}" "\"${mod_name}\"@\"${mod_register}\"^" CACHE INTERNAL "")
    set(MICROPY_SOURCE_USERMOD "${MICROPY_SOURCE_USERMOD}" "${mods}" CACHE INTERNAL "")


else()
   message(STATUS "hub75 disabled")
endif()