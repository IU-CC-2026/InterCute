#!/bin/bash

cd "$(dirname "$0")/.." || exit 1

bison -d -o parser/parser.cpp parser/parser.y || exit 1

g++ parser/test.cpp lexer/lexer.cpp ast/ast.cpp parser/adapter.cpp parser/parser.cpp -o parser/cute_parser || exit 1

./parser/cute_parser