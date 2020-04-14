#include <stdint.h>
#include <string.h>

#include "py/obj.h"
#include "py/runtime.h"

#include "shared-bindings/aes/__init__.h"

STATIC mp_obj_t aes_make_new(const mp_obj_type_t *type, size_t n_args, const mp_obj_t *pos_args, mp_map_t *kw_args) {
    enum { ARG_mode, ARG_key, ARG_iv };
    static const mp_arg_t allowed_args[] = {
        { MP_QSTR_mode, MP_ARG_INT | MP_ARG_REQUIRED },
        { MP_QSTR_key, MP_ARG_OBJ | MP_ARG_REQUIRED },
        { MP_QSTR_iv, MP_ARG_OBJ },
    };
    mp_arg_val_t args[MP_ARRAY_SIZE(allowed_args)];
    mp_arg_parse_all(n_args, pos_args, kw_args, MP_ARRAY_SIZE(allowed_args), allowed_args, args);

    aes_obj_t *self = m_new_obj(aes_obj_t);
    self->base.type = &aes_type;
    mp_buffer_info_t bufinfo;
    switch (args[ARG_mode].u_int) {
        case AES_MODE_CBC: /* CBC */ break;
        case AES_MODE_ECB: /* ECB */ break;
        case AES_MODE_CTR: /* CTR */ break;
        default: mp_raise_TypeError(translate("mode must be aes.MODE_CBC, aes.MODE_ECB, or aes.MODE_CTR"));
    }
    int mode = args[ARG_mode].u_int;

    const uint8_t *key = NULL;
    if (mp_get_buffer(args[ARG_key].u_obj, &bufinfo, MP_BUFFER_READ)) {
        if (bufinfo.len != AES_KEYLEN) {
            mp_raise_TypeError(translate("key must be 256 bits (32 bytes)"));
        }
        key = bufinfo.buf;
    } else {
        mp_raise_TypeError(translate("must specify a key"));
    }

    const uint8_t *iv = NULL;
    if (args[ARG_iv].u_obj != NULL && mp_get_buffer(args[ARG_iv].u_obj, &bufinfo, MP_BUFFER_READ)) {
        if (bufinfo.len != AES_BLOCKLEN) {
            mp_raise_TypeError(translate("iv must be 16 bytes long"));
        }
        iv = bufinfo.buf;
    }

    common_hal_aes_construct(self, key, iv, mode);
    return MP_OBJ_FROM_PTR(self);
}

STATIC byte *duplicate_data(const byte* src_buf, size_t len) {
    byte *dest_buf = m_new(byte, len);
    memcpy(dest_buf, src_buf, len);
    return dest_buf;
}

STATIC mp_obj_t aes_encrypt_in_place(mp_obj_t aes_obj, mp_obj_t buf) {
    if (!MP_OBJ_IS_TYPE(aes_obj, &aes_type)) {
        mp_raise_TypeError_varg(translate("Expected a %q"), aes_type.name);
    }
    // Convert parameters into expected types.
    aes_obj_t *aes = MP_OBJ_TO_PTR(aes_obj);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
    common_hal_aes_encrypt(aes, (uint8_t*)bufinfo.buf, bufinfo.len);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(aes_encrypt_in_place_obj, aes_encrypt_in_place);

STATIC mp_obj_t aes_encrypt(mp_obj_t aes_obj, mp_obj_t buf) {
    if (!MP_OBJ_IS_TYPE(aes_obj, &aes_type)) {
        mp_raise_TypeError_varg(translate("Expected a %q"), aes_type.name);
    }
    // Convert parameters into expected types.
    aes_obj_t *aes = MP_OBJ_TO_PTR(aes_obj);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);

    byte *dest_buf = duplicate_data((const byte *)bufinfo.buf, bufinfo.len);
    common_hal_aes_encrypt(aes, (uint8_t*)dest_buf, bufinfo.len);

    return mp_obj_new_bytearray_by_ref(bufinfo.len, dest_buf);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(aes_encrypt_obj, aes_encrypt);

STATIC mp_obj_t aes_decrypt_in_place(mp_obj_t aes_obj, mp_obj_t buf) {
    if (!MP_OBJ_IS_TYPE(aes_obj, &aes_type)) {
        mp_raise_TypeError_varg(translate("Expected a %q"), aes_type.name);
    }
    // Convert parameters into expected types.
    aes_obj_t *aes = MP_OBJ_TO_PTR(aes_obj);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);
    common_hal_aes_decrypt(aes, (uint8_t*)bufinfo.buf, bufinfo.len);
    return mp_const_none;
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(aes_decrypt_in_place_obj, aes_decrypt_in_place);

STATIC mp_obj_t aes_decrypt(mp_obj_t aes_obj, mp_obj_t buf) {
    if (!MP_OBJ_IS_TYPE(aes_obj, &aes_type)) {
        mp_raise_TypeError_varg(translate("Expected a %q"), aes_type.name);
    }
    // Convert parameters into expected types.
    aes_obj_t *aes = MP_OBJ_TO_PTR(aes_obj);
    mp_buffer_info_t bufinfo;
    mp_get_buffer_raise(buf, &bufinfo, MP_BUFFER_READ);

    byte *dest_buf = duplicate_data((const byte *)bufinfo.buf, bufinfo.len);
    common_hal_aes_decrypt(aes, (uint8_t*)dest_buf, bufinfo.len);
    return mp_obj_new_bytearray_by_ref(bufinfo.len, dest_buf);
}
STATIC MP_DEFINE_CONST_FUN_OBJ_2(aes_decrypt_obj, aes_decrypt);


STATIC const mp_rom_map_elem_t aes_locals_dict_table[] = {
    // Methods
    { MP_ROM_QSTR(MP_QSTR___name__), MP_OBJ_NEW_QSTR(MP_QSTR_aes) },
    { MP_ROM_QSTR(MP_QSTR_encrypt), (mp_obj_t)&aes_encrypt_obj },
    { MP_ROM_QSTR(MP_QSTR_decrypt), (mp_obj_t)&aes_decrypt_obj },
    { MP_ROM_QSTR(MP_QSTR_encrypt_in_place), (mp_obj_t)&aes_encrypt_in_place_obj },
    { MP_ROM_QSTR(MP_QSTR_decrypt_in_place), (mp_obj_t)&aes_decrypt_in_place_obj },
    // { MP_ROM_QSTR(MP_QSTR___enter__), MP_ROM_PTR(&default___enter___obj) },
    // { MP_ROM_QSTR(MP_QSTR___exit__), MP_ROM_PTR(&default___exit___obj) },

    // // Properties
    // { MP_ROM_QSTR(MP_QSTR_sample_rate), MP_ROM_PTR(&audioio_rawsample_sample_rate_obj) },
};
STATIC MP_DEFINE_CONST_DICT(aes_locals_dict, aes_locals_dict_table);

const mp_obj_type_t aes_type = {
    { &mp_type_type },
    .name = MP_QSTR_AES,
    .make_new = aes_make_new,
    .locals_dict = (mp_obj_dict_t*)&aes_locals_dict,
};
