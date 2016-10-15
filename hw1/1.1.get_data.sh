#!/bin/bash
for np in 10 50 100 200; do
    startTime=`date +%s%N | cut -b1-13`
    val=`1.1.sh $np 0 5`
    endTime=`date +%s%N | cut -b1-13`
    totalTime="$endTime - $startTime"
    totalTime=`bc -l <<< $totalTime`
    echo "$val $totalTime"
done

