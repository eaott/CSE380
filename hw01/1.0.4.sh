#!/bin/bash
function triangle() {
    for i in `seq $1`
    do  
        val="."
        if $2 ; then
            val="$i"
        fi
        skip=`expr $n \- $i`
        for j in `seq $skip`
        do
          echo -ne " "
        done
        stop="$i - 1"
        stop=`bc -l <<< $stop`
        for j in `seq $stop`
        do
            echo -ne "$val "
        done
        echo -ne "$val"
        echo
    done
}
n=5
triangle $n true
triangle $n false
