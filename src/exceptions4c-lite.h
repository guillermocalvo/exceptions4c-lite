/*
 * Copyright 2025 Guillermo Calvo
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef EXCEPTIONS4C_LITE

/**
 * Returns the major version number of this library.
 */
#define EXCEPTIONS4C_LITE 0

#include <setjmp.h> /* longjmp, setjmp */
#include <stdio.h> /* fflush, fprintf, snprintf, sprintf, stderr */
#include <stdlib.h> /* EXIT_FAILURE, abort, exit */

#ifndef EXCEPTIONS4C_MAX_BLOCKS

/**
 * Determines the maximum number of #TRY blocks that can be nested.
 *
 * The total number of blocks are preallocated inside the
 * [global variable](#exceptions4c) that contains the current status of
 * exceptions.
 *
 * @note
 * You MAY define this macro with a different value.
 */
#define EXCEPTIONS4C_MAX_BLOCKS 32

#endif

#ifndef EXCEPTIONS4C_MAX_LENGTH

/**
 * Determines the maximum length of an exception message, in characters.
 *
 * The total number of characters are preallocated inside each
 * [exception object](#e4c_exception.message).
 *
 * @note
 * You MAY define this macro with a different value.
 */
#define EXCEPTIONS4C_MAX_LENGTH 256

#endif

#ifndef EXCEPTIONS4C_PANIC

#ifndef NDEBUG

/**
 * Determines what needs to be done in the event of too many nested TRY blocks.
 *
 * @note
 * You MAY define this macro with a different value.
 */
#define EXCEPTIONS4C_PANIC                                                  \
  (void) fprintf(stderr, "\n[exceptions4c-lite]: "                          \
      "Too many TRY blocks nested.\n    at %s:%d\n", __FILE__, __LINE__),   \
  (void) fflush(stderr),                                                    \
  abort()

#else

/**
 * Determines what needs to be done in the event of too many nested TRY blocks.
 *
 * @note
 * You MAY define this macro with a different value.
 */
#define EXCEPTIONS4C_PANIC                                                  \
  (void) fprintf(stderr, "\n[exceptions4c-lite]: "                          \
      "Too many TRY blocks nested.\n"),                                     \
  (void) fflush(stderr),                                                    \
  abort()

#endif

#endif


#ifndef EXCEPTIONS4C_TERMINATE

/**
 * Determines what needs to be done in the event of an uncaught exception.
 *
 * @note
 * You MAY define this macro with a different value.
 */
#define EXCEPTIONS4C_TERMINATE                                              \
  (void) EXCEPTION_PRINT,                                                   \
  (void) fflush(stderr),                                                    \
  exit(EXIT_FAILURE)

#endif

/**
 * Represents a category of problematic situations in a program.
 *
 * Defines a kind of error or exceptional condition that a program might want to
 * #THROW and #CATCH. It serves as a way to group related issues that share
 * common characteristics.
 *
 * Exception types SHOULD be defined as <tt>const</tt>.
 *
 * ```c
 * const e4c_exception_type IO_ERROR = "I/O Error";
 * ```
 *
 * @see THROW
 * @see CATCH
 */
typedef const char * e4c_exception_type;

/**
 * Represents a specific occurrence of an exceptional situation in a program.
 *
 * #e4c_exception combines an [exception type](#e4c_exception_type) with a
 * detailed error message that provides specific information about what went
 * wrong in this particular instance.
 *
 * After an exception is [thrown](#THROW), it may propagate through the program
 * and be caught by an appropriate #CATCH or #CATCH_ALL block.
 *
 * @remark
 * When an exception is caught, #EXCEPTION can be used to retrieve the exception
 * currently being handled. This allows for inspection and further handling of
 * the error, based on both its type and the detailed context of the situation.
 *
 * @see EXCEPTION
 */
struct e4c_exception {

    /** The general category of the error. */
    e4c_exception_type type;

    /** The name of the exception type. */
    const char * name;

