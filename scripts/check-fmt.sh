#!/bin/bash

./run-clang-format/run-clang-format.py \
    --exclude tests/slice99 \
    --exclude tests/build \
    -r include src tests
