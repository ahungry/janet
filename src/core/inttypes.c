/*
* Copyright (c) 2020 Calvin Rose & contributors
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to
* deal in the Software without restriction, including without limitation the
* rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
* sell copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
* FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
* IN THE SOFTWARE.
*/

#include <errno.h>
#include <stdlib.h>
#include <limits.h>
#include <inttypes.h>
#include <math.h>

#ifndef JANET_AMALG
#include "features.h"
#include <janet.h>
#include "util.h"
#endif

/* Conditional compilation */
#ifdef JANET_INT_TYPES

#define MAX_INT_IN_DBL 9007199254740992ULL /* 2^53 */

static int it_s64_get(void *p, Janet key, Janet *out);
static int it_u64_get(void *p, Janet key, Janet *out);

static int32_t janet_int64_hash(void *p1, size_t size) {
    (void) size;
    int32_t *words = p1;
    return words[0] ^ words[1];
}

static int janet_int64_compare(void *p1, void *p2) {
    int64_t x = *((int64_t *)p1);
    int64_t y = *((int64_t *)p2);
    return x == y ? 0 : x < y ? -1 : 1;
}

static int janet_uint64_compare(void *p1, void *p2) {
    uint64_t x = *((uint64_t *)p1);
    uint64_t y = *((uint64_t *)p2);
    return x == y ? 0 : x < y ? -1 : 1;
}

static void int64_marshal(void *p, JanetMarshalContext *ctx) {
    janet_marshal_abstract(ctx, p);
    janet_marshal_int64(ctx, *((int64_t *)p));
}

static void *int64_unmarshal(JanetMarshalContext *ctx) {
    int64_t *p = janet_unmarshal_abstract(ctx, sizeof(int64_t));
    p[0] = janet_unmarshal_int64(ctx);
    return p;
}

static void it_s64_tostring(void *p, JanetBuffer *buffer) {
    char str[32];
    sprintf(str, "%" PRId64, *((int64_t *)p));
    janet_buffer_push_cstring(buffer, str);
}

static void it_u64_tostring(void *p, JanetBuffer *buffer) {
    char str[32];
    sprintf(str, "%" PRIu64, *((uint64_t *)p));
    janet_buffer_push_cstring(buffer, str);
}

const JanetAbstractType janet_s64_type = {
    "core/s64",
    NULL,
    NULL,
    it_s64_get,
    NULL,
    int64_marshal,
    int64_unmarshal,
    it_s64_tostring,
    janet_int64_compare,
    janet_int64_hash,
    JANET_ATEND_HASH
};

const JanetAbstractType janet_u64_type = {
    "core/u64",
    NULL,
    NULL,
    it_u64_get,
    NULL,
    int64_marshal,
    int64_unmarshal,
    it_u64_tostring,
    janet_uint64_compare,
    janet_int64_hash,
    JANET_ATEND_HASH
};

int64_t janet_unwrap_s64(Janet x) {
    switch (janet_type(x)) {
        default:
            break;
        case JANET_NUMBER : {
            double dbl = janet_unwrap_number(x);
            if (fabs(dbl) <=  MAX_INT_IN_DBL)
                return (int64_t)dbl;
            break;
        }
        case JANET_STRING: {
            int64_t value;
            const uint8_t *str = janet_unwrap_string(x);
            if (janet_scan_int64(str, janet_string_length(str), &value))
                return value;
            break;
        }
        case JANET_ABSTRACT: {
            void *abst = janet_unwrap_abstract(x);
            if (janet_abstract_type(abst) == &janet_s64_type ||
                    (janet_abstract_type(abst) == &janet_u64_type))
                return *(int64_t *)abst;
            break;
        }
    }
    janet_panic("bad s64 initializer");
    return 0;
}

uint64_t janet_unwrap_u64(Janet x) {
    switch (janet_type(x)) {
        default:
            break;
        case JANET_NUMBER : {
            double dbl = janet_unwrap_number(x);
            if ((dbl >= 0) && (dbl <= MAX_INT_IN_DBL))
                return (uint64_t)dbl;
            break;
        }
        case JANET_STRING: {
            uint64_t value;
            const uint8_t *str = janet_unwrap_string(x);
            if (janet_scan_uint64(str, janet_string_length(str), &value))
                return value;
            break;
        }
        case JANET_ABSTRACT: {
            void *abst = janet_unwrap_abstract(x);
            if (janet_abstract_type(abst) == &janet_s64_type ||
                    (janet_abstract_type(abst) == &janet_u64_type))
                return *(uint64_t *)abst;
            break;
        }
    }
    janet_panic("bad u64 initializer");
    return 0;
}

