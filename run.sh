#!/bin/bash
if [ $# -lt 1 ]; then
  echo '[error] Needs arguments.' >&2
  exit 1
fi
echo '[info]  Compiling L4T compiler.' >&2
make 1> /dev/null
if [ $? -gt 0 ]; then
  echo '[error] Failed to compile compiler. Please report to the developer.' >&2
  exit 1
fi
echo '[info]  Successfully compiled L4T compiler.' >&2
echo '[info]  Compiling your L4T code.' >&2
rm -f $1.S
cat $1.l4t | ./l4tc 1> $1.S
if [ $? -gt 0 ]; then
  echo '[error] Failed to compile your L4T code. Please check the error message above.' >&2
  exit 1
fi
gcc-9 -fsplit-stack -o $1 $1.S
if [ $? -gt 0 ]; then
  echo '[error] Failed to assemble. Please report to the developer.' >&2
  exit 1
fi
echo '[info]  Successfully compiled your L4T code.' >&2
echo '[info]  Executing your L4T code.' >&2
./$1
echo "[info]  Execution completed with exit code $?." >&2