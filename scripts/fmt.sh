#!/bin/bash

find include src tests \
    \( -path examples/build -o -path tests/build \) -prune -false -o \
    \( -iname "*.h" \) -or \( -iname "*.c" \) | xargs clang-format -i