JanetIntType janet_is_int(Janet x) {
    if (!janet_checktype(x, JANET_ABSTRACT)) return JANET_INT_NONE;
    const JanetAbstractType *at = janet_abstract_type(janet_unwrap_abstract(x));
    return (at == &janet_s64_type) ? JANET_INT_S64 :
           ((at == &janet_u64_type) ? JANET_INT_U64 :
            JANET_INT_NONE);
}

Janet janet_wrap_s64(int64_t x) {
    int64_t *box = janet_abstract(&janet_s64_type, sizeof(int64_t));
    *box = (int64_t)x;
    return janet_wrap_abstract(box);
}

Janet janet_wrap_u64(uint64_t x) {
    uint64_t *box = janet_abstract(&janet_u64_type, sizeof(uint64_t));
    *box = (uint64_t)x;
    return janet_wrap_abstract(box);
}

static Janet cfun_it_s64_new(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    return janet_wrap_s64(janet_unwrap_s64(argv[0]));
}

static Janet cfun_it_u64_new(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 1);
    return janet_wrap_u64(janet_unwrap_u64(argv[0]));
}

#define OPMETHOD(T, type, name, oper) \
static Janet cfun_it_##type##_##name(int32_t argc, Janet *argv) { \
    janet_arity(argc, 2, -1); \
    T *box = janet_abstract(&janet_##type##_type, sizeof(T)); \
    *box = janet_unwrap_##type(argv[0]); \
    for (int32_t i = 1; i < argc; i++) \
        *box oper##= janet_unwrap_##type(argv[i]); \
    return janet_wrap_abstract(box); \
} \

#define OPMETHODINVERT(T, type, name, oper) \
static Janet cfun_it_##type##_##name(int32_t argc, Janet *argv) { \
    janet_fixarity(argc, 2); \
    T *box = janet_abstract(&janet_##type##_type, sizeof(T)); \
    *box = janet_unwrap_##type(argv[1]); \
    *box oper##= janet_unwrap_##type(argv[0]); \
    return janet_wrap_abstract(box); \
} \

#define DIVMETHOD(T, type, name, oper) \
static Janet cfun_it_##type##_##name(int32_t argc, Janet *argv) { \
    janet_arity(argc, 2, -1);                       \
    T *box = janet_abstract(&janet_##type##_type, sizeof(T)); \
    *box = janet_unwrap_##type(argv[0]); \
    for (int32_t i = 1; i < argc; i++) { \
      T value = janet_unwrap_##type(argv[i]); \
      if (value == 0) janet_panic("division by zero"); \
      *box oper##= value; \
    } \
    return janet_wrap_abstract(box); \
} \

#define DIVMETHODINVERT(T, type, name, oper) \
static Janet cfun_it_##type##_##name(int32_t argc, Janet *argv) { \
    janet_fixarity(argc, 2);                       \
    T *box = janet_abstract(&janet_##type##_type, sizeof(T)); \
    *box = janet_unwrap_##type(argv[1]); \
    T value = janet_unwrap_##type(argv[0]); \
    if (value == 0) janet_panic("division by zero"); \
    *box oper##= value; \
    return janet_wrap_abstract(box); \
} \

#define DIVMETHOD_SIGNED(T, type, name, oper) \
static Janet cfun_it_##type##_##name(int32_t argc, Janet *argv) { \
    janet_arity(argc, 2, -1);                       \
    T *box = janet_abstract(&janet_##type##_type, sizeof(T)); \
    *box = janet_unwrap_##type(argv[0]); \
    for (int32_t i = 1; i < argc; i++) { \
      T value = janet_unwrap_##type(argv[i]); \
      if (value == 0) janet_panic("division by zero"); \
      if ((value == -1) && (*box == INT64_MIN)) janet_panic("INT64_MIN divided by -1"); \
      *box oper##= value; \
    } \
    return janet_wrap_abstract(box); \
} \

