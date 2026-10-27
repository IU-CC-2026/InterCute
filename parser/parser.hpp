/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_PARSER_PARSER_HPP_INCLUDED
# define YY_YY_PARSER_PARSER_HPP_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 85 "parser/parser.y"

    #include "../ast/ast.h"

#line 53 "parser/parser.hpp"

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    VAR = 258,                     /* VAR  */
    IS = 259,                      /* IS  */
    IF = 260,                      /* IF  */
    THEN = 261,                    /* THEN  */
    ELSE = 262,                    /* ELSE  */
    FOR = 263,                     /* FOR  */
    WHILE = 264,                   /* WHILE  */
    IN = 265,                      /* IN  */
    LOOP = 266,                    /* LOOP  */
    EXIT = 267,                    /* EXIT  */
    OR = 268,                      /* OR  */
    AND = 269,                     /* AND  */
    XOR = 270,                     /* XOR  */
    NOT = 271,                     /* NOT  */
    PRINT = 272,                   /* PRINT  */
    RETURN = 273,                  /* RETURN  */
    END = 274,                     /* END  */
    INT = 275,                     /* INT  */
    REAL = 276,                    /* REAL  */
    BOOL = 277,                    /* BOOL  */
    STRING = 278,                  /* STRING  */
    FUNC = 279,                    /* FUNC  */
    PLUS = 280,                    /* PLUS  */
    MINUS = 281,                   /* MINUS  */
    MUL = 282,                     /* MUL  */
    DIV = 283,                     /* DIV  */
    LT = 284,                      /* LT  */
    LE = 285,                      /* LE  */
    GT = 286,                      /* GT  */
    GE = 287,                      /* GE  */
    EQ = 288,                      /* EQ  */
    NEQ = 289,                     /* NEQ  */
    ASSIGN = 290,                  /* ASSIGN  */
    ARROW = 291,                   /* ARROW  */
    RANGE = 292,                   /* RANGE  */
    OPEN_ROUND_BRACKET = 293,      /* OPEN_ROUND_BRACKET  */
    CLOSE_ROUND_BRACKET = 294,     /* CLOSE_ROUND_BRACKET  */
    OPEN_SQUARE_BRACKET = 295,     /* OPEN_SQUARE_BRACKET  */
    CLOSE_SQUARE_BRACKET = 296,    /* CLOSE_SQUARE_BRACKET  */
    OPEN_BRACE = 297,              /* OPEN_BRACE  */
    CLOSE_BRACE = 298,             /* CLOSE_BRACE  */
    COMMA = 299,                   /* COMMA  */
    DOT = 300,                     /* DOT  */
    IDENTIFIER = 301,              /* IDENTIFIER  */
    SEMICOLON = 302,               /* SEMICOLON  */
    NEWLINE = 303,                 /* NEWLINE  */
    INT_LITERAL = 304,             /* INT_LITERAL  */
    REAL_LITERAL = 305,            /* REAL_LITERAL  */
    STRING_LITERAL = 306,          /* STRING_LITERAL  */
    TRUE = 307,                    /* TRUE  */
    FALSE = 308,                   /* FALSE  */
    NONE = 309,                    /* NONE  */
    FUNC_BODY = 310,               /* FUNC_BODY  */
    UNARY = 311                    /* UNARY  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef ASTNode* YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif


extern YYSTYPE yylval;
extern YYLTYPE yylloc;

int yyparse (Program*& root, std::string& error);


#endif /* !YY_YY_PARSER_PARSER_HPP_INCLUDED  */
