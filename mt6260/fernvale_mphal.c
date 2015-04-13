/*
 * This file is part of the Micro Python project, http://micropython.org/
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2014 Damien P. George
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

#include <stdio.h>
#include <stdint.h>

#include "serial.h"
#include "memio.h"
#include "fernvale-pmic.h"

//extern void ets_wdt_disable(void);
//extern void wdt_feed(void);
//extern void ets_delay_us();

void mp_hal_init(void) {
    serial_init();

    /* Disable system watchdog */
    writel(0x2200, 0xa0030000);

    /* Enable USB Download mode (required for no-battery operation) */
    writew(0x8000, PMIC_CTRL10);

    /* Disable battery watchdog */
    writew(0x2, PMIC_CTRL9);
}

void mp_hal_feed_watchdog(void) {
    //ets_wdt_disable(); // it's a pain while developing
    //WRITE_PERI_REG(0x60000914, 0x73);
    //wdt_feed(); // might also work
}

void mp_hal_udelay(uint32_t us) {
    uint32_t i, j;
    for (i = 0; i < us; i++) {
        for (j = 0; j < 73; j++) {
            asm("nop");
        }
    }
}

int mp_hal_stdin_rx_chr(void) {
    return serial_getc();
}

void mp_hal_stdout_tx_str(const char *str) {
    while (*str)
        serial_putc(*str++);
}

void mp_hal_stdout_tx_strn(const char *str, uint32_t len) {
    while (len--)
        serial_putc(*str++);
}

void mp_hal_stdout_tx_strn_cooked(const char *str, uint32_t len) {
    while (len--) {
        if (*str == '\n') {
            serial_putc('\r');
        }
        serial_putc(*str++);
    }
}

uint32_t HAL_GetTick(void) {
    // TODO
    return 0;
}

void HAL_Delay(uint32_t Delay) {
    mp_hal_udelay(Delay * 1000);
}

void mp_hal_set_interrupt_char(int c) {
    // TODO
}

uint32_t mp_hal_get_cpu_freq(void) {
    return 364 * 1000 * 1000;
}
