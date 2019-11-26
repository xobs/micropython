#ifndef __USB_H
#define __USB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

struct usb_setup_request;

void usb_isr(void);
void usb_init(void);
void usb_connect(void);
void usb_idle(void);
void usb_disconnect(void);
void usb_setup(const struct usb_setup_request *setup);
void usb_set_address(uint8_t address);
void usb_poll(void);
int usb_recv(void *buffer, unsigned int buffer_len);

void usb_ack_in(uint8_t epno);
void usb_ack_out(uint8_t epno);
void usb_err_in(uint8_t epno);
void usb_err_out(uint8_t epno);

void usb_send(const void *data, int total_count);

int usb_can_getc(void);
int usb_can_putc(void);
int usb_write(const char *buf, int count);
int usb_getc(void);
void usb_putc(char c);

#ifdef __cplusplus
}
#endif

#endif
