#!/bin/bash

cd "$(dirname "$0")" || exit 1

bison -d -o parser/parser.cpp parser/parser.y || exit 1

g++ main.cpp lexer/lexer.cpp ast/ast.cpp \
    parser/adapter.cpp parser/parser.cpp \
    semantic/semantic_analyzer.cpp \
    optimizer/optimizer.cpp \
    -o cute_compiler || exit 1

./cute_compiler