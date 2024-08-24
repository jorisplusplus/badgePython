set(srcs
    "modusb.c"
    "usb_cdc_helpers.c"
)

set(includes
    "include"
)

idf_component_register(SRCS "${srcs}"
                       INCLUDE_DIRS ${includes}
                       PRIV_REQUIRES main esp_tinyusb)
