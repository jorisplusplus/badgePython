set(srcs
    "driver_display.c"
)

set(includes
    "include"
    )

set(IDF_COMPONENTS
    buses
    esp_lcd
)

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS ${includes}
                       REQUIRES ${IDF_COMPONENTS} )
