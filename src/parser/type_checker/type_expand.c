#include <parser/type_checker/type.h>

#include <debug/error_handler.h>

size_t type_checker_type_sizes[9] = {
    [BTT_VOID] = 0,

    [BTT_BOOL] = 8,

    [BTT_CHAR] = 1,
    [BTT_SHORT] = 2,
    [BTT_INT] = 3,
    [BTT_LONG] = 4,
    [BTT_LONG_LONG] = 5,

    [BTT_FLOAT] = 6,
    [BTT_DOUBLE] = 7,
};

type_checker_type_t * type_checker_type_expand(type_checker_type_t * a, type_checker_type_t * b) {
    if (a == NULL) return NULL;
    if (b == NULL) return NULL;

    type_checker_type_t * _a;
    type_checker_type_t * _b;

    if (!a->is_base) {
        if (a->derived_type.type == DTT_QUALIFIED) {
            _a = a->derived_type.qualified.subtype;
            if (!_a->is_base) return NULL;
        }
        else return NULL;
    }
    else _a = a;

    if (!b->is_base) {
        if (b->derived_type.type == DTT_QUALIFIED) {
            _b = b->derived_type.qualified.subtype;
            if (!_b->is_base) return NULL;
        }
        else return NULL;
    }
    else _b = b;

    size_t a_size = type_checker_type_sizes[_a->base_type.type];
    size_t b_size = type_checker_type_sizes[_b->base_type.type];

    if (a_size > b_size) return _a;
    else return _b;
}