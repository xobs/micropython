#include "py/mpextern.h"

static unsigned char dummy_data[4] = {1, 2, 3, 4};

STATIC mp_obj_t dummy_compute(const mp_ext_table_t *et, mp_obj_t val1, mp_obj_t val2) {
    mp_uint_t _val1 = et->mp_obj_get_int(val1);
    mp_uint_t _val2 = et->mp_obj_get_int(val2);

    return MP_OBJ_NEW_SMALL_INT((mp_int_t)((_val1 * 5) + (_val2 / 3)));
}

STATIC mp_obj_t dummy_getdata(const mp_ext_table_t *et, mp_obj_t index) {
    mp_uint_t idx = et->mp_obj_get_int(index);

    if (idx < 4) {
        return MP_OBJ_NEW_SMALL_INT(dummy_data[idx]);
    }
    else {
        return MP_OBJ_NEW_SMALL_INT(-1);
    }
}

MP_EXT_HEADER

MP_EXT_INIT
void init(const mp_ext_table_t *et) {
    mp_obj_t f_compute = et->mp_obj_new_fun_extern(false, 2, 2, dummy_compute);
    mp_obj_t f_getdata = et->mp_obj_new_fun_extern(false, 1, 1, dummy_getdata);
    et->mp_store_global(et->qstr_from_str("compute"), f_compute);
    et->mp_store_global(et->qstr_from_str("getdata"), f_getdata);
}