#define DIVMETHODINVERT_SIGNED(T, type, name, oper) \
static Janet cfun_it_##type##_##name(int32_t argc, Janet *argv) { \
    janet_fixarity(argc, 2);                       \
    T *box = janet_abstract(&janet_##type##_type, sizeof(T)); \
    *box = janet_unwrap_##type(argv[1]); \
    T value = janet_unwrap_##type(argv[0]); \
    if (value == 0) janet_panic("division by zero"); \
    if ((value == -1) && (*box == INT64_MIN)) janet_panic("INT64_MIN divided by -1"); \
    *box oper##= value; \
    return janet_wrap_abstract(box); \
} \

#define COMPMETHOD(T, type, name, oper) \
static Janet cfun_it_##type##_##name(int32_t argc, Janet *argv) { \
    janet_fixarity(argc, 2); \
    T v1 = janet_unwrap_##type(argv[0]); \
    T v2 = janet_unwrap_##type(argv[1]); \
    return janet_wrap_boolean(v1 oper v2); \
}

static Janet cfun_it_s64_mod(int32_t argc, Janet *argv) {
    janet_arity(argc, 2, -1);
    int64_t *box = janet_abstract(&janet_s64_type, sizeof(int64_t));
    *box = janet_unwrap_s64(argv[0]);
    for (int32_t i = 1; i < argc; i++) {
        int64_t value = janet_unwrap_s64(argv[i]);
        if (value == 0) janet_panic("division by zero");
        int64_t x = *box % value;
        if (x < 0) {
            x = (*box < 0) ? x - *box : x + *box;
        }
        *box = x;
    }
    return janet_wrap_abstract(box);
}

static Janet cfun_it_s64_modi(int32_t argc, Janet *argv) {
    janet_fixarity(argc, 2);
    int64_t *box = janet_abstract(&janet_s64_type, sizeof(int64_t));
    int64_t op1 = janet_unwrap_s64(argv[0]);
    int64_t op2 = janet_unwrap_s64(argv[1]);
    int64_t x = op1 % op2;
    if (x < 0) {
        x = (op1 < 0) ? x - op1 : x + op1;
    }
    *box = x;
    return janet_wrap_abstract(box);
}

OPMETHOD(int64_t, s64, add, +)
OPMETHOD(int64_t, s64, sub, -)
OPMETHODINVERT(int64_t, s64, subi, -)
OPMETHOD(int64_t, s64, mul, *)
DIVMETHOD_SIGNED(int64_t, s64, div, /)
DIVMETHOD_SIGNED(int64_t, s64, rem, %)
DIVMETHODINVERT_SIGNED(int64_t, s64, divi, /)
DIVMETHODINVERT_SIGNED(int64_t, s64, remi, %)
OPMETHOD(int64_t, s64, and, &)
OPMETHOD(int64_t, s64, or, |)
OPMETHOD(int64_t, s64, xor, ^)
OPMETHOD(int64_t, s64, lshift, <<)
OPMETHOD(int64_t, s64, rshift, >>)
COMPMETHOD(int64_t, s64, lt, <)
COMPMETHOD(int64_t, s64, gt, >)
COMPMETHOD(int64_t, s64, le, <=)
COMPMETHOD(int64_t, s64, ge, >=)
COMPMETHOD(int64_t, s64, eq, ==)
COMPMETHOD(int64_t, s64, ne, !=)

OPMETHOD(uint64_t, u64, add, +)
OPMETHOD(uint64_t, u64, sub, -)
OPMETHODINVERT(uint64_t, u64, subi, -)
OPMETHOD(uint64_t, u64, mul, *)
DIVMETHOD(uint64_t, u64, div, /)
DIVMETHOD(uint64_t, u64, mod, %)
DIVMETHODINVERT(uint64_t, u64, divi, /)
DIVMETHODINVERT(uint64_t, u64, modi, %)
OPMETHOD(uint64_t, u64, and, &)
OPMETHOD(uint64_t, u64, or, |)
OPMETHOD(uint64_t, u64, xor, ^)
OPMETHOD(uint64_t, u64, lshift, <<)
OPMETHOD(uint64_t, u64, rshift, >>)
COMPMETHOD(uint64_t, u64, lt, <)
COMPMETHOD(uint64_t, u64, gt, >)
COMPMETHOD(uint64_t, u64, le, <=)
COMPMETHOD(uint64_t, u64, ge, >=)
COMPMETHOD(uint64_t, u64, eq, ==)
COMPMETHOD(uint64_t, u64, ne, !=)

