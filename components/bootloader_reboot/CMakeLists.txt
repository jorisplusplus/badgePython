set(srcs
    "bootloader.c"
    "modbootloader.c"
)

set(includes
    "include"
    )

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS ${includes}
                       REQUIRES main esp_rom soc esp_tinyusb)
