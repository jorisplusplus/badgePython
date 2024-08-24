#include <stdio.h>
#include <string.h>

#include "sdkconfig.h"

#include "py/obj.h"
#include "py/objstr.h"
#include "py/runtime.h"

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_heap_task_info.h"
#include "esp_log.h"

#include "include/usb_cdc_helpers.h"

//--------------------------------------------------------
static mp_obj_t usb_cdc_connected() {
	return is_usb_cdc_connected() ? mp_const_true : mp_const_false;
}
static MP_DEFINE_CONST_FUN_OBJ_0(usb_cdc_connected_obj, usb_cdc_connected);


//=========================================================
static const mp_rom_map_elem_t usb_module_globals_table[] = {
    { MP_OBJ_NEW_QSTR(MP_QSTR_cdc_connected),		(mp_obj_t)&usb_cdc_connected_obj}
};
static MP_DEFINE_CONST_DICT(usb_module_globals, usb_module_globals_table);

//===================================
const mp_obj_module_t usb_module = {
    .base = {&mp_type_module},
    .globals = (mp_obj_dict_t *)&usb_module_globals,
};

MP_REGISTER_MODULE(MP_QSTR_usb, usb_module);