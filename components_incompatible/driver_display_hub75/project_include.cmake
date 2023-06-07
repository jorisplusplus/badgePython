#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/modhub75.c"
)

#Define the name of your module here
set(mod_name "hub75")
set(mod_register "hub75 DISPLAY")

if(CONFIG_DRIVER_HUB75_ENABLE)
    message(STATUS "hub75 enabled")
    set(EXTMODS "${EXTMODS}" "${mods}" CACHE INTERNAL "")
    set(EXTMODS_NAMES "${EXTMODS_NAMES}" "${mod_name}" CACHE INTERNAL "")
    set(EXTMODS_INIT "${EXTMODS_INIT}" "\"${mod_name}\"@\"${mod_register}\"^" CACHE INTERNAL "")
    
    add_library("${mod_name}" INTERFACE)

    target_sources("${mod_name}" INTERFACE
        "${mods}"
    )

    target_include_directories("${mod_name}" INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
    )

    target_link_libraries("${mod_name}" INTERFACE "${mod_name}")
else()
    message(STATUS "hub75 disabled")
endif()