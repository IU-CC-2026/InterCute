bison -d -o parser/parser.cpp parser/parser.y || exit 1

g++ main.cpp lexer/lexer.cpp ast/ast.cpp parser/adapter.cpp parser/parser.cpp -o cute_compiler || exit 1

./cute_compiler