#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.


# Autoconf requirements
AC_PREREQ([2.68])


# Initialize package
AC_INIT([exceptions4c-lite], [1.0.0], [exceptions4c-lite@guillermo.dev], [exceptions4c-lite], [https://github.com/guillermocalvo/exceptions4c-lite/])


# Information on the package
AC_COPYRIGHT([Copyright 2025 Guillermo Calvo])
AC_REVISION([$PACKAGE_VERSION])
AC_MSG_NOTICE([
                         _   _                 _  _
  _____  _____ ___ _ __ | |_(_) ___  _ __  ___| || |   ___
 / _ \ \/ / __/ _ \ '_ \| __| |/ _ \| '_ \/ __| || |_ / __|
|  __/>  < (_|  __/ |_) | |_| | (_) | | | \__ \__   _| (__
 \___/_/\_\___\___| .__/ \__|_|\___/|_| |_|___/  |_|  \___|
                  |_| Lightweight exception handling for C
])


# Check if the source folder is correct
AC_CONFIG_SRCDIR([src/exceptions4c-lite.h])


# Checks for programs.
AC_PROG_CC
AC_PROG_CPP
AC_PROG_RANLIB
AM_PROG_AR


# Checks for header files.
AC_CHECK_HEADERS([setjmp.h])
AC_CHECK_HEADERS([stdio.h])
AC_CHECK_HEADERS([stdlib.h])


# Checks for types
AC_TYPE_SIZE_T
AC_CHECK_TYPES(jmp_buf)


# Checks for compiler characteristics
AC_LANG([C])


# Checks for library functions.
AC_FUNC_MALLOC
AC_CHECK_FUNCS([exit])
AC_CHECK_FUNCS([fflush])
AC_CHECK_FUNCS([fprintf])
AC_CHECK_FUNCS([longjmp])
AC_CHECK_FUNCS([setjmp])
AC_CHECK_FUNCS([snprintf])


# The config file is generated but not used by the source code
#AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
    exceptions4c-lite.pc
    Makefile
])


# Automake initialisation
AM_INIT_AUTOMAKE([
    -Wall
    -Werror
    foreign
    subdir-objects
    no-define
])


AC_OUTPUT
