#pragma once

#include "stdce.h"

// maybe it's not necessary
// ResultVoid printlnf(const char *__restrict format, ...);
// ResultVoid eprintlnf(const char *__restrict format, ...);

handle ResultHeapString str_of_i64(i64 value);
handle ResultHeapString str_of_u64(u64 value);
handle ResultHeapString str_of_f64(f64 value);

ResultI64 i64_of_str(const char *str);
ResultU64 u64_of_str(const char *str);
ResultF64 f64_of_str(const char *str);

inline void trim(char *str);

// exec command
//
// read file
// write file
