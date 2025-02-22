
<div style="justify-items: center">
<img style="display: block" src="exceptions4c-lite.svg">
</div>

# Introduction

Bring the power of exceptions to your C applications with this header-only library!

![][EXAMPLE]

> [!NOTE]
> This library provides you with a set of macros and functions that map the exception handling semantics you are
> probably already used to.


# Getting Started

## Adding Exceptions to Your Project

This library consists of one header file only. All you need to do is copy `exceptions4c-lite.h` into your project,
include it, and define a global variable `exceptions4c`.

```c
#include <exceptions4c-lite.h>

struct e4c_context exceptions4c = {0};
```

Since it's a header-only library, there is no library code to link against.

> [!NOTE]
> This is the lightweight version of **exceptions4c**. You may also want to try the full version of the library on
> [GitHub][EXCEPTIONS4C].

## Defining Exception Types

Create meaningful exceptions that reflect problematic situations in the program.

@snippet pet-store.c exception_types

An exception type is tied to its default error message.


# Usage

Exception handling lets a program deal with errors without crashing. When something goes wrong, the program pauses its
normal flow, jumps to code that handles the issue, and then either recovers or exits cleanly.

This library provides the following macros that are used to handle exceptions:

- #THROW
- #TRY
- #CATCH
- #FINALLY

## Throwing Exceptions

When we #THROW an exception, the flow of the program moves to the appropriate #CATCH block. If the exception is not
handled by any of the blocks in the current function, it propagates up the call stack to the function that called the
current function. This continues until the top level of the program is reached. If no block handles the exception, the
program terminates and an error message is printed to the console.

### Throwing Exceptions with a Custom Message

Use #THROW to trigger an exception when something goes wrong.

@snippet pet-store.c throw

### Throwing Exceptions with a Default Message

If you don't provide an error message, the default one for that exception type will be used.

@snippet pet-store.c throw-default

### Throwing Exceptions with a Formatted Message

Use #THROWF to trigger an exception with a formatted message, just as you would with `printf`. 

@snippet pet-store.c throwf

## Trying Risky Code

Use a #TRY block to wrap code that might cause an exception.

@snippet pet-store.c try

These code blocks, by themselves, don't do anything special. But they allow the introduction of other blocks that do
serve specific purposes.

> [!TIP]
> A single #TRY block must be followed by one or more #CATCH blocks to handle the errors, and an optional #FINALLY block
> to execute cleanup code.

> [!CAUTION]
> Never exit these blocks using `goto`, `break`, `continue`, or `return`.

## Catching Exceptions

To prevent the program from crashing, exceptions need to be handled properly in designated sections of the code.

### Handling Specific Types of Exceptions

Use a #CATCH block to handle a specific type of exceptions when they occur.

@snippet pet-store.c catch

One or more #CATCH blocks can follow a #TRY block. Each #CATCH block must specify the type of exception it handles. If
its type doesn't match the thrown exception, then that block is ignored, and the exception may be caught by the
following blocks.

### Handling All Kinds of Exceptions

On the other hand, the #CATCH_ALL block is a special block that can handle all types of exceptions.

@snippet pet-store.c catch_all

Only one #CATCH_ALL block is allowed per #TRY block, and it must appear after all type-specific #CATCH blocks if any are
present.

### Retrieving the Current Exception

Use #EXCEPTION to retrieve the exception currently being handled.

@snippet pet-store.c catch_all_get_exception

> [!TIP]
> This allows for inspection and further handling of the error, based on both its type and the detailed context of the
> situation.

## Ensuring Cleanup

A #FINALLY block always runs, no matter whether an exception happens or not.

@snippet pet-store.c finally

This block is optional. And, for each #TRY block, there can be only one #FINALLY block. If an exception occurs, the
#FINALLY block is executed after the #CATCH or block that can handle it. Otherwise, it is executed after the #TRY block.

> [!TIP]
> Use #EXCEPTION_IS_UNCAUGHT to determine whether the thrown exception hasn't been handled yet.


