#ifndef RTCMEM_H
#define RTCMEM_H

#include <stdbool.h>
#include <stdint.h>
#include <esp_err.h>

__BEGIN_DECLS

#define POS_HEAPLIMITER (2)

extern esp_err_t rtcmem_int_write(int pos, int val);
extern esp_err_t rtcmem_int_read(int pos, int* val);

extern esp_err_t rtcmem_string_write(const char* str);
extern esp_err_t rtcmem_string_read(const char** str);

extern esp_err_t rtcmem_clear();

extern esp_err_t rtcmem_init(void);

__END_DECLS

#endif // RTCMEM_H
