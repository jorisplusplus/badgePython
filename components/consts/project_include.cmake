#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/modconsts.c"
)

#Define the name of your module here
set(mod_name "modconsts")
set(mod_register "consts")

set(EXTMODS "${EXTMODS}" "${mods}" CACHE INTERNAL "")
set(EXTMODS_NAMES "${EXTMODS_NAMES}" "${mod_name}" CACHE INTERNAL "")
set(EXTMODS_INIT "${EXTMODS_INIT}" "\"${mod_name}\"@\"${mod_register}\"^" CACHE INTERNAL "")
set(MICROPY_SOURCE_USERMOD "${MICROPY_SOURCE_USERMOD}" "${mods}" CACHE INTERNAL "")
