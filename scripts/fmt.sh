#!/bin/bash

find include src tests \
    \( -path tests/build -o -path tests/nala \) -prune -false -o \
    \( -iname "*.h" \) -or \( -iname "*.c" \) | xargs clang-format -i
