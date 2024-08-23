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
static mp_obj_t fluidsim_begin_fun(mp_obj_t num_particles, mp_obj_t colour) {
  mp_int_t num_int = mp_obj_get_int(num_particles);
  mp_int_t colour_int = mp_obj_get_int(colour);
  fluidsim_begin(num_int, (colour_int << 8) + 0xFF);
  return mp_const_true;
}
static MP_DEFINE_CONST_FUN_OBJ_2(fluidsim_begin_obj, fluidsim_begin_fun);

static mp_obj_t fluidsim_has_particle_fun(mp_obj_t x, mp_obj_t y) {
  mp_int_t x_int = mp_obj_get_int(x);
  mp_int_t y_int = mp_obj_get_int(y);
  return fluidsim_has_particle(x_int, y_int) ? mp_const_true : mp_const_false;
}
static MP_DEFINE_CONST_FUN_OBJ_2(fluidsim_has_particle_obj, fluidsim_has_particle_fun);

static mp_obj_t fluidsim_push_fun() {
  fluidsim_push_particle();
  return mp_const_true;
}
static MP_DEFINE_CONST_FUN_OBJ_0(fluidsim_push_obj, fluidsim_push_fun);

static mp_obj_t fluidsim_pop_fun() {
  fluidsim_pop_particle();
  return mp_const_true;
}
static MP_DEFINE_CONST_FUN_OBJ_0(fluidsim_pop_obj, fluidsim_pop_fun);

static mp_obj_t fluidsim_num_particles_fun() {
  fluidsim_num_particles();
  return mp_const_true;
}
static MP_DEFINE_CONST_FUN_OBJ_0(fluidsim_num_particles_obj, fluidsim_num_particles_fun);

//=========================================================
static const mp_rom_map_elem_t fluidsim_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_begin),			(mp_obj_t)&fluidsim_begin_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_has_particle),			(mp_obj_t)&fluidsim_has_particle_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_push_particle),			(mp_obj_t)&fluidsim_push_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_pop_particle),			(mp_obj_t)&fluidsim_pop_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_num_particles),			(mp_obj_t)&fluidsim_num_particles_obj},
};
static MP_DEFINE_CONST_DICT(fluidsim_module_globals, fluidsim_module_globals_table);

//===================================
const mp_obj_module_t fluidsim_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&fluidsim_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_fluidsim, fluidsim_module);