#undef OPMETHOD
#undef DIVMETHOD
#undef DIVMETHOD_SIGNED
#undef COMPMETHOD

static JanetMethod it_s64_methods[] = {
    {"+", cfun_it_s64_add},
    {"r+", cfun_it_s64_add},
    {"-", cfun_it_s64_sub},
    {"r-", cfun_it_s64_subi},
    {"*", cfun_it_s64_mul},
    {"r*", cfun_it_s64_mul},
    {"/", cfun_it_s64_div},
    {"r/", cfun_it_s64_divi},
    {"mod", cfun_it_s64_mod},
    {"rmod", cfun_it_s64_modi},
    {"%", cfun_it_s64_rem},
    {"r%", cfun_it_s64_remi},
    {"<", cfun_it_s64_lt},
    {">", cfun_it_s64_gt},
    {"<=", cfun_it_s64_le},
    {">=", cfun_it_s64_ge},
    {"=", cfun_it_s64_eq},
    {"!=", cfun_it_s64_ne},
    {"&", cfun_it_s64_and},
    {"r&", cfun_it_s64_and},
    {"|", cfun_it_s64_or},
    {"r|", cfun_it_s64_or},
    {"^", cfun_it_s64_xor},
    {"r^", cfun_it_s64_xor},
    {"<<", cfun_it_s64_lshift},
    {">>", cfun_it_s64_rshift},

    {NULL, NULL}
};

static JanetMethod it_u64_methods[] = {
    {"+", cfun_it_u64_add},
    {"r+", cfun_it_u64_add},
    {"-", cfun_it_u64_sub},
    {"r-", cfun_it_u64_subi},
    {"*", cfun_it_u64_mul},
    {"r*", cfun_it_u64_mul},
    {"/", cfun_it_u64_div},
    {"r/", cfun_it_u64_divi},
    {"mod", cfun_it_u64_mod},
    {"rmod", cfun_it_u64_modi},
    {"%", cfun_it_u64_mod},
    {"r%", cfun_it_u64_modi},
    {"<", cfun_it_u64_lt},
    {">", cfun_it_u64_gt},
    {"<=", cfun_it_u64_le},
    {">=", cfun_it_u64_ge},
    {"=", cfun_it_u64_eq},
    {"!=", cfun_it_u64_ne},
    {"&", cfun_it_u64_and},
    {"r&", cfun_it_u64_and},
    {"|", cfun_it_u64_or},
    {"r|", cfun_it_u64_or},
    {"^", cfun_it_u64_xor},
    {"r^", cfun_it_u64_xor},
    {"<<", cfun_it_u64_lshift},
    {">>", cfun_it_u64_rshift},

    {NULL, NULL}
};

static int it_s64_get(void *p, Janet key, Janet *out) {
    (void) p;
    if (!janet_checktype(key, JANET_KEYWORD))
        return 0;
    return janet_getmethod(janet_unwrap_keyword(key), it_s64_methods, out);
}

static int it_u64_get(void *p, Janet key, Janet *out) {
    (void) p;
    if (!janet_checktype(key, JANET_KEYWORD))
        return 0;
    return janet_getmethod(janet_unwrap_keyword(key), it_u64_methods, out);
}

static const JanetReg it_cfuns[] = {
    {
        "int/s64", cfun_it_s64_new,
        JDOC("(int/s64 value)\n\n"
             "Create a boxed signed 64 bit integer from a string value.")
    },
    {
        "int/u64", cfun_it_u64_new,
        JDOC("(int/u64 value)\n\n"
             "Create a boxed unsigned 64 bit integer from a string value.")
    },
    {NULL, NULL, NULL}
};

/* Module entry point */
void janet_lib_inttypes(JanetTable *env) {
    janet_core_cfuns(env, NULL, it_cfuns);
    janet_register_abstract_type(&janet_s64_type);
    janet_register_abstract_type(&janet_u64_type);
}

#endif
