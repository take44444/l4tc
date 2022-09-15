#!/bin/bash
ESC=$(printf '\033')
rm -f ./$1/result/*
rm -f $2.S
cat $2.l4t | ./l4tc 1> $2.S
gcc-9 -o $2 $2.S
testcases=`ls ./$1/in/`
TIMEFORMAT='%R'
result=0
for f in $testcases
do
  echo -n "Case Name: $f / Result: " >&2
  T=`( time ./$2 < ./$1/in/$f > ./$1/result/$f 2> /dev/null ) 2>&1`
  if [ `echo "$T > 2.0" | bc` == 1 ]; then
    echo -n "${ESC}[33mTLE${ESC}[m" >&2
    result=2
  else
    diff ./$1/result/$f ./$1/out/$f > /dev/null 2>&1
    if [ $? -ne 0 ]; then
      echo -n "${ESC}[33mWA${ESC}[m" >&2
      if [ $result -ne 2 ]; then
        result=1
      fi
    else
      echo -n "${ESC}[32mAC${ESC}[m" >&2
    fi
  fi
  echo " / Exec Time: $T sec" >&2
done

if [ $result -eq 0 ]; then
  echo "Judge Result: ${ESC}[32mAC${ESC}[m" >&2
elif [ $result -eq 1 ]; then
  echo "Judge Result: ${ESC}[33mWA${ESC}[m" >&2
  exit 1
else
  echo "Judge Result: ${ESC}[33mTLE${ESC}[m" >&2
  exit 1
fi