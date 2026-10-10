#pragma once


#include "../lexer/lexer.h"


void set_lexer(Lexer* l);
int yylex();