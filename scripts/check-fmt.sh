#!/bin/bash

./run-clang-format/run-clang-format.py \
    --exclude tests/build \
    -r include src tests