    /** A text message describing the specific problem. */
    char message[EXCEPTIONS4C_MAX_LENGTH];

#ifndef NDEBUG

    /**
     * The name of the source file that threw this exception.
     *
     * @pre
     * This member is only available if <tt>NDEBUG</tt> is not defined.
     */
    const char * file;

    /**
     * The line number in the source file that threw this exception.
     *
     * @pre
     * This member is only available if <tt>NDEBUG</tt> is not defined.
     */
    int line;

#endif

};

/**
 * @internal
 * @brief Represents the current status of exceptions.
 */
struct e4c_context {
    unsigned char blocks;
    struct e4c_exception thrown;
    struct e4c_block {
      unsigned char stage;
      unsigned char uncaught;
      jmp_buf jump;
    } block[EXCEPTIONS4C_MAX_BLOCKS];
};

/**
 * @internal
 * @brief Returns the current exception block.
 */
#define EXCEPTION_BLOCK                                                     \
                                                                            \
  exceptions4c.block[exceptions4c.blocks - 1]

/**
 * @internal
 * @brief Returns whether the current exception block is in range.
 */
#define EXCEPTION_BLOCK_RANGE_CHECK                                         \
                                                                            \
  exceptions4c.blocks > 0 && exceptions4c.blocks <= EXCEPTIONS4C_MAX_BLOCKS

/**
 * @internal
 * @brief Propagates the current exception to the outer exception block.
 */
#define EXCEPTION_PROPAGATE                                                 \
                                                                            \
  longjmp(EXCEPTION_BLOCK.jump, EXCEPTION_BLOCK.uncaught = 1)

/**
 * Contains the current status of exceptions.
 *
 * You MUST define this global variable for your program.
 *
 * ```c
 * struct e4c_context exceptions4c = {0};
 * ```
 */
extern struct e4c_context exceptions4c;

/**
 * Introduces a block of code that may throw exceptions during execution.
 *
 * The #TRY block is used to define a section of code where exceptions might
 * occur. It allows you to handle exceptions gracefully using other blocks that
 * follow it. If an exception occurs, control is transferred to the appropriate
 * block.
 *
 * A single #TRY block can be followed by:
 *
 * - One or more #CATCH blocks to handle specific types of exceptions.
 * - Optionally, one #CATCH_ALL block to handle all exception types (if present,
 *   it must appear after all #CATCH blocks).
 * - Optionally, one #FINALLY block to execute cleanup code, regardless of
 *   whether an exception was thrown or caught.
 *
 * @note
 * The blocks must appear in this order: #CATCH blocks (if any), #CATCH_ALL
 * block (if any), and #FINALLY block (if any). A single #TRY block MUST be
 * followed by, at least, one #CATCH, #CATCH_ALL, or #FINALLY block.
 *
 * @see CATCH
 * @see CATCH_ALL
 * @see FINALLY
 */
#define TRY                                                                 \
                                                                            \
  for (                                                                     \
    (void) (exceptions4c.blocks >= EXCEPTIONS4C_MAX_BLOCKS                  \
      && ((void) (EXCEPTIONS4C_PANIC), 0)),                                 \
    exceptions4c.blocks++,                                                  \
    EXCEPTION_BLOCK.stage = EXCEPTION_BLOCK.uncaught = 0,                   \
    (void) setjmp(EXCEPTION_BLOCK.jump);                                    \
                                                                            \
    EXCEPTION_BLOCK_RANGE_CHECK && (++EXCEPTION_BLOCK.stage < 4             \
      || (exceptions4c.block[--exceptions4c.blocks].uncaught                \
        && ((void) (exceptions4c.blocks > 0 && (EXCEPTION_PROPAGATE, 0)),   \
          (void) (EXCEPTIONS4C_TERMINATE), 0)));                            \
  )                                                                         \
    if (EXCEPTION_BLOCK.stage == 1)

/**
 * Introduces a block of code that handles exceptions thrown by a preceding #TRY
 * block.
 *
 * @param exception_type the type of exception to catch.
 *
 * One or more #CATCH blocks can follow a #TRY block. Each #CATCH block MUST
 * specify the type of exception it handles.
 *
 * @see TRY
 * @see CATCH_ALL
 *
 */
