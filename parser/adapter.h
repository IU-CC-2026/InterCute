#pragma once


#include "../lexer/lexer.h"
#include "parser.hpp"
#include "../ast/ast.h"


void set_lexer(Lexer* l);

SourceLocation make_loc(const YYLTYPE& y);

int yylex();