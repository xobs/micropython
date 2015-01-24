#include "py/mpextern.h"

STATIC mp_obj_t modx_add1(const mp_ext_table_t *et, mp_obj_t x) {
    return et->mp_binary_op(MP_BINARY_OP_ADD, x, MP_OBJ_NEW_SMALL_INT(1));
}

MP_EXT_HEADER

MP_EXT_INIT
void init(const mp_ext_table_t *et) {
    mp_obj_t f_add1 = et->mp_obj_new_fun_extern(false, 1, 1, modx_add1);
    mp_obj_t list[6] = {MP_OBJ_NEW_SMALL_INT(1), MP_OBJ_NEW_SMALL_INT(2), MP_OBJ_NEW_SMALL_INT(3), et->mp_const_true_, MP_OBJ_NEW_QSTR(et->qstr_from_str("modx")), f_add1};
    et->mp_store_global(et->qstr_from_str("data"), et->mp_obj_new_list(6, list));
    et->mp_store_global(et->qstr_from_str("add1"), f_add1);
}
