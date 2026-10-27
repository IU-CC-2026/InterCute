#include "../lexer/lexer.h"
#include "parser.hpp"
#include "../ast/ast.h"
#include <string>
#include <iostream>
#include <exception>


Lexer* lexer = nullptr;

void set_lexer(Lexer* l) {
    lexer = l;
}

int mapping(TokenType type) {
    switch (type) {
        case TokenType::VAR: return VAR;
        case TokenType::IS: return IS;

        case TokenType::IF: return IF;
        case TokenType::THEN: return THEN;
        case TokenType::ELSE: return ELSE;

        case TokenType::FOR: return FOR;
        case TokenType::WHILE: return WHILE;
        case TokenType::IN: return IN;
        case TokenType::LOOP: return LOOP;
        case TokenType::EXIT: return EXIT;

        case TokenType::OR: return OR;
        case TokenType::AND: return AND;
        case TokenType::XOR: return XOR;
        case TokenType::NOT: return NOT;

        case TokenType::PRINT: return PRINT;
        case TokenType::RETURN: return RETURN;

        case TokenType::END: return END;

        case TokenType::INT: return INT;
        case TokenType::REAL: return REAL;
        case TokenType::BOOL: return BOOL;
        case TokenType::STRING: return STRING;
        case TokenType::FUNC: return FUNC;

        case TokenType::PLUS: return PLUS;
        case TokenType::MINUS: return MINUS;
        case TokenType::MUL: return MUL;
        case TokenType::DIV: return DIV;

        case TokenType::LT: return LT;
        case TokenType::LE: return LE;
        case TokenType::GT: return GT;
        case TokenType::GE: return GE;
        case TokenType::EQ: return EQ;
        case TokenType::NEQ: return NEQ;

        case TokenType::ASSIGN: return ASSIGN;

        case TokenType::ARROW: return ARROW;

        case TokenType::RANGE: return RANGE;

        case TokenType::OPEN_ROUND_BRACKET: return OPEN_ROUND_BRACKET;
        case TokenType::CLOSE_ROUND_BRACKET: return CLOSE_ROUND_BRACKET;

        case TokenType::OPEN_SQUARE_BRACKET: return OPEN_SQUARE_BRACKET;
        case TokenType::CLOSE_SQUARE_BRACKET: return CLOSE_SQUARE_BRACKET;

        case TokenType::OPEN_BRACE: return OPEN_BRACE;
        case TokenType::CLOSE_BRACE: return CLOSE_BRACE;

        case TokenType::COMMA: return COMMA;
        case TokenType::DOT: return DOT;

        case TokenType::IDENTIFIER: return IDENTIFIER;

        case TokenType::SEMICOLON: return SEMICOLON;
        case TokenType::NEWLINE: return NEWLINE;

        case TokenType::INT_LITERAL: return INT_LITERAL;
        case TokenType::REAL_LITERAL: return REAL_LITERAL;
        case TokenType::STRING_LITERAL: return STRING_LITERAL;

        case TokenType::TRUE: return TRUE;
        case TokenType::FALSE: return FALSE;

        case TokenType::NONE: return NONE;

        case TokenType::END_OF_FILE: return YYEOF;
    }

    return YYUNDEF;
}


int yylex() {
    if (lexer == nullptr) {
        return YYEOF;
    }

    Token token = lexer->get_token();

    yylloc.first_line = token.line;
    yylloc.last_line = token.line;
    yylloc.first_column = token.column;
    yylloc.last_column = token.column;

    try {
        switch (token.type) {
            case TokenType::INT_LITERAL:
                yylval = new IntLiteral(std::stoi(token.value));
                break;

            case TokenType::REAL_LITERAL:
                yylval = new RealLiteral(std::stod(token.value));
                break;

            case TokenType::STRING_LITERAL:
                yylval = new StringLiteral(token.value);
                break;

            case TokenType::TRUE:
                yylval = new BoolLiteral(true);
                break;
            
            case TokenType::FALSE:
                yylval = new BoolLiteral(false);
                break;

            case TokenType::NONE:
                yylval = new NoneLiteral();
                break;

            case TokenType::IDENTIFIER:
                yylval = new Reference(token.value);
                break;

            default:
                yylval = nullptr;
                break;
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return YYUNDEF;
    }
    

    return mapping(token.type);
}

void yyerror(Program*& root, std::string& error, const char* s) {
    error = "Syntax error at line " + std::to_string(yylloc.first_line) +
    ", column " + std::to_string(yylloc.first_column) + ": " + s + '\n';
}