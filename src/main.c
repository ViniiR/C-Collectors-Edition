#include "stdce.h"
#include "stdce.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

[[nodiscard]]
int c_may_fail() {
    char *str = malloc(20);
    if (str == nullptr) {
        return -1; // Must guess error from obscure non 0 values or using
                   // 'errno'
    }
    snprintf(str, 20, "String!");

    int len = strlen(str);
    free(str);

    return len;
}

handle ResultI32 may_fail() {
    ResultI32 result = DefaultResultI32;

    char *str = try_when(malloc(20), nullptr, "Malloc failed");
    snprintf(str, 20, "String!");
    defer { free_unless(str, nullptr); }

    SetOk(result, strlen(str));
    return result;
}

// Nicer [[nodiscard]] macro
handle ResultI32 result_example() {
    ResultI32 result = DefaultResultI32;

    // Unwrap .ok or return .err
    let val = try(may_fail());

    // Macro to conveniently set .ok and remove .err
    SetOk(result, val);
    return result;
}

i32 main() {
    // Infer variable types (not arrays)
    let var_int = 10;
    printf("%i", var_int);

    // Nicer cast syntax
    let var_float = cast(var_int, f64);

    // Defer
    defer { // Run at end of scope
        printf(
            "I'll actually never run since there's a todo macro down below!"
        );
    }

    i32 array[] = {10, 11, 12};
    printf("Length: %zu", lengthof(array)); // 3

    // Loops
    for each(number, array) {
        printf("%i", number); // 10, 11, 12
    }
    for range(i, 0, 100) {
        printf("%i", i); // 0..100
    }
    for rangex(i, 0, 100) {
        printf("%i", i); // 0..99
    }
    for ever {
        printf("i'm just a prettier (;;)");
        break; // Brakes to the rescue
    }

    match(array[0]) {
        when(10) {
            printf("LOOK, NO BREAKS");
        }
        when(9) {
            printf("impossible!"); 
        }
        rest {
            printf("Nicer default branch, no ugly ':'s LIKE PYTHON");
        }
    }

    // Debug
    // Will error out on release build
    // Crashes with file and line information
    todo();
    // Prints string with file and line information
    debug("Hi dbg");
    // Same as debug but with format strings
    debugf("Hi dbg %s is %i!", "format", 10);

    return 0;
}
