#
# exceptions4c-lite
#
# Copyright (c) 2025 Guillermo Calvo
# Licensed under the Apache License, Version 2.0
#

AUTOMAKE_OPTIONS = foreign subdir-objects

AM_CFLAGS = -Wall -Werror --pedantic -Wno-missing-braces -Wno-dangling-else -Isrc

include_HEADERS = src/exceptions4c-lite.h

# Documentation

docsdir = $(datadir)/docs/exceptions4c-lite
docs_DATA = docs/*


# Check

check_PROGRAMS =                    \
    bin/check/catch-all             \
    bin/check/catch                 \
    bin/check/finally               \
    bin/check/limits                \
    bin/check/overflow              \
    bin/check/throw-uncaught        \
    bin/check/throw                 \
    bin/check/throwf-uncaught       \
    bin/check/throwf                \
    bin/check/pet-store

TESTS =                             \
    bin/check/catch-all             \
    bin/check/catch                 \
    bin/check/finally               \
    bin/check/limits                \
    bin/check/overflow              \
    bin/check/throw-uncaught        \
    bin/check/throw                 \
    bin/check/throwf-uncaught       \
    bin/check/throwf

XFAIL_TESTS =                       \
    bin/check/overflow              \
    bin/check/throw-uncaught        \
    bin/check/throwf-uncaught

tests: check


# Tests

bin_check_catch_all_SOURCES         = tests/catch-all.c
bin_check_catch_SOURCES             = tests/catch.c
bin_check_finally_SOURCES           = tests/finally.c
bin_check_limits_SOURCES            = tests/limits.c
bin_check_overflow_SOURCES          = tests/overflow.c
bin_check_throw_uncaught_SOURCES    = tests/throw-uncaught.c
bin_check_throw_SOURCES             = tests/throw.c
bin_check_throwf_uncaught_SOURCES   = tests/throwf-uncaught.c
bin_check_throwf_SOURCES            = tests/throwf.c
bin_check_pet_store_SOURCES         = examples/pet-store.c


# Generate documentation

docs: docs/html/index.html

docs/html/index.html: docs/Doxyfile docs/doxygen-awesome.css docs/doxygen-awesome-sidebar-only.css
	doxygen docs/Doxyfile

docs/doxygen-awesome.css:
	wget --verbose --output-document docs/doxygen-awesome.css https://github.com/jothepro/doxygen-awesome-css/raw/refs/heads/main/doxygen-awesome.css

docs/doxygen-awesome-sidebar-only.css:
	wget --verbose --output-document docs/doxygen-awesome-sidebar-only.css https://github.com/jothepro/doxygen-awesome-css/raw/refs/heads/main/doxygen-awesome-sidebar-only.css
