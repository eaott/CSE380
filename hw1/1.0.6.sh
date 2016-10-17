#!/bin/bash
function help_fn() {
    echo "1.0.6.sh [flags] [val]"
    echo "FLAGS:"
    echo -e "\t-h: print this message"
    echo -e "\t-c: convert val to celsius"
    echo -e "\t-f: convert val to fahrenheit"
    echo -e "Can be called with no arguments to enter"
    echo -e "into interactive mode"
    exit
}

function celcius() {
        str="5 / 9 * ($1 - 32)"
        res=`bc -l <<< $str`
        printf "%.3f\n" $res
}
function fahrenheit() {
            str="32 + 9 / 5 * $1"
            res=`bc -l <<< $str`
            printf "%.3f\n" $res
}

if [ \( -z "$1" \) ]; then
    echo -n "Input value: "
    read VAL
    echo -n "Convert to Celcius (C) or Fahrenheit (F): "
    read CONVERT
    if [[ $CONVERT == "C" ]]; then
        celcius $VAL
    else
        fahrenheit $VAL
    fi
    
else
 while getopts ":f:c:h" flag; do
    case $flag in
        f)
             fahrenheit $OPTARG 
        ;;
        c)
             celcius $OPTARG
	;;
	h)
		help_fn
	;;
        \?)
            help_fn
        ;;
    esac
 done
fi
