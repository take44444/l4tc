#!/usr/bin/env bash

function compiler_code {
    echo '#include <bits/stdc++.h>' && \
    local tmpdir=$(mktemp -d 2>/dev/null || mktemp -d -t 'l4t_atcoder_compiler_code') && \
    for d in generator parser tokenizer; do mkdir "${tmpdir}/${d}"; done && \
    for f in generator/* parser/* tokenizer/*; do sed '/#include/d' "${f}" > "${tmpdir}/${f}"; done && \
    sed '/#include "bits\/stdc++.h"/d' l4tc.cpp > "${tmpdir}/l4tc.cpp" && \
    cp l4tc.hpp "${tmpdir}/l4tc.hpp" && \
    g++ -std=c++17 -E "${tmpdir}/l4tc.cpp" "${tmpdir}/tokenizer/tokenizer.cpp" \
        "${tmpdir}/parser/parser.cpp" "${tmpdir}/parser/utils.cpp" \
        "${tmpdir}/generator/generator.cpp" "${tmpdir}/generator/utils.cpp" | sed '/#/d' | cat -s && \
    rm -rf "${tmpdir}"
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
    tmpdir=$(mktemp -d 2>/dev/null || mktemp -d -t 'l4t_atcoder')
    compiler_code > "${tmpdir}/compiler.cpp" && \
    compose $1 "${tmpdir}/compiler.cpp" && \
    rm "${tmpdir}/compiler.cpp"
fi

