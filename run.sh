#!/bin/bash
if [ $# -lt 1 ]; then
  echo 'need argument' >&2
  exit 1
fi
make
rm -f $1.S
cat main.lup | ./lupc 1> $1.S
gcc -o $1 $1.S
./$1
echo $?