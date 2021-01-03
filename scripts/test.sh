#!/bin/bash

mkdir tests/build -p
cd tests/build
cmake ..
cmake --build .
./tests
