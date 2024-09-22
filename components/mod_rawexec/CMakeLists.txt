set(srcs
    "modrawexec.c"
)

set(includes
    ""
)

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS ${includes}
                       PRIV_REQUIRES main esp_tinyusb)
