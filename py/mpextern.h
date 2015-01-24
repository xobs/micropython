/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Damien P. George
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
#ifndef __MICROPY_INCLUDED_PY_MPEXTERN_H__
#define __MICROPY_INCLUDED_PY_MPEXTERN_H__

#include "py/runtime0.h"
#include "py/runtime.h"

#define MP_EXT_VERSION_MAJOR (0)
#define MP_EXT_VERSION_MINOR (0)
#define MP_EXT_VERSION_SUBMINOR (1)
#define MP_EXT_ARCH_X86    (1)
#define MP_EXT_ARCH_X64    (2)
#define MP_EXT_ARCH_ARM    (3)
#define MP_EXT_ARCH_THUMB2 (4)

// TODO auto-detect current arch
#define MP_EXT_ARCH_CURRENT (MP_EXT_ARCH_X86)

#define MP_EXT_HEADER \
    __attribute__((section(".mpyheader"))) \
    const byte header[8] = { \
        'M', 'P', 'Y', \
        MP_EXT_VERSION_MAJOR, MP_EXT_VERSION_MINOR, MP_EXT_VERSION_SUBMINOR, \
        MP_EXT_ARCH_CURRENT, 0, \
    };

#define MP_EXT_INIT \
    __attribute__((section(".mpytext")))

typedef struct _mp_ext_table_t {
    mp_obj_t mp_const_none_;
    mp_obj_t mp_const_false_;
    mp_obj_t mp_const_true_;
    mp_obj_t (*mp_obj_new_fun_extern)(bool is_kw, mp_uint_t n_args_min, mp_uint_t n_args_max, void *f);
    qstr (*qstr_from_str)(const char *str);
    void (*mp_store_global)(qstr qst, mp_obj_t obj);
    mp_obj_t (*mp_obj_new_list)(mp_uint_t n, mp_obj_t *items);
    mp_obj_t (*mp_binary_op)(mp_uint_t op, mp_obj_t lhs, mp_obj_t rhs);
} mp_ext_table_t;

void mp_extern_load(const char *ext_name, mp_obj_dict_t *globals);

// to be implemented per-port
const byte *mp_extern_load_binary(const char *ext_name);

// entry point for the extern binary
void init(const mp_ext_table_t *et);

#endif // __MICROPY_INCLUDED_PY_MPEXTERN_H__
