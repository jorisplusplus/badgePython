#include "include/fluidsim.h"

#include <stdio.h>
#include <string.h>

#include "sdkconfig.h"

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"

#include "mphalport.h"
#include "modmachine.h"

// ====== RTC memory functions ============================
/**
 * Usages
 * pos 0 : ???
 * pos 1 : ???
 * pos 2 : heap limit (0 no heap limit)
 */

//--------------------------------------------------------------------------------
static mp_obj_t esp_fluidsim_update(mp_obj_t accel_x_obj, mp_obj_t accel_y_obj, mp_obj_t accel_z_obj) {
  float accel_x = mp_obj_get_float_to_f(accel_x_obj);
  float accel_y = mp_obj_get_float_to_f(accel_y_obj);
  float accel_z = mp_obj_get_float_to_f(accel_z_obj);
  fluidsim_step(accel_x, accel_y, accel_z);
	return mp_const_true;
}
static MP_DEFINE_CONST_FUN_OBJ_3(esp_fluidsim_update_obj, esp_fluidsim_update);

//=========================================================
static const mp_rom_map_elem_t fluidsim_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_update),			(mp_obj_t)&esp_fluidsim_update_obj},
};
static MP_DEFINE_CONST_DICT(fluidsim_module_globals, fluidsim_module_globals_table);

//===================================
const mp_obj_module_t fluidsim_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&fluidsim_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_fluidsim, fluidsim_module);