#!/bin/bash
if [ \( -z "$1" \) ]; then
    echo Error: must supply one argument
    echo usage: 1.0.5.sh var1
    echo -e "\tprints var1 in hexadecimal format"
    exit 1
fi
printf "%x\n" $1
