#!/bin/bash
if [ \( -z "$1" \) -o \( -z "$2" \) ]; then
    echo Error: must supply two arguments
    echo usage: 1.0.1.sh var1 var2
    echo -e "\tprints var1+var2"
    exit 1
fi
echo $(($1 + $2))
