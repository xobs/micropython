#include "mpextern.h"

#if MICROPY_MODULE_EXTERN

typedef mp_obj_t (*mp_fun_ext_0_t)(const mp_ext_table_t *et);
typedef mp_obj_t (*mp_fun_ext_1_t)(const mp_ext_table_t *et, mp_obj_t);
typedef mp_obj_t (*mp_fun_ext_2_t)(const mp_ext_table_t *et, mp_obj_t, mp_obj_t);
typedef mp_obj_t (*mp_fun_ext_3_t)(const mp_ext_table_t *et, mp_obj_t, mp_obj_t, mp_obj_t);
typedef mp_obj_t (*mp_fun_ext_var_t)(const mp_ext_table_t *et, mp_uint_t n, const mp_obj_t *);
typedef mp_obj_t (*mp_fun_ext_kw_t)(const mp_ext_table_t *et, mp_uint_t n, const mp_obj_t *, mp_map_t *);

STATIC const mp_obj_type_t mp_type_fun_extern;
STATIC const mp_ext_table_t mp_ext_table;

typedef struct _mp_obj_fun_extern_t { // use this to make const objects that go in ROM
    mp_obj_base_t base;
    bool is_kw : 1;
    mp_uint_t n_args_min : 15; // inclusive
    mp_uint_t n_args_max : 16; // inclusive
    void *fun; // must be a pointer to a callable function in ROM
    mp_obj_dict_t *dict_globals;
} mp_obj_fun_extern_t;

STATIC mp_obj_t fun_extern_call(mp_obj_t self_in, mp_uint_t n_args, mp_uint_t n_kw, const mp_obj_t *args) {
    assert(MP_OBJ_IS_TYPE(self_in, &mp_type_fun_extern));
    mp_obj_fun_extern_t *self = self_in;

    // check number of arguments
    mp_arg_check_num(n_args, n_kw, self->n_args_min, self->n_args_max, self->is_kw);

    if (self->is_kw) {
        // function allows keywords

        // we create a map directly from the given args array
        mp_map_t kw_args;
        mp_map_init_fixed_table(&kw_args, n_kw, args + n_args);

        return ((mp_fun_ext_kw_t)self->fun)(&mp_ext_table, n_args, args, &kw_args);

    } else if (self->n_args_min <= 3 && self->n_args_min == self->n_args_max) {
        // function requires a fixed number of arguments

        // dispatch function call
        switch (self->n_args_min) {
            case 0:
                return ((mp_fun_ext_0_t)self->fun)(&mp_ext_table);

            case 1:
                return ((mp_fun_ext_1_t)self->fun)(&mp_ext_table, args[0]);

            case 2:
                return ((mp_fun_ext_2_t)self->fun)(&mp_ext_table, args[0], args[1]);

            case 3:
            default:
                return ((mp_fun_ext_3_t)self->fun)(&mp_ext_table, args[0], args[1], args[2]);
        }

    } else {
        // function takes a variable number of arguments, but no keywords

        return ((mp_fun_ext_var_t)self->fun)(&mp_ext_table, n_args, args);
    }
}

STATIC const mp_obj_type_t mp_type_fun_extern = {
    { &mp_type_type },
    .name = MP_QSTR_function,
    .call = fun_extern_call,
};

STATIC mp_obj_t mp_obj_new_fun_extern(bool is_kw, mp_uint_t n_args_min, mp_uint_t n_args_max, void *f) {
    mp_obj_fun_extern_t *self = m_new_obj(mp_obj_fun_extern_t);
    self->base.type = &mp_type_fun_extern;
    self->is_kw = is_kw;
    self->n_args_min = n_args_min;
    self->n_args_max = n_args_max;
    self->fun = f;
    self->dict_globals = mp_globals_get();
    return self;
}

STATIC const mp_ext_table_t mp_ext_table = {
    .mp_const_none_ = mp_const_none,
    .mp_const_false_ = mp_const_false,
    .mp_const_true_ = mp_const_true,
    .mp_obj_new_fun_extern = mp_obj_new_fun_extern,
    .qstr_from_str = qstr_from_str,
    .mp_store_global = mp_store_global,
    .mp_obj_new_list = mp_obj_new_list,
    .mp_binary_op = mp_binary_op,
};

void mp_extern_load(const char *ext_name, mp_obj_dict_t *globals) {
    const byte *buf = mp_extern_load_binary(ext_name);

    if (buf == NULL) {
        nlr_raise(mp_obj_new_exception_msg_varg(&mp_type_OSError, "could not load MPY binary"));
    }

    if (!(buf[0] == 'M' && buf[1] == 'P' && buf[2] == 'Y')) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "not a valid MPY binary"));
    }

    if (buf[3] != MP_EXT_VERSION_MAJOR || buf[4] > MP_EXT_VERSION_MINOR) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "MPY binary has wrong version"));
    }

    if (buf[6] != MP_EXT_ARCH_CURRENT) {
        nlr_raise(mp_obj_new_exception_msg(&mp_type_OSError, "MPY binary has wrong arch"));
    }

    // push context
    mp_obj_dict_t *old_locals = mp_locals_get();
    mp_obj_dict_t *old_globals = mp_globals_get();
    mp_locals_set(globals);
    mp_globals_set(globals);

    // call extern init
    mp_obj_t (*f)(const mp_ext_table_t*) = (mp_obj_t(*)(const mp_ext_table_t*))MICROPY_MAKE_POINTER_CALLABLE(buf + 8);
    f(&mp_ext_table);

    // pop context
    mp_globals_set(old_globals);
    mp_locals_set(old_locals);
}

#endif // MICROPY_MODULE_EXTERN
