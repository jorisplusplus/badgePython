set(srcs "motor.c"
        "motorindent.c"
        "modmotor.c")


idf_component_register(SRCS ${srcs}
                       INCLUDE_DIRS "include"
                       REQUIRES esp_timer driver main)

target_compile_options(${COMPONENT_LIB} PRIVATE -Wdouble-promotion)