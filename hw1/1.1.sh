#!/bin/bash
if [ \( -z "$1" \) -o \( -z "$2" \) -o \( -z "$3" \) ]; then
    echo Error: must supply three arguments
    echo usage: 1.1.sh NP Xmin Xmax
    echo -e "\tcomputes the approximate integral of cos(x) from Xmin to Xmax"
    echo -e "\tusing NP discretization points."
    exit 1
fi

NP="$1"
Xmin="$2"
Xmax="$3"
range="$Xmax - $Xmin"
range=`bc -l <<< $range`
h="$range / $NP"
h=`bc -l <<< $h`
total="0"
# time in millis
startTime=`date +%s%N | cut -b1-13`
for i in `seq $NP`
do
    Xstart="$Xmin + $h * ($i - 1)"
    Xstart=`bc -l <<< $Xstart`
    Xmid="$Xstart + $h / 2"
    Xmid=`bc -l <<< $Xmid`
    integral="$h * c($Xmid)"
    integral=`bc -l <<< $integral`
    # I think this is more or less what to do
    # generates NP=3 min=0 max=1: (0, 0.5, 1.0)
    # which are the appropriate points, I think, however
    # we really only want the min of each section plus a midpoint.
    # so maybe overthought this one?
    total="$total + $integral"
    total=`bc -l <<< $total`
done
exact="s($Xmax) - s($Xmin)"
exact=`bc -l <<< $exact`
error="$total - $exact"
error=`bc -l <<< $error`
error="if ($error > 0) { $error } else { -1 * $error }"
error=`bc -l <<< $error`
echo $error
endTime=`date +%s%N | cut -b1-13`
totalTime="$endTime - $startTime"
totalTime=`bc -l <<< $totalTime`
echo $totalTime
