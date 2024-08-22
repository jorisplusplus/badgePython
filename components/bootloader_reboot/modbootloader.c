#include "include/bootloader.h"

#include <stdio.h>
#include <string.h>

#include "sdkconfig.h"

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"

#include "mphalport.h"
#include "modmachine.h"

//--------------------------------------------------------------------------------
static mp_obj_t bootloader_enter(mp_obj_t dfu_mode) {
  mp_int_t is_dfu_mode = mp_obj_get_int(dfu_mode);
  reboot_into_bootloader((bool) is_dfu_mode);
	return mp_const_true;
}
static MP_DEFINE_CONST_FUN_OBJ_1(bootloader_enter_obj, bootloader_enter);

//=========================================================
static const mp_rom_map_elem_t bootloader_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_enter),			(mp_obj_t)&bootloader_enter_obj},
};
static MP_DEFINE_CONST_DICT(bootloader_module_globals, bootloader_module_globals_table);

//===================================
const mp_obj_module_t bootloader_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&bootloader_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_bootloader, bootloader_module);