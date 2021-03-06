#include <unistd.h>
#include "py/mpconfig.h"
#include "usb.h"

/*
 * Core UART functions to implement for a port
 */

// Receive single character
int mp_hal_stdin_rx_chr(void) {
    do {
        usb_poll();
    } while (!usb_can_getc());
    return usb_getc();
}

// Send string of given length
void mp_hal_stdout_tx_strn(const char *str, mp_uint_t len) {
    while (len) {
        while (!usb_can_putc())
            usb_poll();
        int bytes_written = usb_write(str, len);
        str += bytes_written;
        len -= bytes_written;
    }
}
