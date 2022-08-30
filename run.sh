#!/bin/bash
if [ $# -lt 1 ]; then
  echo 'need argument' >&2
  exit 1
fi
make
if [ $? -gt 0 ]; then
  exit 1
fi
rm -f $1.S
cat main.lup | ./lupc 1> $1.S
if [ $? -gt 0 ]; then
  exit 1
fi
gcc -o $1 $1.S
if [ $? -gt 0 ]; then
  exit 1
fi
./$1