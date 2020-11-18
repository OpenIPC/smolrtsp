#!/bin/bash

find include src -iname *.h -o -iname *.c | xargs clang-format -i
