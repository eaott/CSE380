#!/bin/bash

if [ -z "$1" ]; then
    echo Error: must supply one argument
    echo usage: 1.0.1.sh var1
    echo -e "\tprints factorial of var1"
    exit 1
fi
n=$1
factorial=1
while [ \( $n -gt 0 \) ]; do
    factorial=`expr $factorial \* $n`
    n=`expr $n - 1`
done
echo $factorial
