#!/bin/bash
if [ $# -lt 1 ]; then
  echo '[error] Needs arguments.' >&2
  exit 1
fi
echo '[info]  Compiling L4T compiler.'
make 1> /dev/null
if [ $? -gt 0 ]; then
  echo '[error] Failed to compile compiler. Please report to the developer.' >&2
  exit 1
fi
echo '[info]  Successfully compiled L4T compiler.'
echo '[info]  Compiling your L4T code.'
rm -f $1.S
cat main.l4t | ./l4tc 1> $1.S
if [ $? -gt 0 ]; then
  echo '[error] Failed to compile your L4T code. Please check the error message above.' >&2
  exit 1
fi
gcc-9 -o $1 $1.S
if [ $? -gt 0 ]; then
  echo '[error] Failed to assemble. Please report to the developer.' >&2
  exit 1
fi
echo '[info]  Successfully compiled your L4T code.'
echo '[info]  Executing your L4T code.'
./$1
echo "[info]  Execution completed with exit code $?."