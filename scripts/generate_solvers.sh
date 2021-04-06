#!/bin/bash

for w in 4 5 6 7 8
do
    for h in 4 5 6 7 8
    do
        echo "Compile solver for ${w}x${h} grid..."
        g++ -o solver${w}x${h} source/main.cpp source/Node.cpp -Iinclude/ -pthread
    done
done
