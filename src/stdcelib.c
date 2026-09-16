#include "stdcelib.h"
#include "stdce.h"
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// TODO: trim only edges
inline void trim(char *str) {
    //
    // str[strcspn(str, "\r\n")] = 0;
}

handle ResultI64 i64_of_str(const char *str) {
    ResultI64 result = DefaultResultI64;

    errno = 0;

    char *endptr;
    i64 value = strtol(str, &endptr, 10);

    try_when(endptr, str, "Failed to convert string to int, no numbers found");
    try_when(
        *endptr != '\0',
        true,
        "Failed to convert string to int, invalid character found"
    );
    try_when(
        errno,
        ERANGE,
        "Failed to convert string to int, overflow or underflow occurred"
    );

    SetOk(result, value);
    return result;
}

handle ResultU64 u64_of_str(const char *str) {
    ResultU64 result = DefaultResultU64;

    errno = 0;

    char *endptr;
    u64 value = strtoul(str, &endptr, 10);

    try_when(endptr, str, "Failed to convert string to uint, no numbers found");
    try_when(
        *endptr != '\0',
        true,
        "Failed to convert string to uint, invalid character found"
    );
    try_when(
        errno,
        ERANGE,
        "Failed to convert string to uint, overflow or underflow occurred"
    );

    SetOk(result, value);
    return result;
}

handle ResultF64 f64_of_str(const char *str) {
    ResultF64 result = DefaultResultF64;

    errno = 0;

    char *endptr;
    f64 value = strtod(str, &endptr);

    try_when(
        endptr, str, "Failed to convert string to double, no numbers found"
    );
    try_when(
        *endptr != '\0',
        true,
        "Failed to convert string to double, invalid character found"
    );
    try_when(
        errno,
        ERANGE,
        "Failed to convert string to double, overflow or underflow occurred"
    );

    SetOk(result, value);
    return result;
}

handle ResultHeapString str_of_i64(i64 value) {
    ResultHeapString result = DefaultResultHeapString;

    char *string;
    try_when(asprintf(&string, "%li", value), -1, "Allocated sprintf failed");

    SetOk(result, string);
    return result;
}
handle ResultHeapString str_of_u64(u64 value) {
    ResultHeapString result = DefaultResultHeapString;

    char *string;
    try_when(asprintf(&string, "%lu", value), -1, "Allocated sprintf failed");

    SetOk(result, string);
    return result;
}
handle ResultHeapString str_of_f64(f64 value) {
    ResultHeapString result = DefaultResultHeapString;

    char *string;
    try_when(asprintf(&string, "%fl", value), -1, "Allocated sprintf failed");

    SetOk(result, string);
    return result;
}
