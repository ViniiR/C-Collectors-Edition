# C Collector's Edition Standard

Collection of macros and functions to improve C's workflow and more importantly, **looks**

## The first C header file with a cool name (and maybe logo)!
Originally called **Elegant C**, which may help you make sense of some of its features.

## Features
<ul>
    <li><code>let</code> macro to avoid type definitions</li>
    <li><code>handle</code> macro for a more idiomatic [[nodiscard]] attribute</li>
    <li><code>cast</code> macro for a more idiomatic <code>((Type) val)</code> cast</li>
    <li>ssize and usize type aliases</li>
    <li>i8-i64, u8-u64, f32-f64 type aliases</li>
    <!-- Result Types -->
    <li>Result types with <code>.ok</code> and <code>.err</code>, should also be used as an Option type</li>
    <li>Easy DefaultResult values for pre implemented types</li>
    <li><code>SetOk</code> and <code>SetErr</code> macros for easy Result management</li>
    <li><code>IsOk</code> and <code>IsErr</code> macros for easy Result testing</li>
    <li><code>try</code> macro for error propagation or value unwraping</li>
    <li>
        <code>try_when</code> macro is similar to try but may be used for stdlib fallible functions, receiving: (value,
        fail_if_value, fail_message)
    </li>
    <!-- Switches -->
    <li>
        <code>match</code>, <code>when</code>, and <code>rest</code> macros for a nicer switch, case, and default syntax
    </li>
    <!-- Memory Allocation -->
    <li>
        <code>defer</code> macro runs a nested function at the end of scope, useful for allocation cleanup or file
        closing
    </li>
    <li>
        <code>free_unless</code> wrapper function, use it inside a defer to only free a value if it is not the
        '_unless_' argument, since defers will run before any return statement
    </li>
    <!-- Arrays -->
    <li><code>lengthof</code> macro to get an array's length</li>
    <li>for <code>each</code> macro for easy iteration over each element of an array</li>
    <li>for <code>range</code> and for <code>rangex</code> macros for a loop in a range, inclusive or exclusive</li>
    <li>for <code>ever</code> macro for an idiomatic infinite loop</li>
    <!-- Debug -->
    <li><code>debugf</code> and <code>debug</code> macros to print formatted line and file@line information</li>
    <li><code>todo</code> macro to crash the program with line information</li>
</ul>

## Why?
You might ask: "Why? C is already great."
Yes, C is great, if it wasn't, I would've **tried** making a new language

## However
Although as of 2026 i believe this is safe(due to a gcc nested function safety fix) you're better off using zig.
C++ also has many of the features here, with the exception of the try macros, you could get everything here in C++
without a compiler extension, there is a very nice implementation of defer in this
<a href="https://gist.github.com/p2004a/045726d70a490d12ad62">gist</a>.

## GCC Compiler flags
To fully use this library you must enable GCC compiler extensions with ```-std=gnu23```.
I also recommend enabling ```-Wall -Wextra -Wpedantic``` and ```-Werror```

## Clangd LSP
if you're using clangd as your lsp, you might notice it really dislikes GCC's extensions,
to suppress them, put this in your ```<project_root>/.clangd```
    ```yml
    Diagnostics:
    Suppress:
    - "typecheck_sclass_func"
    - "attribute_cleanup_arg_not_function"
    - "function_definition_not_allowed"
    ```

    ## Clang Format
    Clang format will hate our for macros so you might need this in your ```<project_root>/.clang-format```.
        ```yml
        ForEachMacros:
        - "for ever"
        - "for each"
        - "for range"
        - "for rangex"

        SpaceBeforeParens: Custom
        SpaceBeforeParensOptions:
        AfterForeachMacros: false

        StatementMacros:
        - "for ever"
        - "for each"
        - "for range"
        - "for rangex"

        Macros:
        - ever=(;;)
        - each(i, a, b)=(int i = a; i < b; i++) - range(i, a, b)=(int i=a; i < b; i++) - rangex(i, a, b)=(int i=a; i <
            b; i++) ``` ## Treesitter highlight To pretend these macros are keywords, put this on your editor's C
            highlight .scm file. For neovim it's ```~/.config/nvim/queries/c/highlights.scm```; ```scm ; extends ;
            Keywords ((type_identifier) @keyword . (compound_statement) (#any-of? @keyword "defer" )) ((identifier)
            @keyword (#any-of? @keyword "each" "range" "rangex" "ever" "rest" )) (call_expression function: (identifier)
            @keyword (#any-of? @keyword "lengthof" "match" "when" )) ((type_identifier) @keyword (#any-of?
            @keyword "let" "match" )) (call_expression function: (identifier) @keyword (#any-of?
            @keyword "try" "try_when" )) (call_expression function: (identifier) @keyword (#eq? @keyword "cast" )
            arguments: (argument_list (_) (identifier) @type)) (function_definition type: (type_identifier) @keyword
            (#eq? @keyword "handle" ) (ERROR (identifier) @type)) ((type_identifier) @keyword (#eq? @keyword "handle" ))
            ; Builtins ((identifier) @constant.builtin (#lua-match? @constant.builtin "^DefaultResult.*" ))
            (call_expression function: (identifier) @constant.builtin (#any-of?
            @constant.builtin "SetOk" "SetErr" "IsOk" "IsErr" )) ; Special (call_expression function: (identifier)
            @punctuation.special (#any-of? @punctuation.special "debugf" "debug" "todo" ) arguments: (argument_list))
            ```
