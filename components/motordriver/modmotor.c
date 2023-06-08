#include <stdio.h>
#include <string.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"

#ifndef NO_QSTR
#include "motorindent.h"
#endif

STATIC mp_obj_t mpy_motor_read() {
	return mp_obj_new_float(motor_latest_angle()*180.0f/3.142f);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mpy_motor_read_obj, mpy_motor_read);

static mp_obj_t mpy_motor_indent_reset() {
    motor_indent_destroy();
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mpy_motor_indent_reset_obj, mpy_motor_indent_reset);

static mp_obj_t mpy_motor_indent_add(size_t n_args, const mp_obj_t *args) {
    if (n_args != 4) {
        mp_raise_ValueError("Expected 4 arguments: angle, cw_range, ccw_range, power");
        return mp_const_none;
    }
    float angle = mp_obj_get_float(args[0]);
    float cw_range = mp_obj_get_float(args[1]);
    float ccw_range = mp_obj_get_float(args[2]);
    float power = mp_obj_get_float(args[3]);
    motor_indent_register(angle, cw_range, ccw_range, power);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_VAR(mpy_motor_indent_add_obj, 4, mpy_motor_indent_add);

static mp_obj_t mpy_motor_status(mp_obj_t enable) {
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_0(mpy_motor_status_obj, mpy_motor_status);

//=========================================================
STATIC const mp_rom_map_elem_t motor_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_reset),			(mp_obj_t)&mpy_motor_indent_reset_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_read),			(mp_obj_t)&mpy_motor_read_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_add),			    (mp_obj_t)&mpy_motor_indent_add_obj},
    { MP_OBJ_NEW_QSTR(MP_QSTR_status),          (mp_obj_t)&mpy_motor_status_obj}
};
STATIC MP_DEFINE_CONST_DICT(motor_module_globals, motor_module_globals_table);

//===================================
const mp_obj_module_t motor_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&motor_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_motor, motor_module);