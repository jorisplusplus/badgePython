set(srcs
    "fluidsim.c"
    "modfluidsim.c"
)

set(includes
    "include"
    )

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS ${includes}
                       REQUIRES driver_display_hub75)
