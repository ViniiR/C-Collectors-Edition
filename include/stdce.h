/*
 * C Collector's Edition Standard
 * License:   MIT
 * Full Text: See LICENSE file
 */
#pragma once

#include <float.h>
#include <limits.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: add comments for macros
// TODO: #define IS_BETWEEN(n,L,H)   ((unsigned char)((n) >= (L) && (n) <= (H)))

// https://github.com/plainC/wondermacros/blob/master/wondermacros/comparison/is_any_of.h
// TODO: isanyof/isany/anyof macro

/// Free value unless it is equal to 'unless'
/// NOTE: only function declaration in here due to nested functions not being
/// caught by the LSP so clangd complains stdcelib isnt used
inline static void free_unless(void *value, const void *unless) {
    if (value != unless) {
        free(value);
    }
}

// Types

typedef ssize_t ssize; // visualize it as isize
typedef size_t usize;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;
// typedef double f128;
_Static_assert(sizeof(f32) * CHAR_BIT == 32, "f32 type is not 32-bits wide");
_Static_assert(sizeof(f64) * CHAR_BIT == 64, "f64 type is not 64-bits wide");
// _Static_assert(sizeof(f128)*CHAR_BIT==128, "f128 type is not 128-bits wide");

// Result types

#define _MAKE_RESULT(_T_, _Name_)                                              \
    typedef struct {                                                           \
        char *err;                                                             \
        _T_ ok;                                                                \
    } Result##_Name_;

_MAKE_RESULT(void *, Void)
_MAKE_RESULT(i8, I8)
_MAKE_RESULT(i16, I16)
_MAKE_RESULT(i32, I32)
_MAKE_RESULT(i64, I64)
_MAKE_RESULT(u8, U8)
_MAKE_RESULT(u16, U16)
_MAKE_RESULT(u32, U32)
_MAKE_RESULT(u64, U64)
_MAKE_RESULT(f32, F32)
_MAKE_RESULT(f64, F64)
_MAKE_RESULT(bool, Bool)
_MAKE_RESULT(char *, HeapString)
_MAKE_RESULT(void *, HeapPointer)

#define _UNKNOWN_RESULT_ERR "Unknown Result Error"
#define _DEFAULT_RESULT_OF(_value_) {.err = _UNKNOWN_RESULT_ERR, .ok = _value_}

#define DefaultResultVoid _DEFAULT_RESULT_OF(nullptr)
#define DefaultResultI8 _DEFAULT_RESULT_OF(0)
#define DefaultResultI16 _DEFAULT_RESULT_OF(0)
#define DefaultResultI32 _DEFAULT_RESULT_OF(0)
#define DefaultResultI64 _DEFAULT_RESULT_OF(0)
#define DefaultResultU8 _DEFAULT_RESULT_OF(0)
#define DefaultResultU16 _DEFAULT_RESULT_OF(0)
#define DefaultResultU32 _DEFAULT_RESULT_OF(0)
#define DefaultResultU64 _DEFAULT_RESULT_OF(0)
#define DefaultResultF32 _DEFAULT_RESULT_OF(0.0)
#define DefaultResultF64 _DEFAULT_RESULT_OF(0.0)
#define DefaultResultBool _DEFAULT_RESULT_OF(false)
#define DefaultResultHeapString _DEFAULT_RESULT_OF("")
#define DefaultResultHeapPointer _DEFAULT_RESULT_OF("")

// Shorthand macros

#define let __extension__ __auto_type

#define handle [[nodiscard]]

#define cast(_value_, _T_) ((_T_)_value_)

#define lengthof(_a_) (sizeof(_a_) / sizeof((_a_)[0]))

#define anyof(_compare_, ...)

/// Semantic switch statement that signals it will only use when macros
/// with when case branch with implicit break statement
/// and default branch with implicit :
#define match switch
#define when(_val_)                                                            \
    break;                                                                     \
    case (_val_):
#define rest                                                                   \
    break;                                                                     \
    default:

// Macros

/// "result" is the literal variable name
#define SetOk(_res_, _ok_)                                                     \
    do {                                                                       \
        (_res_).ok = (_ok_);                                                   \
        (_res_).err = nullptr;                                                 \
    } while (0)

/// "result" is the literal variable name
#define SetErr(_res_, _err_)                                                   \
    do {                                                                       \
        (_res_).err = _err_;                                                   \
    } while (0)

#define IsOk(_res_) ((_res_).err == nullptr)

#define IsErr(_res_) ((_res_).err != nullptr)

