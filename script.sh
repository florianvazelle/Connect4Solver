#!/bin/sh
a=4
while [ $a -lt 7 ] ;
do
  b=4
  while [ $b -lt 7 ] ;
  do
    #if [ $a -eq 6 ] && [ $b -eq 6 ] then break fi
    echo "compilation pour W=$b et H=$a"
    g++ -o exe -g -W -Wall -O3 -std=c++14 main.cpp Node.cpp Board.cpp -DW=$b -DH=$a -pthread
    echo "./connect$b$a"
    ./exe
    b=$(($b+1))
  done
  a=$(($a+1))
done
