#!/usr/bin/env sh

set -ex

mkdir -p build
cd build
cmake -G "Unix Makefiles" ..
cmake --build .

./main.exe
