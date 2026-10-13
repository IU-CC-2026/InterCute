#!/usr/bin/env bash
set -e
cd -- "$(dirname -- "$0")"

bison -d -o parser/parser.cpp parser/parser.y

g++ -std=c++17 main.cpp lexer/lexer.cpp ast/ast.cpp ast/ast_dump.cpp \
    parser/adapter.cpp parser/parser.cpp -o cute_compiler

./cute_compiler "$@"
