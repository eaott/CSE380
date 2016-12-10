#!/bin/bash
mkdir -p inputs
mkdir -p outputs
rm -f inputs/*
rm -f outputs/*
python "pr2_create_input.py"
make

for file in `ls inputs`
do
  echo $file
  (time -p ./runner "inputs/$file") 2> "outputs/time_$file" 1> "outputs/val_$file"
  (grep "user" "outputs/time_$file" | cut -d" " -f2) >> "outputs/val_$file"
  rm "outputs/time_$file"
done

python "pr2_results.py"
