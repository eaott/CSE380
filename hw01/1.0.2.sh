#!/bin/bash
if [ \( -z "$1" \) -o \( -z "$2" \) -o \( -z "$3" \) ]; then
    echo Error: must supply three arguments
    echo usage: 1.0.2.sh var1 var2 var3
    echo -e "\tprints maximum of the three integers"
    exit 1
fi

# We have reasonable variables, now find max
if [ \( $1 -ge $2 \) -a \( $1 -ge $3 \) ]; then
    echo $1
elif [ \( $2 -ge $3 \) ]; then
    echo $2
else
    echo $3
fi
