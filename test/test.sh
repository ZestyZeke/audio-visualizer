#!/bin/bash

# Bash script for testing this program

# -------------------------------------------------------
# this is the directory everything should be built to
# -------------------------------------------------------
BUILD_DIR=audio-build
cd ../
if [ ! -d "$BUILD_DIR" ]; then
    echo "Please run the install.sh to install the program"
fi
cd "$BUILD_DIR"/src

# -------------------------------------------------------
# arguments should provide filename
# -------------------------------------------------------
if [ "$#" -ge 3]; then
    echo "Illegal number of parameters"
    echo "Usage: $0 <file-name-without-path>"
fi

# -------------------------------------------------------
# run program
# -------------------------------------------------------
if [ "$#" -eq 2]; then
    ./analyzable $1
fi

if [ "$#" -eq 1]; then
    ./analyzable
fi
