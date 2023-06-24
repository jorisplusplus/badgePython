#include <stdio.h>
#include <string.h>

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"

#ifndef NO_QSTR
#include "display.h"
#endif

STATIC mp_obj_t mpy_display_flush(mp_obj_t buffer) {
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buffer, &bufinfo, MP_BUFFER_READ);

    display_write_partial(bufinfo.buf, 0, 0, DISPLAY_WIDTH, DISPLAY_HEIGHT);
    
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_1(mpy_display_flush_obj, mpy_display_flush);

//=========================================================
STATIC const mp_rom_map_elem_t display_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_flush),   (mp_obj_t)&mpy_display_flush_obj},
    { MP_ROM_QSTR(MP_QSTR_WIDTH),       MP_ROM_INT(DISPLAY_WIDTH)},
    { MP_ROM_QSTR(MP_QSTR_HEIGHT),      MP_ROM_INT(DISPLAY_HEIGHT)}
};
STATIC MP_DEFINE_CONST_DICT(display_module_globals, display_module_globals_table);

//===================================
const mp_obj_module_t display_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&display_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_udisplay, display_module);