#define CATCH(exception_type)                                               \
                                                                            \
    else if (EXCEPTION_IS_UNCAUGHT                                          \
      && EXCEPTION_BLOCK.stage == 2                                         \
      && (exception_type) == EXCEPTION.type                                 \
      && (EXCEPTION_BLOCK.uncaught = 0, 1))

/**
 * Introduces a block of code that handles any exception thrown by a preceding
 * #TRY block, regardless of its type.
 *
 * The #CATCH_ALL block works like a general #CATCH block that does not require
 * specifying the type of exception to handle. It acts as a fallback for
 * catching all exceptions, including those not explicitly declared in other
 * #CATCH blocks.
 *
 * Only one #CATCH_ALL block is allowed per #TRY block, and it must appear after
 * all type-specific #CATCH blocks if any are present.
 *
 * @remark
 * Using a #CATCH_ALL block is useful for logging, debugging, or handling
 * unexpected exceptions that don't fit into specific categories. However,
 * specific #CATCH blocks SHOULD be used whenever possible to maintain clarity
 * and precise control over exception handling.
 *
 * @see TRY
 * @see CATCH
 */
#define CATCH_ALL                                                           \
                                                                            \
    else if (EXCEPTION_IS_UNCAUGHT                                          \
      && EXCEPTION_BLOCK.stage == 2                                         \
      && (EXCEPTION_BLOCK.uncaught = 0, 1))

/**
 * Introduces a block of code that is executed after a #TRY block, regardless of
 * whether an exception was thrown or not.
 *
 * A #FINALLY block can follow a #TRY block, with or without accompanying #CATCH
 * blocks. Only one #FINALLY block is allowed per #TRY block.
 *
 * @remark
 * It is typically used to release resources, close files, or perform cleanup
 * tasks.
 *
 * @see TRY
 */
#define FINALLY                                                             \
                                                                            \
    else if (EXCEPTION_BLOCK_RANGE_CHECK && EXCEPTION_BLOCK.stage == 3)

/**
 * Throws an exception, interrupting the normal flow of execution.
 *
 * @param exception_type the type of the exception to throw.
 * @param error_message the error message.
 *
 * #THROW is used within a #TRY block, a #CATCH block, or any other function to
 * signal that an error has occurred. The thrown exception will be of the
 * specified type, and it MAY be handled by a preceding #CATCH block.
 *
 * If a thrown exception is not handled by any of the #CATCH blocks in the
 * current function, it propagates up the call stack to the function that called
 * the current function. This continues until the exception is either handled by
 * a #CATCH block higher in the stack, or it reaches the top level of the
 * program. If no #CATCH block handles the exception, the program terminates and
 * an error message is printed to the console.
 *
 * @note
 * The error message will be copied as it is into the thrown #EXCEPTION. To use
 * a formatted error message, use #THROWF instead.
 *
 * @see THROWF
 */
