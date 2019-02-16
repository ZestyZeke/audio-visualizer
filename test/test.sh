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
if [ "$#" -ge 2 ]; then
    echo "Illegal number of parameters"
    echo "Usage: $0 <file-name-without-path>"
fi

# -------------------------------------------------------
# for displaying help message and exiting
# -------------------------------------------------------
display_help() {
    filename=$(basename $0)
    echo "Usage: $filename [help|-h|--help|--list|filename]"
    exit 1
}


# -------------------------------------------------------
# for listing available files to play
# -------------------------------------------------------
list_files() {
    echo "$PWD"
    list=$(find ../../res -name '*.wav' -exec basename {} \;)
    for song in "$list"
    do
        echo "$song"
    done
    exit 1
}

#@TODO: add 'debug' option
# -------------------------------------------------------
# run program
# -------------------------------------------------------
if [ "$#" -eq 1 ]; then
    if [[ ( $1 == "help" ) || ( $1 == "-h" ) || ( $1 == "--help" ) ]]
    then
        display_help
    fi

    if [[ ( $1 == "--list" ) ]]
    then
        list_files
    fi

    echo "playing $1"
    ./analyzable $1
fi

if [ "$#" -eq 0 ]; then
    ./analyzable
fi
