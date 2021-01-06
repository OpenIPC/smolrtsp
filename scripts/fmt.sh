#!/bin/bash

find include src tests -iname *.h -o -iname *.c -path tests/build -prune | xargs clang-format -i
