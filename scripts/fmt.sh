#!/bin/bash

find include src tests -iname *.h -o -iname *.c | xargs clang-format -i
