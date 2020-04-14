#include <string.h>

#include "py/runtime.h"

#include "shared-module/aes/__init__.h"

void common_hal_aes_construct(aes_obj_t* self,
                              const uint8_t* key,
                              const uint8_t* iv,
                              int mode) {
    memset(&self->ctx, 0, sizeof(self->ctx));
    if (iv != NULL) {
        AES_init_ctx_iv(&self->ctx, key, iv);
    } else {
        AES_init_ctx(&self->ctx, key);
    }
    self->mode = mode;
}

void common_hal_aes_encrypt(aes_obj_t* self,
                            uint8_t* buffer,
                            size_t length) {
    switch (self->mode) {
        case AES_MODE_ECB:
            if (length != 16)
                mp_raise_msg(&mp_type_ValueError, translate("ecb only operates on 16 bytes at a time"));
            AES_ECB_encrypt(&self->ctx, buffer);
            break;
        case AES_MODE_CBC:
            if ((length & 15) != 0)
                mp_raise_msg(&mp_type_ValueError, translate("cbc blocks must be multiples of 16 bytes"));
            AES_CBC_encrypt_buffer(&self->ctx, buffer, length);
            break;
        case AES_MODE_CTR:
            AES_CTR_xcrypt_buffer(&self->ctx, buffer, length);
            break;
        default:
            mp_raise_msg(&mp_type_ValueError, translate("unknown encryption mode"));
    }
}

void common_hal_aes_decrypt(aes_obj_t* self,
                            uint8_t* buffer,
                            size_t length) {
    switch (self->mode) {
        case AES_MODE_ECB:
            if (length != 16)
                mp_raise_msg(&mp_type_ValueError, translate("ecp only operates on 16 bytes at a time"));
            AES_ECB_decrypt(&self->ctx, buffer);
            break;
        case AES_MODE_CBC:
            if ((length & 15) != 0)
                mp_raise_msg(&mp_type_ValueError, translate("cbc blocks must be multiples of 16 bytes"));
            AES_CBC_decrypt_buffer(&self->ctx, buffer, length);
            break;
        case AES_MODE_CTR:
            AES_CTR_xcrypt_buffer(&self->ctx, buffer, length);
            break;
        default:
            mp_raise_msg(&mp_type_ValueError, translate("unknown encryption mode"));
    }
}
