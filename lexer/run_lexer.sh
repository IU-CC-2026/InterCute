#!/bin/bash

cd "$(dirname "$0")/.." || exit 1

g++ lexer/test.cpp lexer/lexer.cpp -o lexer/cute_lexer || exit 1

./lexer/cute_lexer