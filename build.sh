#!/bin/bash
if [[ $# -eq 1 ]]; then
    xxd -i -n rom $1 > inc/app/rom.h
    sed -i "s/unsigned char rom\[\]/const unsigned char rom\[\] __attribute__((used,section(\".emurom\")))/g" inc/app/rom.h
    make clean
    make
    make flash
else
    echo "usage: build.sh <your_rom.gb>"
fi