# Customization

You can define these macros to customize exceptions4c-lite:

- [NDEBUG](#e4c_exception.file): Determines whether exceptions should retain file/line information for debugging
  purposes.
  - By default, `NDEBUG` is not defined and the debugging info is enabled.
- #EXCEPTIONS4C_MAX_LENGTH: Determines the maximum length of an [exception message](#e4c_exception.message).
  - By default, messages are arrays of 256 characters.
- #EXCEPTIONS4C_MAX_BLOCKS: Determines the maximum number of #TRY blocks that can be nested.
  - By default, up to 32 #TRY blocks can be nested in the program.
- #EXCEPTIONS4C_PANIC: Determines what needs to be done in the event of too many nested #TRY blocks.
  - By default, an error message is printed and then the program is aborted.
- #EXCEPTIONS4C_TERMINATE: Determines what needs to be done in the event of an uncaught exception.
  - By default, the uncaught exception is printed and then the program terminates with a failure value.
- #THROWF: Define this macro at compile time for legacy ANSI C compilers.
  - By default, [C99]'s [variadic macros][VARIADIC_MACROS] and `snprintf` are used.


# Additional Info

## Compatibility

This library should compile in any ANSI C compiler.

> [!TIP]
> For a fully-featured exception handling library, consider using the complete version [exceptions4c][EXCEPTIONS4C],
> which is also Open Source. And if you're looking for a cleaner, safer, and more modern approach to error handling that
> doesn't involve throwing and catching exceptions, you may want to take a look at [Result Library][RESULT_LIBRARY].

## Caveat

Exception handling is based on standard C library functions [`setjmp`][SETJMP] to save the current execution context
and [`longjmp`][LONGJMP] to restore it. According to the documentation:

> Upon return to the scope of [`setjmp`][SETJMP]:
> - all accessible objects, floating-point status flags, and other components of the abstract machine have the same
>   values as they had when [`longjmp`][LONGJMP] was executed,
> - except for the non-[volatile][VOLATILE] local variables in the function containing the invocation of
>   [`setjmp`][SETJMP], whose values are indeterminate if they have been changed since the [`setjmp`][SETJMP]
>   invocation.

Since each #TRY block invokes [`setjmp`][SETJMP], modified local variables in scope must be [`volatile`][VOLATILE].

## Releases

This library adheres to [Semantic Versioning][SEMVER]. All notable changes for each version are documented in a
[change log][CHANGELOG].

Head over to GitHub for the [latest release][LATEST_RELEASE].

[![Latest Release][BADGE_LATEST_RELEASE]][LATEST_RELEASE]

## Source Code

The source code is [available on GitHub][SOURCE_CODE].

[![Fork me on GitHub][BADGE_GITHUB]][SOURCE_CODE]


[BADGE_GITHUB]:                 https://img.shields.io/badge/Fork_me_on_GitHub-black?logo=github
[BADGE_LATEST_RELEASE]:         https://img.shields.io/github/v/release/guillermocalvo/exceptions4c-lite
[C99]:                          https://en.wikipedia.org/wiki/C99
[CHANGELOG]:                    https://github.com/guillermocalvo/exceptions4c-lite/blob/main/CHANGELOG.md
[EXAMPLE]:                      example.png
[EXCEPTIONS4C]:                 https://github.com/guillermocalvo/exceptions4c/
[LATEST_RELEASE]:               https://github.com/guillermocalvo/exceptions4c-lite/releases/latest
[LONGJMP]:                      https://devdocs.io/c/program/longjmp
[RESULT_LIBRARY]:               https://result.guillermo.dev/
[SEMVER]:                       https://semver.org/
[SETJMP]:                       https://devdocs.io/c/program/setjmp
[SOURCE_CODE]:                  https://github.com/guillermocalvo/exceptions4c-lite/
[VARIADIC_MACROS]:              https://en.wikipedia.org/wiki/Variadic_macro_in_the_C_preprocessor
[VOLATILE]:                     https://devdocs.io/c/language/volatile