#define THROW(exception_type, error_message)                                \
                                                                            \
  (EXCEPTION.type = (exception_type), EXCEPTION.name = (error_message),     \
    (void) sprintf(EXCEPTION.message, "%.*s",                               \
      (int) (EXCEPTIONS4C_MAX_LENGTH),                                      \
      EXCEPTION.name ? EXCEPTION.name : EXCEPTION.type),                    \
      EXCEPTION.name = #exception_type, EXCEPTION_RETHROW)

#ifndef THROWF

/**
 * Throws an exception with a formatted error message.
 *
 * @param exception_type the type of the exception to throw.
 * @param format the error message.
 * @param ... a list of arguments that will be formatted according to
 *   <tt>format</tt>.
 *
 * This macro works just like #THROW, but it allows you to format the error
 * message, just as you would with <tt>printf</tt>.
 *
 * @see THROW
 */
#define THROWF(exception_type, format, ...)                                 \
                                                                            \
  (EXCEPTION.type = (exception_type), EXCEPTION.name = #exception_type,     \
    (void) snprintf(EXCEPTION.message, (EXCEPTIONS4C_MAX_LENGTH),           \
      (format), __VA_ARGS__), EXCEPTION_RETHROW)

#endif

/**
 * Retrieves the last exception that was thrown.
 *
 * @return the last exception that was thrown.
 *
 * This macro SHOULD be used in the body of a #CATCH or #CATCH_ALL block to
 * inspect the exception being handled.
 *
 * It MAY also be used in the body of a #FINALLY block to determine if an
 * exception was thrown in the corresponding #TRY block, or during the execution
 * of a #CATCH or #CATCH_ALL block.
 *
 * @see e4c_exception
 * @see THROW
 * @see CATCH
 * @see FINALLY
 * @see EXCEPTION_IS_UNCAUGHT
 */
#define EXCEPTION                                                           \
                                                                            \
  exceptions4c.thrown

/**
 * Determines whether the current exception (if any) hasn't been handled yet by
 * any #CATCH or #CATCH_ALL block.
 *
 * @return whether the thrown exception was not caught by any #CATCH or
 *   #CATCH_ALL block.
 *
 * An exception is considered "uncaught" if no matching #CATCH or #CATCH_ALL
 * block has been executed for it. In other words, this macro evaluates to a
 * truthy value if the exception has bypassed all specific exception-handling
 * logic and is propagating further. And it evaluates to a falsy value if no
 * exception was thrown in the #TRY block, or if an exception was successfully
 * caught.
 *
 * @remark
 * This macro SHOULD be used exclusively in the body of a #FINALLY block to
 * check whether an exception thrown during the #TRY block has propagated past
 * all #CATCH and #CATCH_ALL blocks without being handled.
 *
 * @see FINALLY
 * @see EXCEPTION
 */
#define EXCEPTION_IS_UNCAUGHT                                               \
                                                                            \
  (EXCEPTION_BLOCK_RANGE_CHECK && EXCEPTION_BLOCK.uncaught)

#ifndef NDEBUG

/**
 * Prints the current exception to standard error output and flushes it.
 *
 * @see EXCEPTION
 */
#define EXCEPTION_PRINT                                                     \
                                                                            \
  (fprintf(stderr, "\n%s: %s\n    at %s:%d\n",                              \
    EXCEPTION.name, EXCEPTION.message, EXCEPTION.file, EXCEPTION.line))

#else

/**
 * Prints the current exception to standard error output and flushes it.
 *
 * @see EXCEPTION
 */
#define EXCEPTION_PRINT                                                     \
                                                                            \
  (fprintf(stderr, "\n%s: %s\n", EXCEPTION.name, EXCEPTION.message))

#endif

#ifndef NDEBUG

/**
 * Throws the current exception again.
 *
 * @remark
 * This macro SHOULD be used in the body of #CATCH or #CATCH_ALL blocks to throw
 * the exception that is currently being handled.
 *
 * @see CATCH
 * @see CATCH_ALL
 */
#define EXCEPTION_RETHROW                                                   \
                                                                            \
  (EXCEPTION.file = __FILE__, EXCEPTION.line = __LINE__,                    \
    (exceptions4c.blocks <= 0 && ((void) (EXCEPTIONS4C_TERMINATE), 0)),     \
    EXCEPTION_PROPAGATE)

#else

/**
 * Throws the current exception again.
 *
 * @remark
 * This macro SHOULD be used in the body of #CATCH or #CATCH_ALL blocks to throw
 * the exception that is currently being handled.
 *
 * @see CATCH
 * @see CATCH_ALL
 */
#define EXCEPTION_RETHROW                                                   \
                                                                            \
  ((exceptions4c.blocks <= 0 && ((void) (EXCEPTIONS4C_TERMINATE), 0)),      \
    EXCEPTION_PROPAGATE)

#endif

/* OpenMP support */
#ifdef _OPENMP
# pragma omp threadprivate(exceptions4c)
#endif

#endif
