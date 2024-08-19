#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/modfluidsim.c"
)

#Define the name of your module here
set(mod_name "fluidsim")
set(mod_register "fluidsim")

message(STATUS "fluidsim enabled")
set(EXTMODS_INIT "${EXTMODS_INIT}" "\"${mod_name}\"@\"${mod_register}\"^" CACHE INTERNAL "")
set(MICROPY_SOURCE_USERMOD "${MICROPY_SOURCE_USERMOD}" "${mods}" CACHE INTERNAL "")


