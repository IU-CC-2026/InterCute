%{
#include "../ast/ast.h"

int yylex();
void yyerror(Program*& root, const char* s);
%}

%code requires {
    #include "../ast/ast.h"
}

%parse-param { Program*& root }

%define api.value.type {ASTNode*}

%token VAR IS
%token IF THEN ELSE
%token FOR WHILE IN LOOP EXIT
%token OR AND XOR NOT
%token PRINT RETURN
%token END
%token INT REAL BOOL STRING FUNC

%token PLUS MINUS MUL DIV
%token LT LE GT GE EQ NEQ
%token ASSIGN
%token ARROW
%token RANGE

%token OPEN_ROUND_BRACKET CLOSE_ROUND_BRACKET
%token OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET
%token OPEN_BRACE CLOSE_BRACE
%token COMMA DOT

%token IDENTIFIER

%token SEMICOLON NEWLINE

%token INT_LITERAL REAL_LITERAL STRING_LITERAL
%token TRUE FALSE
%token NONE


%%

program:
    %empty {
        root = new Program({});
    }
    | program NEWLINE
    ;

%%