/// NOTE: try can only be used when inside a ResultType function
/// Results in '_value_.ok'
/// or
/// Set 'result.err' to '_value_.err' and return
/// "result" is the literal variable name
#define try(_value_) _try_named(_value_, result)
#define _try_named(_value_, _result_)                                          \
    __extension__({                                                            \
        __auto_type __val = (_value_);                                         \
        if ((__val.err != nullptr) && (__val.err[0] != '\0')) {                \
            (_result_).err = __val.err;                                        \
            return (_result_);                                                 \
        }                                                                      \
        ((typeof(__val))__val).ok;                                             \
    })

/// NOTE: try_when can only be used when inside a ResultType function
/// Results in '_value_'
/// or
/// Set 'result.err' to '_errmsg_' and return
/// "result" is the literal variable name
#define try_when(_value_, _errvalue_, _errmsg_)                                \
    __extension__({                                                            \
        __auto_type __val = (_value_);                                         \
        if ((__val) == _errvalue_) {                                           \
            (result).err = _errmsg_;                                           \
            return (result);                                                   \
        }                                                                      \
        ((typeof(__val))__val);                                                \
    })

/// WARNING: do not use free() inside defer unless you check it before,
/// defer will always run at the end of scope.
/// This piece of code is dangerous:
/// ```c
/// char *str = malloc(10);
/// defer {
///     free(str); // use free_unless(str, nullptr)
/// }
/// // Useless check (if done after defer {} usage)
/// if (str == nullptr) {
///     return;
/// }
/// ```
/* ----------------------------------------------------------------------------
 * Third-Party Component: c-defer
 * License:   Unlicense (Public Domain)
 * Full Text: See NOTICES file (Section #2)
 * Source:    https://github.com/cmhood/c-defer
 */
#define defer _defer__2(__COUNTER__)
#define _defer__2(X) _defer__3(X)
#define _defer__3(X) _defer__4(defer__id##X)
#define _defer__4(ID)                                                          \
    __extension__ auto void ID##func(void *);                                  \
    __extension__ __attribute__((cleanup(ID##func))) char ID##var = 0;         \
    __extension__ void ID##func(void *ID##param)
/*  ---------------------------------------------------------------------------
 */

// Loop Macros

#define ever (;;)

#define each(_item_, _array_)                                                  \
    (typeof(*(_array_)) *_p_ = (_array_), (_item_) = *_p_;                     \
     _p_ < &((_array_)[lengthof(_array_)]);                                    \
     _p_++, (_item_) = *_p_)

/// Loop over range inclusive end
/// works from higher to lower number aswell
/* ----------------------------------------------------------------------------
 * Third-Party Component: #macros.h
 * License:   MIT
 * Full Text: See NOTICES file (Section #1)
 * Source:    https://github.com/ramdeoshubham/macros
 */
#define range(_i_, _from_, _to_)                                               \
    (typeof(_from_) _i_ = (_from_);                                            \
     (((_to_) >= (_from_)) ? (_i_ <= (_to_)) : (_i_ >= _to_));                 \
     (((_to_) >= (_from_)) ? ((_i_)++) : ((_i_)--)))
/* ----------------------------------------------------------------------------
 */

// TODO: might cause issues when from is higher than to
/// Loop over range exclusive end
#define rangex(_i_, _from_, _toexclusive_) range(_i_, _from_, _toexclusive_ - 1)

// Debug macros

#ifdef NDEBUG
#define debug(...) #error "Cannot use debug macro in release builds."
#define debugf(...) #error "Cannot use debugf macro in release builds."

#define todo() #error "Cannot use debugf macro in release builds."

#else

#define debug(_str_) printf("%s@%d: " _str_ "\n", __FILE__, __LINE__)
/* ----------------------------------------------------------------------------
 * Third-Party Component: #macros.h
 * License:   MIT
 * Full Text: See NOTICES file (Section #1)
 * Source:    https://github.com/ramdeoshubham/macros
 */
#define debugf(fmt, ...)                                                       \
    fprintf(stderr, "%s@%d: " fmt "\n", __FILE__, __LINE__, __VA_ARGS__)
/* ----------------------------------------------------------------------------
 */

#define todo()                                                                 \
    do {                                                                       \
        fprintf(                                                               \
            stderr,                                                            \
            "%s@%d: Reached todo macro! Forcefully closing the program.\n",    \
            __FILE__,                                                          \
            __LINE__                                                           \
        );                                                                     \
        exit(1);                                                               \
    } while (0)
#endif
