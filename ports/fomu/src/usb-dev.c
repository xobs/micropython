#include <stdint.h>
#include <unistd.h>
#include <usb.h>
#include <system.h>
#include <usb-desc.h>

static uint8_t reply_buffer[8];
static uint8_t usb_configuration = 0;
static uint8_t data_buffer[64];
static uint8_t line_coding[7];
volatile uint8_t terminal_is_connected;

__attribute__((section(".ramtext")))
void usb_setup(const struct usb_setup_request *setup) {
    const uint8_t *data = NULL;
    uint32_t datalen = 0;
    const usb_descriptor_list_t *list;
    uint8_t ep_dir_is_in = setup->bmRequestType >> 7;

    switch (setup->wRequestAndType)
    {
    case 0x2021: // Set Line Coding
        if (setup->wLength)
            usb_recv(line_coding, setup->wLength);
        usb_ack_in(0);
        return;

    case 0x21A1: // Get Line Coding
        datalen = sizeof(line_coding);
        data = line_coding;
        break;

    case 0x2221: // Set control line state
        terminal_is_connected = setup->wValue & 1;
        if (setup->wLength)
            usb_recv(data_buffer, setup->wLength);
        usb_ack_in(0);
        return;

    case 0x0500: // SET_ADDRESS
        usb_set_address(setup->wValue);
        break;

    case 0x0b01: // SET_INTERFACE
        break;

    case 0x0900: // SET_CONFIGURATION
        usb_configuration = setup->wValue;
        usb_ack_in(0);
        return;

    case 0x0880: // GET_CONFIGURATION
        reply_buffer[0] = usb_configuration;
        datalen = 1;
        data = reply_buffer;
        break;

    case 0x0080: // GET_STATUS (device)
        reply_buffer[0] = 0;
        reply_buffer[1] = 0;
        datalen = 2;
        data = reply_buffer;
        break;

    case 0x0082: // GET_STATUS (endpoint)
        if (setup->wIndex > 0)
        {
            usb_err_in(0);
            return;
        }
        reply_buffer[0] = 0;
        reply_buffer[1] = 0;
        data = reply_buffer;
        datalen = 2;
        break;

    case 0x0102: // CLEAR_FEATURE (endpoint)
        if (setup->wIndex > 0 || setup->wValue != 0)
        {
            usb_err_out(0);
            return;
        }
        break;

    case 0x0302: // SET_FEATURE (endpoint)
        if (setup->wIndex > 0 || setup->wValue != 0)
        {
            usb_err_out(0);
            return;
        }
        break;

    case 0x0680: // GET_DESCRIPTOR
    case 0x0681:
        for (list = usb_descriptor_list; 1; list++)
        {
            if (list->addr == NULL)
                break;
            if (setup->wValue == list->wValue)
            {
                data = list->addr;
                if ((setup->wValue >> 8) == 3)
                {
                    // for string descriptors, use the descriptor's
                    // length field, allowing runtime configured
                    // length.
                    datalen = *(list->addr);
                }
                else
                {
                    datalen = list->length;
                }
                goto send;
            }
        }
        usb_err_in(0);
        return;
#if 0
    case (MSFT_VENDOR_CODE << 8) | 0xC0: // Get Microsoft descriptor
    case (MSFT_VENDOR_CODE << 8) | 0xC1:
        if (setup->wIndex == 0x0004)
        {
            // Return WCID descriptor
            data = usb_microsoft_wcid;
            datalen = MSFT_WCID_LEN;
            break;
        }
        usb_err_in(0);
        return;

    case (WEBUSB_VENDOR_CODE << 8) | 0xC0: // Get WebUSB descriptor
        if (setup->wIndex == 0x0002)
        {
            if (setup->wValue == 0x0001)
            {
                data = (uint8_t*)&landing_url_descriptor;
                datalen = LANDING_PAGE_DESCRIPTOR_SIZE;
                break;
            }
        }
        usb_err_in(0);
        return;
#endif

    default:
        usb_err_in(0);
        return;
    }

send:
    if (data && datalen) {
        if (datalen > setup->wLength)
            datalen = setup->wLength;
        usb_send(data, datalen);
        usb_ack_out(0);
    }
    else {
        // Ack the opposite endpoint type
        if (ep_dir_is_in)
            usb_ack_out(0);
        else
            usb_ack_in(0);
    }
    return;
}
