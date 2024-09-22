#include <stdio.h>
#include <stdint.h>
#include <string.h>

//#include "tinyusb.h"

#include "py/mperrno.h"
#include "py/mphal.h"
#include "py/runtime.h"

typedef int (*printf_ptr)(const char *, ...);

typedef void (*assembly_ptr)(const char *, printf_ptr);

static IRAM_ATTR uint8_t assembly[4096] __attribute__((aligned (4))) = {0};

extern uint32_t tud_cdc_n_write(uint8_t itf, void const* buffer, uint32_t bufsize);

uint32_t tud_cdc_n_write_str (uint8_t itf, char const* str)
{
  return tud_cdc_n_write(itf, str, strlen(str));
}

static uint32_t tud_cdc_write_str (char const* str)
{
  return tud_cdc_n_write_str(0, str);
}

static mp_obj_t rawexec_call_(mp_obj_t _text, mp_obj_t _assembly) {
  mp_uint_t text_len;
  const char *text = (char *) mp_obj_str_get_data(_text, &text_len);

  bool is_bytes = MP_OBJ_IS_TYPE(_assembly, &mp_type_bytes);
  if (!is_bytes) {
    mp_raise_msg(&mp_type_AttributeError, "Assembly should be a bytestring");
  }

  mp_uint_t assembly_len;
  uint8_t *assembly_pointer = (uint8_t *) mp_obj_str_get_data(_assembly, &assembly_len);
  printf(
    "assembly_len: %lu, assembly location: %p, first dword: %04lX\n",
    assembly_len, assembly, *((uint32_t *) assembly_pointer)
  );
  memcpy(assembly, assembly_pointer, assembly_len);

  assembly_ptr call_assembly = assembly;
  call_assembly(text, tud_cdc_write_str);

  return mp_const_none;
}

static MP_DEFINE_CONST_FUN_OBJ_2(rawexec_call_obj, rawexec_call_);

static const mp_rom_map_elem_t rawexec_module_globals_table[] = {
  {MP_ROM_QSTR(MP_QSTR_call), MP_ROM_PTR(&rawexec_call_obj)},
};

static MP_DEFINE_CONST_DICT(rawexec_module_globals, rawexec_module_globals_table);

const mp_obj_module_t rawexec_module = {
  .base = {&mp_type_module},
  .globals = (mp_obj_dict_t *) &rawexec_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_rawexec, rawexec_module);