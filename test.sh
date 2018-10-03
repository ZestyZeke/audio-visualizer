#!/bin/bash

# Bash script for testing this program

rm -rf build

mkdir build && cd build

cmake ..

make

cd src

./analyzable
