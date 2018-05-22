#!/usr/bin/bash

echo "" > ./script/data.txt

for x in {1..15}
do
    ./bin/viajante $x >> ./script/data.txt
done