#!/bin/bash
function help {
    echo something
    exit
}
if [ \( -z "$1" \) ]; then
    echo interactive
fi

while getopts ":f:c:i" flag; do
    case $flag in
        f)
            str="5 / 9 * ($OPTARG - 32)"
            fahrenheit=`bc -l <<< $str`
            printf "%.3f\n" $fahrenheit
        ;;
        c)
            str="32 + 9 / 5 * $OPTARG"
            celcius=`bc -l <<< $str`
            printf "%.3f\n" $celcius
        ;;
        \?)
            help
        ;;
    esac
done

