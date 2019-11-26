#include <stdint.h>
#include <generated/csr.h>
#include <usb.h>

struct usb_setup_request;

void epfifo_usb_isr(void);
void epfifo_usb_init(void);
void epfifo_usb_connect(void);
void epfifo_usb_idle(void);
void epfifo_usb_disconnect(void);
void epfifo_usb_set_address(uint8_t address);
void epfifo_usb_ack_in(uint8_t epno);
void epfifo_usb_ack_out(uint8_t epno);
void epfifo_usb_err_in(uint8_t epno);
void epfifo_usb_err_out(uint8_t epno);
void epfifo_usb_send(const void *data, int total_count);
int epfifo_usb_can_getc(void);
int epfifo_usb_can_putc(void);
int epfifo_usb_write(const char *buf, int count);
int epfifo_usb_getc(void);
void epfifo_usb_putc(char c);
void epfifo_usb_poll(void);

void eptri_usb_isr(void);
void eptri_usb_init(void);
void eptri_usb_connect(void);
void eptri_usb_idle(void);
void eptri_usb_disconnect(void);
void eptri_usb_set_address(uint8_t address);
void eptri_usb_ack_in(uint8_t epno);
void eptri_usb_ack_out(uint8_t epno);
void eptri_usb_err_in(uint8_t epno);
void eptri_usb_err_out(uint8_t epno);
void eptri_usb_send(const void *data, int total_count);
int eptri_usb_can_getc(void);
int eptri_usb_can_putc(void);
int eptri_usb_write(const char *buf, int count);
int eptri_usb_getc(void);
void eptri_usb_putc(char c);
void eptri_usb_poll(void);

void usb_isr(void) {
    if (version_major_read() == 2)
        eptri_usb_isr();
    else
        epfifo_usb_isr();
}

void usb_init(void) {
    if (version_major_read() == 2)
        eptri_usb_init();
    else
        epfifo_usb_init();
}

void usb_connect(void) {
    if (version_major_read() == 2)
        eptri_usb_connect();
    else
        epfifo_usb_connect();
}

void usb_idle(void) {
    if (version_major_read() == 2)
        eptri_usb_idle();
    else
        epfifo_usb_idle();
}

void usb_disconnect(void) {
    if (version_major_read() == 2)
        eptri_usb_disconnect();
    else
        epfifo_usb_disconnect();
}

void usb_set_address(uint8_t address) {
    if (version_major_read() == 2)
        eptri_usb_set_address(address);
    else
        epfifo_usb_set_address(address);
}

void usb_ack_in(uint8_t epno) {
    if (version_major_read() == 2)
        eptri_usb_ack_in(epno);
    else
        epfifo_usb_ack_in(epno);
}

void usb_ack_out(uint8_t epno) {
    if (version_major_read() == 2)
        eptri_usb_ack_out(epno);
    else
        epfifo_usb_ack_out(epno);
}

void usb_err_in(uint8_t epno) {
    if (version_major_read() == 2)
        eptri_usb_err_in(epno);
    else
        epfifo_usb_err_in(epno);
}

void usb_err_out(uint8_t epno) {
    if (version_major_read() == 2)
        eptri_usb_err_out(epno);
    else
        epfifo_usb_err_out(epno);
}

void usb_send(const void *data, int total_count) {
    if (version_major_read() == 2)
        eptri_usb_send(data, total_count);
    else
        epfifo_usb_send(data, total_count);
}

int usb_can_getc(void) {
    if (version_major_read() == 2)
        return eptri_usb_can_getc();
    else
        return epfifo_usb_can_getc();
}

int usb_can_putc(void) {
    if (version_major_read() == 2)
        return eptri_usb_can_putc();
    else
        return epfifo_usb_can_putc();
}

int usb_write(const char *buf, int count) {
    if (version_major_read() == 2)
        return eptri_usb_write(buf, count);
    else
        return epfifo_usb_write(buf, count);
}

int usb_getc(void) {
    if (version_major_read() == 2)
        return eptri_usb_getc();
    else
        return epfifo_usb_getc();
}

void usb_putc(char c) {
    if (version_major_read() == 2)
        eptri_usb_putc(c);
    else
        epfifo_usb_putc(c);
}

void usb_poll(void) {
    if (version_major_read() == 2)
        eptri_usb_poll();
    else
        epfifo_usb_poll();
}

int usb_recv(void *buffer, unsigned int buffer_len) {
    if (version_major_read() == 2)
        return eptri_usb_recv(buffer, buffer_len);
    else
        return epfifo_usb_recv(buffer, buffer_len);
}
