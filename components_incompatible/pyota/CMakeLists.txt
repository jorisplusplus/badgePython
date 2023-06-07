idf_component_register(
  SRCS "modpyota.c"
  REQUIRES mbedtls esp_https_ota app_update micropython
)
