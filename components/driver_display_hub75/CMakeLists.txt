if(CONFIG_DRIVER_HUB75_ENABLE)
    set(srcs
        "compositor.c"
        "font_6x3.c"
        "font_7x5.c"
        "val2pwm.c"
        "modhub75.c"
    )
    if(CONFIG_HUB75_BRUCON)
        list(APPEND srcs "i2s_parallel_brucon.c")
        list(APPEND srcs "driver_hub75_brucon.c")
    else()
        list(APPEND srcs "i2s_parallel.c")
        list(APPEND srcs "driver_hub75.c")
    endif()
else()
   set(srcs "")
endif()

set(includes
    "include"
    )

idf_component_register(SRCS "${srcs}"
        INCLUDE_DIRS ${includes}
        REQUIRES main)
