/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Damien P. George
 * Copyright (c) 2015 Daniel Campora
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

#include "py/mpextern.h"
#include "py/misc.h"
#include "ff.h"


const byte *mp_extern_load_binary(const char *ext_name) {
    FILINFO fno;
    FRESULT res = f_stat(ext_name, &fno);
    if (res != FR_OK) {
        return NULL;
    }
    FIL fp;
    res = f_open(&fp, ext_name, FA_READ);
    if (res != FR_OK) {
        return NULL;
    }
    byte *buf = m_new(byte, fno.fsize);
    UINT n;
    res = f_read(&fp, buf, fno.fsize, &n);
    if (res != FR_OK) {
        m_del(byte, buf, fno.fsize);
        return NULL;
    }
    f_close(&fp);
    return buf;
}

