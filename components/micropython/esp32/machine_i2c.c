/*
 * This file is part of the MicroPython project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Damien P. George
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "py/runtime.h"
#include "py/mphal.h"
#include "py/mperrno.h"
#include "extmod/machine_i2c.h"
#include "modmachine.h"
#include "buses.h"

#ifndef NO_QSTR
#include "driver/i2c.h"
#include "i2c_bus.h"
#endif

#define I2C_DEFAULT_TIMEOUT_US (10000) // 10ms

typedef struct _machine_hw_i2c_obj_t {
    mp_obj_base_t base;
    mp_int_t i2c_id;
    mp_int_t i2c_addr;
    i2c_bus_device_handle_t device;
    struct _machine_hw_i2c_obj_t *next;
} machine_hw_i2c_obj_t;

int machine_hw_i2c_transfer(mp_obj_base_t *self_in, uint16_t addr, size_t n, mp_machine_i2c_buf_t *bufs, unsigned int flags) {
    machine_hw_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);   

    int data_len = 0;
    esp_err_t err = ESP_OK;
    if (flags & MP_MACHINE_I2C_FLAG_WRITE1) {
        err = i2c_bus_write_bytes(self->device, NULL_I2C_MEM_ADDR, bufs->len, bufs->buf);
        data_len += bufs->len;
        --n;
        ++bufs;
    }

    for (; n--; ++bufs) {
        if (flags & MP_MACHINE_I2C_FLAG_READ) {
            err = i2c_bus_read_bytes(self->device, NULL_I2C_MEM_ADDR, bufs->len, bufs->buf);
        } else {
            if (bufs->len != 0) {
                err = i2c_bus_write_bytes(self->device, NULL_I2C_MEM_ADDR, bufs->len, bufs->buf);
            }
        }
        data_len += bufs->len;
    }

    if (err == ESP_FAIL) {
        return -MP_ENODEV;
    } else if (err == ESP_ERR_TIMEOUT) {
        return -MP_ETIMEDOUT;
    } else if (err != ESP_OK) {
        return -abs(err);
    }

    return data_len;
}

/******************************************************************************/
// MicroPython bindings for machine API

STATIC void machine_hw_i2c_print(const mp_print_t *print, mp_obj_t self_in, mp_print_kind_t kind) {
    machine_hw_i2c_obj_t *self = MP_OBJ_TO_PTR(self_in);
    mp_printf(print, "I2C(%u)", self->i2c_addr);
}

mp_obj_t machine_hw_i2c_make_new(const mp_obj_type_t *type, size_t n_args, size_t n_kw, const mp_obj_t *all_args) {
    MP_MACHINE_I2C_CHECK_FOR_LEGACY_SOFTI2C_CONSTRUCTION(n_args, n_kw, all_args);

    // Parse args
    enum { ARG_id, ARG_addr, ARG_freq, ARG_timeout };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_id, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_addr, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
        { MP_QSTR_freq, MP_ARG_REQUIRED | MP_ARG_OBJ, {.u_obj = MP_OBJ_NULL} },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all_kw_array(n_args, n_kw, all_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    // Get I2C bus
    mp_int_t i2c_id = mp_obj_get_int(args[ARG_id].u_obj);
    i2c_bus_handle_t bus = get_i2c_bus(i2c_id);
    if (bus == NULL) {
        mp_raise_msg_varg(&mp_type_ValueError, MP_ERROR_TEXT("I2C(%d) doesn't exist"), i2c_id);
    }
    mp_int_t i2c_addr = mp_obj_get_int(args[ARG_addr].u_obj);

    for (machine_hw_i2c_obj_t *t = MP_STATE_PORT(machine_hw_i2c_obj_head); t; t = t->next) {
        if (t->i2c_addr == i2c_addr && t->i2c_id == i2c_id) {
            return t;
        }
    }

    mp_int_t i2c_speed = mp_obj_get_int(args[ARG_freq].u_obj);

    // Get static peripheral object
    machine_hw_i2c_obj_t *self = mp_obj_malloc(machine_hw_i2c_obj_t, &machine_hw_i2c_type);
    self->i2c_addr = i2c_addr;
    self->i2c_id = i2c_id;
    self->device = i2c_bus_device_create(bus, self->i2c_addr, i2c_speed);

    self->next = MP_STATE_PORT(machine_hw_i2c_obj_head);
    MP_STATE_PORT(machine_hw_i2c_obj_head) = self;

    return MP_OBJ_FROM_PTR(self);
}

STATIC const mp_machine_i2c_p_t machine_hw_i2c_p = {
    .transfer = machine_hw_i2c_transfer,
};

const mp_obj_type_t machine_hw_i2c_type = {
    { &mp_type_type },
    .name = MP_QSTR_I2C,
    .print = machine_hw_i2c_print,
    .make_new = machine_hw_i2c_make_new,
    .protocol = &machine_hw_i2c_p,
    .locals_dict = (mp_obj_dict_t *)&mp_machine_i2c_locals_dict,
};
