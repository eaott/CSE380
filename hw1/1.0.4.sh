#!/bin/bash

n=5
for i in `seq $n`
do
    skip=`expr $n \- $i`
    for j in `seq $skip`
    do
        echo -ne " "
    done
    for j in `seq $i`
    do
        echo -ne "$i "
    done
    echo
done
for i in `seq $n`
do
    skip=`expr $n \- $i`
    for j in `seq $skip`
    do
        echo -ne " "
    done
    for j in `seq $i`
    do
        echo -ne ". "
    done
    echo
done
