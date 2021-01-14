#!/bin/bash

find include src tests -iname *.h -o -iname *.c \
    -path tests/build -path tests/slice99 -prune \
    | xargs clang-format -i
