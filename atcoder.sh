#!/usr/bin/env bash

function compiler_code {
    echo '#include <bits/stdc++.h>' && \
    mkdir tmp && \
    for d in generator parser tokenizer; do mkdir "tmp/${d}"; done && \
    for f in generator/* parser/* tokenizer/*; do sed '/#include/d' "${f}" > "tmp/${f}"; done && \
    sed '/#include "bits\/stdc++.h"/d' l4tc.cpp > tmp/l4tc.cpp && \
    cp l4tc.hpp tmp/l4tc.hpp && \
    g++ -std=c++17 -E tmp/l4tc.cpp tmp/tokenizer/tokenizer.cpp \
        tmp/parser/parser.cpp tmp/parser/utils.cpp \
        tmp/generator/generator.cpp tmp/generator/utils.cpp | sed '/#/d' | cat -s && \
    rm -rf tmp
}

function compose {
    cat - << EOS
import sys
import os


if sys.argv[-1] != 'ONLINE_JUDGE':
    os.system('./main')
else:
    program_code = r'''
EOS
    cat $1
    cat - << EOS
'''

    compiler_code = r'''
EOS
    cat $2
    cat - << EOS
'''

    with open('main.l4t', 'w') as f:
        f.write(program_code)
    with open('l4tc.cpp', 'w') as f:
        f.write(compiler_code)

    os.system('g++ -std=c++17 -O2 -o l4tc l4tc.cpp && '
              './l4tc < main.l4t > main.S && '
              'gcc -fsplit-stack -o main main.S')
EOS
}

if [ $# -ne 1 ]; then
    echo Usage: $(basename $0) your_L4T_code_filename
    exit 1
else
    compiler_code > tmp.cpp && \
    compose $1 tmp.cpp && \
    rm tmp.cpp
fi

