#Include all files that contain MP bindings
set(mods
    "${COMPONENT_DIR}/modheapspace.c"
)

#Define the name of your module here
set(mod_name "heapspace")
set(mod_register "heapspace")

if(CONFIG_DRIVER_HEAPSPACE_ENABLE)
    message(STATUS "heapspace enabled")
    set(EXTMODS "${EXTMODS}" "${mods}" CACHE INTERNAL "")
    set(EXTMODS_NAMES "${EXTMODS_NAMES}" "${mod_name}" CACHE INTERNAL "")

    add_library("${mod_name}" INTERFACE)

    target_sources("${mod_name}" INTERFACE
        "${mods}"
    )

    target_include_directories("${mod_name}" INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}
    )

    target_link_libraries("${mod_name}" INTERFACE "${mod_name}")
endif()
