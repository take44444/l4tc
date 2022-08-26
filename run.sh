#!/bin/bash
make
rm -f main.S
cat main.lup | ./lupc 1> main.S
gcc -o main main.S
./main
echo $?