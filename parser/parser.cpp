/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

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

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 1 "parser/parser.y"

#include "../ast/ast.h"

#include <string>
#include <vector>

int yylex();
void yyerror(Program*& root, std::string& error, const char* s);

struct NodeList : ASTNode {
    std::vector<ASTNode*> items;
    std::string print(int level) override { return ""; }
};

struct TypeNode : ASTNode {
    VariableType type;
    explicit TypeNode(VariableType type) : type(type) {}
    std::string print(int level) override { return ""; }
};

static NodeList* new_list() {
    return new NodeList();
}

static NodeList* list_of(ASTNode* node) {
    NodeList* list = new NodeList();
    list->items.push_back(node);
    return list;
}

static NodeList* list_add(ASTNode* list, ASTNode* node) {
    NodeList* l = static_cast<NodeList*>(list);
    l->items.push_back(node);
    return l;
}

template <class T>
static std::vector<T*> take(ASTNode* list) {
    NodeList* l = static_cast<NodeList*>(list);
    std::vector<T*> result;
    result.reserve(l->items.size());
    for (ASTNode* item : l->items) {
        result.push_back(static_cast<T*>(item));
    }
    delete l;
    return result;
}

static std::string take_name(ASTNode* identifier) {
    Reference* ref = static_cast<Reference*>(identifier);
    std::string name = ref->identifier;
    delete ref;
    return name;
}

static std::vector<std::string> take_names(ASTNode* list) {
    NodeList* l = static_cast<NodeList*>(list);
    std::vector<std::string> result;
    for (ASTNode* item : l->items) {
        result.push_back(take_name(item));
    }
    delete l;
    return result;
}

static Expression* expr(ASTNode* node) {
    return static_cast<Expression*>(node);
}

static Reference* add_suffix(ASTNode* ref, ReferenceSuffix* suffix) {
    Reference* r = static_cast<Reference*>(ref);
    r->suffixes.push_back(suffix);
    return r;
}

static BinaryExpression* binary(ASTNode* left, BinaryOperator op, ASTNode* right) {
    return new BinaryExpression(expr(left), expr(right), op);
}

static UnaryExpression* unary(UnaryOperator op, ASTNode* operand) {
    return new UnaryExpression(expr(operand), op);
}

#line 155 "parser/parser.cpp"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "parser.hpp"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_VAR = 3,                        /* VAR  */
  YYSYMBOL_IS = 4,                         /* IS  */
  YYSYMBOL_IF = 5,                         /* IF  */
  YYSYMBOL_THEN = 6,                       /* THEN  */
  YYSYMBOL_ELSE = 7,                       /* ELSE  */
  YYSYMBOL_FOR = 8,                        /* FOR  */
  YYSYMBOL_WHILE = 9,                      /* WHILE  */
  YYSYMBOL_IN = 10,                        /* IN  */
  YYSYMBOL_LOOP = 11,                      /* LOOP  */
  YYSYMBOL_EXIT = 12,                      /* EXIT  */
  YYSYMBOL_OR = 13,                        /* OR  */
  YYSYMBOL_AND = 14,                       /* AND  */
  YYSYMBOL_XOR = 15,                       /* XOR  */
  YYSYMBOL_NOT = 16,                       /* NOT  */
  YYSYMBOL_PRINT = 17,                     /* PRINT  */
  YYSYMBOL_RETURN = 18,                    /* RETURN  */
  YYSYMBOL_END = 19,                       /* END  */
  YYSYMBOL_INT = 20,                       /* INT  */
  YYSYMBOL_REAL = 21,                      /* REAL  */
  YYSYMBOL_BOOL = 22,                      /* BOOL  */
  YYSYMBOL_STRING = 23,                    /* STRING  */
  YYSYMBOL_FUNC = 24,                      /* FUNC  */
  YYSYMBOL_PLUS = 25,                      /* PLUS  */
  YYSYMBOL_MINUS = 26,                     /* MINUS  */
  YYSYMBOL_MUL = 27,                       /* MUL  */
  YYSYMBOL_DIV = 28,                       /* DIV  */
  YYSYMBOL_LT = 29,                        /* LT  */
  YYSYMBOL_LE = 30,                        /* LE  */
  YYSYMBOL_GT = 31,                        /* GT  */
  YYSYMBOL_GE = 32,                        /* GE  */
  YYSYMBOL_EQ = 33,                        /* EQ  */
  YYSYMBOL_NEQ = 34,                       /* NEQ  */
  YYSYMBOL_ASSIGN = 35,                    /* ASSIGN  */
  YYSYMBOL_ARROW = 36,                     /* ARROW  */
  YYSYMBOL_RANGE = 37,                     /* RANGE  */
  YYSYMBOL_OPEN_ROUND_BRACKET = 38,        /* OPEN_ROUND_BRACKET  */
  YYSYMBOL_CLOSE_ROUND_BRACKET = 39,       /* CLOSE_ROUND_BRACKET  */
  YYSYMBOL_OPEN_SQUARE_BRACKET = 40,       /* OPEN_SQUARE_BRACKET  */
  YYSYMBOL_CLOSE_SQUARE_BRACKET = 41,      /* CLOSE_SQUARE_BRACKET  */
  YYSYMBOL_OPEN_BRACE = 42,                /* OPEN_BRACE  */
  YYSYMBOL_CLOSE_BRACE = 43,               /* CLOSE_BRACE  */
  YYSYMBOL_COMMA = 44,                     /* COMMA  */
  YYSYMBOL_DOT = 45,                       /* DOT  */
  YYSYMBOL_IDENTIFIER = 46,                /* IDENTIFIER  */
  YYSYMBOL_SEMICOLON = 47,                 /* SEMICOLON  */
  YYSYMBOL_NEWLINE = 48,                   /* NEWLINE  */
  YYSYMBOL_INT_LITERAL = 49,               /* INT_LITERAL  */
  YYSYMBOL_REAL_LITERAL = 50,              /* REAL_LITERAL  */
  YYSYMBOL_STRING_LITERAL = 51,            /* STRING_LITERAL  */
  YYSYMBOL_TRUE = 52,                      /* TRUE  */
  YYSYMBOL_FALSE = 53,                     /* FALSE  */
  YYSYMBOL_NONE = 54,                      /* NONE  */
  YYSYMBOL_FUNC_BODY = 55,                 /* FUNC_BODY  */
  YYSYMBOL_UNARY = 56,                     /* UNARY  */
  YYSYMBOL_YYACCEPT = 57,                  /* $accept  */
  YYSYMBOL_program = 58,                   /* program  */
  YYSYMBOL_body = 59,                      /* body  */
  YYSYMBOL_statement_list = 60,            /* statement_list  */
  YYSYMBOL_separator = 61,                 /* separator  */
  YYSYMBOL_seps = 62,                      /* seps  */
  YYSYMBOL_opt_seps = 63,                  /* opt_seps  */
  YYSYMBOL_statement = 64,                 /* statement  */
  YYSYMBOL_declaration = 65,               /* declaration  */
  YYSYMBOL_variable_definition_list = 66,  /* variable_definition_list  */
  YYSYMBOL_variable_definition = 67,       /* variable_definition  */
  YYSYMBOL_assignment = 68,                /* assignment  */
  YYSYMBOL_print_statement = 69,           /* print_statement  */
  YYSYMBOL_return_statement = 70,          /* return_statement  */
  YYSYMBOL_exit_statement = 71,            /* exit_statement  */
  YYSYMBOL_if_statement = 72,              /* if_statement  */
  YYSYMBOL_loop_statement = 73,            /* loop_statement  */
  YYSYMBOL_expression = 74,                /* expression  */
  YYSYMBOL_reference = 75,                 /* reference  */
  YYSYMBOL_argument_list = 76,             /* argument_list  */
  YYSYMBOL_opt_argument_list = 77,         /* opt_argument_list  */
  YYSYMBOL_type_indicator = 78,            /* type_indicator  */
  YYSYMBOL_literal = 79,                   /* literal  */
  YYSYMBOL_array_literal = 80,             /* array_literal  */
  YYSYMBOL_tuple_literal = 81,             /* tuple_literal  */
  YYSYMBOL_tuple_element_list = 82,        /* tuple_element_list  */
  YYSYMBOL_opt_tuple_element_list = 83,    /* opt_tuple_element_list  */
  YYSYMBOL_tuple_element = 84,             /* tuple_element  */
  YYSYMBOL_function_literal = 85,          /* function_literal  */
  YYSYMBOL_function_params = 86,           /* function_params  */
  YYSYMBOL_parameter_list = 87,            /* parameter_list  */
  YYSYMBOL_function_body = 88,             /* function_body  */
  YYSYMBOL_opt_newlines = 89               /* opt_newlines  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if 1

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* 1 */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  8
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   418

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  57
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  33
/* YYNRULES -- Number of rules.  */
#define YYNRULES  102
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  180

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   311


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   134,   134,   141,   144,   150,   153,   159,   160,   164,
     165,   169,   170,   174,   175,   176,   177,   178,   179,   180,
     184,   190,   193,   199,   202,   208,   214,   220,   223,   229,
     235,   238,   241,   247,   250,   253,   256,   259,   262,   268,
     269,   270,   271,   272,   273,   274,   275,   276,   277,   278,
     279,   280,   281,   282,   283,   284,   287,   290,   291,   295,
     296,   299,   302,   305,   311,   314,   320,   321,   325,   326,
     327,   328,   329,   330,   331,   332,   336,   337,   338,   339,
     340,   341,   342,   343,   344,   348,   354,   360,   363,   369,
     370,   374,   377,   383,   389,   390,   393,   399,   402,   408,
     411,   417,   418
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if 1
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  static const char *const yy_sname[] =
  {
  "end of file", "error", "invalid token", "VAR", "IS", "IF", "THEN",
  "ELSE", "FOR", "WHILE", "IN", "LOOP", "EXIT", "OR", "AND", "XOR", "NOT",
  "PRINT", "RETURN", "END", "INT", "REAL", "BOOL", "STRING", "FUNC",
  "PLUS", "MINUS", "MUL", "DIV", "LT", "LE", "GT", "GE", "EQ", "NEQ",
  "ASSIGN", "ARROW", "RANGE", "OPEN_ROUND_BRACKET", "CLOSE_ROUND_BRACKET",
  "OPEN_SQUARE_BRACKET", "CLOSE_SQUARE_BRACKET", "OPEN_BRACE",
  "CLOSE_BRACE", "COMMA", "DOT", "IDENTIFIER", "SEMICOLON", "NEWLINE",
  "INT_LITERAL", "REAL_LITERAL", "STRING_LITERAL", "TRUE", "FALSE", "NONE",
  "FUNC_BODY", "UNARY", "$accept", "program", "body", "statement_list",
  "separator", "seps", "opt_seps", "statement", "declaration",
  "variable_definition_list", "variable_definition", "assignment",
  "print_statement", "return_statement", "exit_statement", "if_statement",
  "loop_statement", "expression", "reference", "argument_list",
  "opt_argument_list", "type_indicator", "literal", "array_literal",
  "tuple_literal", "tuple_element_list", "opt_tuple_element_list",
  "tuple_element", "function_literal", "function_params", "parameter_list",
  "function_body", "opt_newlines", YY_NULLPTR
  };
  return yy_sname[yysymbol];
}
#endif

#define YYPACT_NINF (-55)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  ((Yyn) == YYTABLE_NINF)

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int16 yypact[] =
{
       7,   -55,   -55,    13,   -55,   -55,     7,   104,   -55,   -55,
     -41,   103,   142,   103,     7,   -55,   103,   103,   -55,     7,
     -55,   -55,   -55,   -55,   -55,   -55,   -55,   -55,    97,   -28,
     -35,   -55,   103,    -6,   103,   103,   103,   103,   161,   -55,
     -55,   -55,   -55,   -55,   -55,   191,    18,   -55,   -55,   -55,
     -55,    23,   239,   316,    24,    47,    -7,    47,     3,   -55,
     103,   103,   103,   -15,   103,   -41,   -55,   -36,    12,   -55,
     -55,   289,    -7,    11,    22,    47,    20,    26,   -55,     7,
     103,   103,   103,   103,   103,   103,   103,   103,   103,   103,
     103,   103,   103,   104,     5,   103,     7,   103,     7,   -55,
     103,   -55,    47,    32,   215,   -55,   -55,    47,   -55,   -55,
     -55,    -9,   103,   -55,    -2,   -55,   -55,   103,   161,   -55,
      17,   374,   374,   374,    72,    72,   -55,   -55,   384,   384,
     384,   384,   384,   384,   -55,   -55,   -55,   -55,   -55,   -55,
      42,    61,   -55,   -55,   264,    66,   340,    86,    47,   -55,
     -55,   -55,    62,    47,     7,   -55,    47,   -55,     7,   -55,
     -55,   -55,     7,   103,   -55,     7,   -55,   -55,    91,    92,
      95,   364,    98,   -55,   -55,   -55,     7,   -55,   101,   -55
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
      11,     8,     7,     0,     2,     9,    12,     3,     1,    10,
       0,     0,     0,     0,    11,    29,     0,    27,    59,    11,
       5,    13,    14,    15,    16,    17,    18,    19,     0,    23,
      20,    21,     0,    94,     0,     0,     0,    66,    89,    76,
      77,    78,    79,    80,    81,     0,    57,    58,    82,    83,
      84,    59,     0,     0,     0,    64,    26,    28,    12,     4,
       0,    66,     0,     0,     0,     0,    52,     0,   101,    53,
      54,     0,    67,     0,    59,    91,    90,     0,    87,    11,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,    11,     0,    11,    34,
       0,     6,    25,     0,     0,    62,    63,    24,    22,    95,
      97,     0,     0,    93,     0,    55,    85,     0,     0,    86,
       0,    39,    40,    41,    48,    49,    50,    51,    44,    45,
      46,    47,    42,    43,    32,    68,    69,    70,    71,    73,
       0,     0,    72,    56,     0,     0,     0,     0,    65,    61,
      60,    96,     0,   100,    11,   102,    92,    88,    11,    30,
      74,    75,    11,     0,    37,    11,    33,    98,     0,     0,
       0,     0,     0,    99,    31,    35,    11,    38,     0,    36
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -55,   -55,   -14,   -55,    -5,   106,   107,   -54,   -55,   -55,
      65,   -55,   -55,   -55,   -55,   -55,   -55,     6,     9,   115,
      73,   -55,   -55,   -55,   -55,   -55,   -55,    15,   -55,   -55,
     -55,   -55,   -55
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     3,     4,    19,     5,     6,     7,    20,    21,    30,
      31,    22,    23,    24,    25,    26,    27,    55,    46,    72,
      73,   143,    47,    48,    49,    76,    77,    78,    50,    68,
     111,   113,   114
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_int16 yytable[] =
{
      54,     9,   154,   109,   101,    29,    10,    64,    11,    65,
     110,    12,    13,     8,    14,    15,    28,    45,    52,    53,
      16,    17,    94,    57,   158,   135,   136,   137,   138,   139,
     151,   105,    67,    95,   106,   152,   159,   100,    66,   134,
      69,    70,    71,    99,    75,   140,   155,   141,   112,    18,
       1,     2,   116,     9,     1,     2,    61,   117,    62,   142,
      80,    81,    82,    63,   118,   120,   102,    28,   104,   119,
     107,   149,    83,    84,    85,    86,    87,    88,    89,    90,
      91,    92,   145,   160,   147,   164,   121,   122,   123,   124,
     125,   126,   127,   128,   129,   130,   131,   132,   133,    85,
      86,   144,    28,   146,   161,   166,   148,    10,   167,    11,
     173,   174,    12,    13,   175,    14,    15,   177,   153,    32,
     179,    16,    17,   156,    75,    58,    59,    33,    34,    35,
     108,    56,    60,   157,   103,    61,     0,    62,     0,     0,
     168,    36,    63,    37,   169,    38,     0,     0,   170,    18,
      18,   172,    39,    40,    41,    42,    43,    44,    32,     0,
       0,     0,   178,     0,     0,     0,    33,    34,    35,   171,
       0,     0,     0,     0,     0,     0,     0,    32,     0,     0,
      36,     0,    37,     0,    38,    33,    34,    35,    51,     0,
       0,    39,    40,    41,    42,    43,    44,    79,     0,    36,
       0,    37,     0,    38,    80,    81,    82,    74,     0,     0,
      39,    40,    41,    42,    43,    44,    83,    84,    85,    86,
      87,    88,    89,    90,    91,    92,     0,    93,    80,    81,
      82,     0,     0,     0,     0,     0,     0,     0,     0,     0,
      83,    84,    85,    86,    87,    88,    89,    90,    91,    92,
      96,     0,    80,    81,    82,     0,   150,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,     0,   162,    97,    80,    81,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,     0,
       0,   163,    80,    81,    82,     0,     0,     0,     0,     0,
       0,     0,     0,     0,    83,    84,    85,    86,    87,    88,
      89,    90,    91,    92,     0,     0,     0,    98,   115,    80,
      81,    82,     0,     0,     0,     0,     0,     0,     0,     0,
       0,    83,    84,    85,    86,    87,    88,    89,    90,    91,
      92,   165,     0,    80,    81,    82,     0,     0,     0,     0,
       0,     0,     0,     0,     0,    83,    84,    85,    86,    87,
      88,    89,    90,    91,    92,   176,     0,    80,    81,    82,
       0,     0,     0,     0,     0,     0,     0,     0,     0,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    83,
      84,    85,    86,    -1,    -1,    -1,    -1,    -1,    -1
};

static const yytype_int16 yycheck[] =
{
      14,     6,     4,    39,    58,    46,     3,    35,     5,    44,
      46,     8,     9,     0,    11,    12,     7,    11,    12,    13,
      17,    18,     4,    17,     7,    20,    21,    22,    23,    24,
      39,    46,    38,    10,    49,    44,    19,    44,    32,    93,
      34,    35,    36,    19,    38,    40,    48,    42,    36,    46,
      47,    48,    41,    58,    47,    48,    38,    35,    40,    54,
      13,    14,    15,    45,    44,    79,    60,    58,    62,    43,
      64,    39,    25,    26,    27,    28,    29,    30,    31,    32,
      33,    34,    96,    41,    98,    19,    80,    81,    82,    83,
      84,    85,    86,    87,    88,    89,    90,    91,    92,    27,
      28,    95,    93,    97,    43,    19,   100,     3,    46,     5,
      19,    19,     8,     9,    19,    11,    12,    19,   112,    16,
      19,    17,    18,   117,   118,    19,    19,    24,    25,    26,
      65,    16,    35,   118,    61,    38,    -1,    40,    -1,    -1,
     154,    38,    45,    40,   158,    42,    -1,    -1,   162,    46,
      46,   165,    49,    50,    51,    52,    53,    54,    16,    -1,
      -1,    -1,   176,    -1,    -1,    -1,    24,    25,    26,   163,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    16,    -1,    -1,
      38,    -1,    40,    -1,    42,    24,    25,    26,    46,    -1,
      -1,    49,    50,    51,    52,    53,    54,     6,    -1,    38,
      -1,    40,    -1,    42,    13,    14,    15,    46,    -1,    -1,
      49,    50,    51,    52,    53,    54,    25,    26,    27,    28,
      29,    30,    31,    32,    33,    34,    -1,    36,    13,    14,
      15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      11,    -1,    13,    14,    15,    -1,    41,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    11,    37,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    -1,
      -1,    37,    13,    14,    15,    -1,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,    30,
      31,    32,    33,    34,    -1,    -1,    -1,    11,    39,    13,
      14,    15,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,
      -1,    25,    26,    27,    28,    29,    30,    31,    32,    33,
      34,    11,    -1,    13,    14,    15,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,    25,    26,    27,    28,    29,
      30,    31,    32,    33,    34,    11,    -1,    13,    14,    15,
      -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    -1,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34,    25,
      26,    27,    28,    29,    30,    31,    32,    33,    34
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,    47,    48,    58,    59,    61,    62,    63,     0,    61,
       3,     5,     8,     9,    11,    12,    17,    18,    46,    60,
      64,    65,    68,    69,    70,    71,    72,    73,    75,    46,
      66,    67,    16,    24,    25,    26,    38,    40,    42,    49,
      50,    51,    52,    53,    54,    74,    75,    79,    80,    81,
      85,    46,    74,    74,    59,    74,    76,    74,    62,    63,
      35,    38,    40,    45,    35,    44,    74,    38,    86,    74,
      74,    74,    76,    77,    46,    74,    82,    83,    84,     6,
      13,    14,    15,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    36,     4,    10,    11,    37,    11,    19,
      44,    64,    74,    77,    74,    46,    49,    74,    67,    39,
      46,    87,    36,    88,    89,    39,    41,    35,    44,    43,
      59,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    64,    20,    21,    22,    23,    24,
      40,    42,    54,    78,    74,    59,    74,    59,    74,    39,
      41,    39,    44,    74,     4,    48,    74,    84,     7,    19,
      41,    43,    11,    37,    19,    11,    19,    46,    59,    59,
      59,    74,    59,    19,    19,    19,    11,    19,    59,    19
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    57,    58,    59,    59,    60,    60,    61,    61,    62,
      62,    63,    63,    64,    64,    64,    64,    64,    64,    64,
      65,    66,    66,    67,    67,    68,    69,    70,    70,    71,
      72,    72,    72,    73,    73,    73,    73,    73,    73,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    74,
      74,    74,    74,    74,    74,    74,    74,    74,    74,    75,
      75,    75,    75,    75,    76,    76,    77,    77,    78,    78,
      78,    78,    78,    78,    78,    78,    79,    79,    79,    79,
      79,    79,    79,    79,    79,    80,    81,    82,    82,    83,
      83,    84,    84,    85,    86,    86,    86,    87,    87,    88,
      88,    89,    89
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     1,     1,     3,     1,     3,     1,     1,     1,
       2,     0,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     1,     3,     1,     3,     3,     2,     1,     2,     1,
       5,     7,     4,     5,     3,     7,     9,     5,     7,     3,
       3,     3,     3,     3,     3,     3,     3,     3,     3,     3,
       3,     3,     2,     2,     2,     3,     3,     1,     1,     1,
       4,     4,     3,     3,     1,     3,     0,     1,     1,     1,
       1,     1,     1,     1,     2,     2,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     3,     3,     1,     3,     0,
       1,     1,     3,     3,     0,     2,     3,     1,     3,     4,
       2,     0,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (root, error, YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location, root, error); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Program*& root, std::string& error)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  YY_USE (root);
  YY_USE (error);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp, Program*& root, std::string& error)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp, root, error);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule, Program*& root, std::string& error)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]), root, error);
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule, root, error); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif


/* Context of a parse error.  */
typedef struct
{
  yy_state_t *yyssp;
  yysymbol_kind_t yytoken;
  YYLTYPE *yylloc;
} yypcontext_t;

/* Put in YYARG at most YYARGN of the expected tokens given the
   current YYCTX, and return the number of tokens stored in YYARG.  If
   YYARG is null, return the number of expected tokens (guaranteed to
   be less than YYNTOKENS).  Return YYENOMEM on memory exhaustion.
   Return 0 if there are more than YYARGN expected tokens, yet fill
   YYARG up to YYARGN. */
static int
yypcontext_expected_tokens (const yypcontext_t *yyctx,
                            yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  int yyn = yypact[+*yyctx->yyssp];
  if (!yypact_value_is_default (yyn))
    {
      /* Start YYX at -YYN if negative to avoid negative indexes in
         YYCHECK.  In other words, skip the first -YYN actions for
         this state because they are default actions.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;
      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yyx;
      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
        if (yycheck[yyx + yyn] == yyx && yyx != YYSYMBOL_YYerror
            && !yytable_value_is_error (yytable[yyx + yyn]))
          {
            if (!yyarg)
              ++yycount;
            else if (yycount == yyargn)
              return 0;
            else
              yyarg[yycount++] = YY_CAST (yysymbol_kind_t, yyx);
          }
    }
  if (yyarg && yycount == 0 && 0 < yyargn)
    yyarg[0] = YYSYMBOL_YYEMPTY;
  return yycount;
}




#ifndef yystrlen
# if defined __GLIBC__ && defined _STRING_H
#  define yystrlen(S) (YY_CAST (YYPTRDIFF_T, strlen (S)))
# else
/* Return the length of YYSTR.  */
static YYPTRDIFF_T
yystrlen (const char *yystr)
{
  YYPTRDIFF_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
# endif
#endif

#ifndef yystpcpy
# if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#  define yystpcpy stpcpy
# else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
static char *
yystpcpy (char *yydest, const char *yysrc)
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
# endif
#endif



static int
yy_syntax_error_arguments (const yypcontext_t *yyctx,
                           yysymbol_kind_t yyarg[], int yyargn)
{
  /* Actual size of YYARG. */
  int yycount = 0;
  /* There are many possibilities here to consider:
     - If this state is a consistent state with a default action, then
       the only way this function was invoked is if the default action
       is an error action.  In that case, don't check for expected
       tokens because there are none.
     - The only way there can be no lookahead present (in yychar) is if
       this state is a consistent state with a default action.  Thus,
       detecting the absence of a lookahead is sufficient to determine
       that there is no unexpected or expected token to report.  In that
       case, just report a simple "syntax error".
     - Don't assume there isn't a lookahead just because this state is a
       consistent state with a default action.  There might have been a
       previous inconsistent state, consistent state with a non-default
       action, or user semantic action that manipulated yychar.
     - Of course, the expected token list depends on states to have
       correct lookahead information, and it depends on the parser not
       to perform extra reductions after fetching a lookahead from the
       scanner and before detecting a syntax error.  Thus, state merging
       (from LALR or IELR) and default reductions corrupt the expected
       token list.  However, the list is correct for canonical LR with
       one exception: it will still contain any token that will not be
       accepted due to an error action in a later state.
  */
  if (yyctx->yytoken != YYSYMBOL_YYEMPTY)
    {
      int yyn;
      if (yyarg)
        yyarg[yycount] = yyctx->yytoken;
      ++yycount;
      yyn = yypcontext_expected_tokens (yyctx,
                                        yyarg ? yyarg + 1 : yyarg, yyargn - 1);
      if (yyn == YYENOMEM)
        return YYENOMEM;
      else
        yycount += yyn;
    }
  return yycount;
}

/* Copy into *YYMSG, which is of size *YYMSG_ALLOC, an error message
   about the unexpected token YYTOKEN for the state stack whose top is
   YYSSP.

   Return 0 if *YYMSG was successfully written.  Return -1 if *YYMSG is
   not large enough to hold the message.  In that case, also set
   *YYMSG_ALLOC to the required number of bytes.  Return YYENOMEM if the
   required number of bytes is too large to store.  */
static int
yysyntax_error (YYPTRDIFF_T *yymsg_alloc, char **yymsg,
                const yypcontext_t *yyctx)
{
  enum { YYARGS_MAX = 5 };
  /* Internationalized format string. */
  const char *yyformat = YY_NULLPTR;
  /* Arguments of yyformat: reported tokens (one for the "unexpected",
     one per "expected"). */
  yysymbol_kind_t yyarg[YYARGS_MAX];
  /* Cumulated lengths of YYARG.  */
  YYPTRDIFF_T yysize = 0;

  /* Actual size of YYARG. */
  int yycount = yy_syntax_error_arguments (yyctx, yyarg, YYARGS_MAX);
  if (yycount == YYENOMEM)
    return YYENOMEM;

  switch (yycount)
    {
#define YYCASE_(N, S)                       \
      case N:                               \
        yyformat = S;                       \
        break
    default: /* Avoid compiler warnings. */
      YYCASE_(0, YY_("syntax error"));
      YYCASE_(1, YY_("syntax error, unexpected %s"));
      YYCASE_(2, YY_("syntax error, unexpected %s, expecting %s"));
      YYCASE_(3, YY_("syntax error, unexpected %s, expecting %s or %s"));
      YYCASE_(4, YY_("syntax error, unexpected %s, expecting %s or %s or %s"));
      YYCASE_(5, YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s"));
#undef YYCASE_
    }

  /* Compute error message size.  Don't count the "%s"s, but reserve
     room for the terminator.  */
  yysize = yystrlen (yyformat) - 2 * yycount + 1;
  {
    int yyi;
    for (yyi = 0; yyi < yycount; ++yyi)
      {
        YYPTRDIFF_T yysize1
          = yysize + yystrlen (yysymbol_name (yyarg[yyi]));
        if (yysize <= yysize1 && yysize1 <= YYSTACK_ALLOC_MAXIMUM)
          yysize = yysize1;
        else
          return YYENOMEM;
      }
  }

  if (*yymsg_alloc < yysize)
    {
      *yymsg_alloc = 2 * yysize;
      if (! (yysize <= *yymsg_alloc
             && *yymsg_alloc <= YYSTACK_ALLOC_MAXIMUM))
        *yymsg_alloc = YYSTACK_ALLOC_MAXIMUM;
      return -1;
    }

  /* Avoid sprintf, as that infringes on the user's name space.
     Don't have undefined behavior even if the translation
     produced a string with the wrong number of "%s"s.  */
  {
    char *yyp = *yymsg;
    int yyi = 0;
    while ((*yyp = *yyformat) != '\0')
      if (*yyp == '%' && yyformat[1] == 's' && yyi < yycount)
        {
          yyp = yystpcpy (yyp, yysymbol_name (yyarg[yyi++]));
          yyformat += 2;
        }
      else
        {
          ++yyp;
          ++yyformat;
        }
  }
  return 0;
}


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp, Program*& root, std::string& error)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  YY_USE (root);
  YY_USE (error);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (Program*& root, std::string& error)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];

  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYPTRDIFF_T yymsg_alloc = sizeof yymsgbuf;

#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* program: body  */
#line 134 "parser/parser.y"
         {
        root = new Program(take<Statement>(yyvsp[0]));
        yyval = root;
    }
#line 1749 "parser/parser.cpp"
    break;

  case 3: /* body: opt_seps  */
#line 141 "parser/parser.y"
             {
        yyval = new_list();
    }
#line 1757 "parser/parser.cpp"
    break;

  case 4: /* body: opt_seps statement_list opt_seps  */
#line 144 "parser/parser.y"
                                       {
        yyval = yyvsp[-1];
    }
#line 1765 "parser/parser.cpp"
    break;

  case 5: /* statement_list: statement  */
#line 150 "parser/parser.y"
              {
        yyval = list_of(yyvsp[0]);
    }
#line 1773 "parser/parser.cpp"
    break;

  case 6: /* statement_list: statement_list seps statement  */
#line 153 "parser/parser.y"
                                    {
        yyval = list_add(yyvsp[-2], yyvsp[0]);
    }
#line 1781 "parser/parser.cpp"
    break;

  case 11: /* opt_seps: %empty  */
#line 169 "parser/parser.y"
           { yyval = nullptr; }
#line 1787 "parser/parser.cpp"
    break;

  case 20: /* declaration: VAR variable_definition_list  */
#line 184 "parser/parser.y"
                                 {
        yyval = new Declaration(take<VariableDefinition>(yyvsp[0]));
    }
#line 1795 "parser/parser.cpp"
    break;

  case 21: /* variable_definition_list: variable_definition  */
#line 190 "parser/parser.y"
                        {
        yyval = list_of(yyvsp[0]);
    }
#line 1803 "parser/parser.cpp"
    break;

  case 22: /* variable_definition_list: variable_definition_list COMMA variable_definition  */
#line 193 "parser/parser.y"
                                                         {
        yyval = list_add(yyvsp[-2], yyvsp[0]);
    }
#line 1811 "parser/parser.cpp"
    break;

  case 23: /* variable_definition: IDENTIFIER  */
#line 199 "parser/parser.y"
               {
        yyval = new VariableDefinition(take_name(yyvsp[0]));
    }
#line 1819 "parser/parser.cpp"
    break;

  case 24: /* variable_definition: IDENTIFIER ASSIGN expression  */
#line 202 "parser/parser.y"
                                   {
        yyval = new VariableDefinition(take_name(yyvsp[-2]), expr(yyvsp[0]));
    }
#line 1827 "parser/parser.cpp"
    break;

  case 25: /* assignment: reference ASSIGN expression  */
#line 208 "parser/parser.y"
                                {
        yyval = new Assignment(static_cast<Reference*>(yyvsp[-2]), expr(yyvsp[0]));
    }
#line 1835 "parser/parser.cpp"
    break;

  case 26: /* print_statement: PRINT argument_list  */
#line 214 "parser/parser.y"
                        {
        yyval = new Print(take<Expression>(yyvsp[0]));
    }
#line 1843 "parser/parser.cpp"
    break;

  case 27: /* return_statement: RETURN  */
#line 220 "parser/parser.y"
           {
        yyval = new Return();
    }
#line 1851 "parser/parser.cpp"
    break;

  case 28: /* return_statement: RETURN expression  */
#line 223 "parser/parser.y"
                        {
        yyval = new Return(expr(yyvsp[0]));
    }
#line 1859 "parser/parser.cpp"
    break;

  case 29: /* exit_statement: EXIT  */
#line 229 "parser/parser.y"
         {
        yyval = new Exit();
    }
#line 1867 "parser/parser.cpp"
    break;

  case 30: /* if_statement: IF expression THEN body END  */
#line 235 "parser/parser.y"
                                {
        yyval = new If(expr(yyvsp[-3]), take<Statement>(yyvsp[-1]));
    }
#line 1875 "parser/parser.cpp"
    break;

  case 31: /* if_statement: IF expression THEN body ELSE body END  */
#line 238 "parser/parser.y"
                                            {
        yyval = new If(expr(yyvsp[-5]), take<Statement>(yyvsp[-3]), take<Statement>(yyvsp[-1]));
    }
#line 1883 "parser/parser.cpp"
    break;

  case 32: /* if_statement: IF expression ARROW statement  */
#line 241 "parser/parser.y"
                                    {
        yyval = new IfShort(expr(yyvsp[-2]), std::vector<Statement*>{ static_cast<Statement*>(yyvsp[0]) });
    }
#line 1891 "parser/parser.cpp"
    break;

  case 33: /* loop_statement: WHILE expression LOOP body END  */
#line 247 "parser/parser.y"
                                   {
        yyval = new WhileLoop(expr(yyvsp[-3]), take<Statement>(yyvsp[-1]));
    }
#line 1899 "parser/parser.cpp"
    break;

  case 34: /* loop_statement: LOOP body END  */
#line 250 "parser/parser.y"
                    {
        yyval = new InfiniteLoop(take<Statement>(yyvsp[-1]));
    }
#line 1907 "parser/parser.cpp"
    break;

  case 35: /* loop_statement: FOR IDENTIFIER IN expression LOOP body END  */
#line 253 "parser/parser.y"
                                                 {
        yyval = new ForEachLoop(expr(yyvsp[-3]), take<Statement>(yyvsp[-1]), take_name(yyvsp[-5]));
    }
#line 1915 "parser/parser.cpp"
    break;

  case 36: /* loop_statement: FOR IDENTIFIER IN expression RANGE expression LOOP body END  */
#line 256 "parser/parser.y"
                                                                  {
        yyval = new ForRangeLoop(expr(yyvsp[-5]), expr(yyvsp[-3]), take<Statement>(yyvsp[-1]), take_name(yyvsp[-7]));
    }
#line 1923 "parser/parser.cpp"
    break;

  case 37: /* loop_statement: FOR expression LOOP body END  */
#line 259 "parser/parser.y"
                                   {
        yyval = new ForEachLoop(expr(yyvsp[-3]), take<Statement>(yyvsp[-1]));
    }
#line 1931 "parser/parser.cpp"
    break;

  case 38: /* loop_statement: FOR expression RANGE expression LOOP body END  */
#line 262 "parser/parser.y"
                                                    {
        yyval = new ForRangeLoop(expr(yyvsp[-5]), expr(yyvsp[-3]), take<Statement>(yyvsp[-1]));
    }
#line 1939 "parser/parser.cpp"
    break;

  case 39: /* expression: expression OR expression  */
#line 268 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::OR,  yyvsp[0]); }
#line 1945 "parser/parser.cpp"
    break;

  case 40: /* expression: expression AND expression  */
#line 269 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::AND, yyvsp[0]); }
#line 1951 "parser/parser.cpp"
    break;

  case 41: /* expression: expression XOR expression  */
#line 270 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::XOR, yyvsp[0]); }
#line 1957 "parser/parser.cpp"
    break;

  case 42: /* expression: expression EQ expression  */
#line 271 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::EQ,  yyvsp[0]); }
#line 1963 "parser/parser.cpp"
    break;

  case 43: /* expression: expression NEQ expression  */
#line 272 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::NEQ, yyvsp[0]); }
#line 1969 "parser/parser.cpp"
    break;

  case 44: /* expression: expression LT expression  */
#line 273 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::LT,  yyvsp[0]); }
#line 1975 "parser/parser.cpp"
    break;

  case 45: /* expression: expression LE expression  */
#line 274 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::LE,  yyvsp[0]); }
#line 1981 "parser/parser.cpp"
    break;

  case 46: /* expression: expression GT expression  */
#line 275 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::GT,  yyvsp[0]); }
#line 1987 "parser/parser.cpp"
    break;

  case 47: /* expression: expression GE expression  */
#line 276 "parser/parser.y"
                                { yyval = binary(yyvsp[-2], BinaryOperator::GE,  yyvsp[0]); }
#line 1993 "parser/parser.cpp"
    break;

  case 48: /* expression: expression PLUS expression  */
#line 277 "parser/parser.y"
                                  { yyval = binary(yyvsp[-2], BinaryOperator::ADD, yyvsp[0]); }
#line 1999 "parser/parser.cpp"
    break;

  case 49: /* expression: expression MINUS expression  */
#line 278 "parser/parser.y"
                                  { yyval = binary(yyvsp[-2], BinaryOperator::SUB, yyvsp[0]); }
#line 2005 "parser/parser.cpp"
    break;

  case 50: /* expression: expression MUL expression  */
#line 279 "parser/parser.y"
                                  { yyval = binary(yyvsp[-2], BinaryOperator::MUL, yyvsp[0]); }
#line 2011 "parser/parser.cpp"
    break;

  case 51: /* expression: expression DIV expression  */
#line 280 "parser/parser.y"
                                  { yyval = binary(yyvsp[-2], BinaryOperator::DIV, yyvsp[0]); }
#line 2017 "parser/parser.cpp"
    break;

  case 52: /* expression: NOT expression  */
#line 281 "parser/parser.y"
                                   { yyval = unary(UnaryOperator::NOT,   yyvsp[0]); }
#line 2023 "parser/parser.cpp"
    break;

  case 53: /* expression: PLUS expression  */
#line 282 "parser/parser.y"
                                   { yyval = unary(UnaryOperator::PLUS,  yyvsp[0]); }
#line 2029 "parser/parser.cpp"
    break;

  case 54: /* expression: MINUS expression  */
#line 283 "parser/parser.y"
                                   { yyval = unary(UnaryOperator::MINUS, yyvsp[0]); }
#line 2035 "parser/parser.cpp"
    break;

  case 55: /* expression: OPEN_ROUND_BRACKET expression CLOSE_ROUND_BRACKET  */
#line 284 "parser/parser.y"
                                                        {
        yyval = yyvsp[-1];
    }
#line 2043 "parser/parser.cpp"
    break;

  case 56: /* expression: reference IS type_indicator  */
#line 287 "parser/parser.y"
                                  {
        yyval = new Is(static_cast<Reference*>(yyvsp[-2]), static_cast<TypeNode*>(yyvsp[0])->type);
    }
#line 2051 "parser/parser.cpp"
    break;

  case 60: /* reference: reference OPEN_SQUARE_BRACKET expression CLOSE_SQUARE_BRACKET  */
#line 296 "parser/parser.y"
                                                                    {
        yyval = add_suffix(yyvsp[-3], new ArrayIndexSuffix(expr(yyvsp[-1])));
    }
#line 2059 "parser/parser.cpp"
    break;

  case 61: /* reference: reference OPEN_ROUND_BRACKET opt_argument_list CLOSE_ROUND_BRACKET  */
#line 299 "parser/parser.y"
                                                                         {
        yyval = add_suffix(yyvsp[-3], new CallSuffix(take<Expression>(yyvsp[-1])));
    }
#line 2067 "parser/parser.cpp"
    break;

  case 62: /* reference: reference DOT IDENTIFIER  */
#line 302 "parser/parser.y"
                               {
        yyval = add_suffix(yyvsp[-2], new MemberSuffix(take_name(yyvsp[0])));
    }
#line 2075 "parser/parser.cpp"
    break;

  case 63: /* reference: reference DOT INT_LITERAL  */
#line 305 "parser/parser.y"
                                {
        yyval = add_suffix(yyvsp[-2], new TupleIndexSuffix(static_cast<IntLiteral*>(yyvsp[0])));
    }
#line 2083 "parser/parser.cpp"
    break;

  case 64: /* argument_list: expression  */
#line 311 "parser/parser.y"
               {
        yyval = list_of(yyvsp[0]);
    }
#line 2091 "parser/parser.cpp"
    break;

  case 65: /* argument_list: argument_list COMMA expression  */
#line 314 "parser/parser.y"
                                     {
        yyval = list_add(yyvsp[-2], yyvsp[0]);
    }
#line 2099 "parser/parser.cpp"
    break;

  case 66: /* opt_argument_list: %empty  */
#line 320 "parser/parser.y"
           { yyval = new_list(); }
#line 2105 "parser/parser.cpp"
    break;

  case 68: /* type_indicator: INT  */
#line 325 "parser/parser.y"
             { yyval = new TypeNode(VariableType::INT); }
#line 2111 "parser/parser.cpp"
    break;

  case 69: /* type_indicator: REAL  */
#line 326 "parser/parser.y"
             { yyval = new TypeNode(VariableType::REAL); }
#line 2117 "parser/parser.cpp"
    break;

  case 70: /* type_indicator: BOOL  */
#line 327 "parser/parser.y"
             { yyval = new TypeNode(VariableType::BOOL); }
#line 2123 "parser/parser.cpp"
    break;

  case 71: /* type_indicator: STRING  */
#line 328 "parser/parser.y"
             { yyval = new TypeNode(VariableType::STRING); }
#line 2129 "parser/parser.cpp"
    break;

  case 72: /* type_indicator: NONE  */
#line 329 "parser/parser.y"
             { yyval = new TypeNode(VariableType::NONE); }
#line 2135 "parser/parser.cpp"
    break;

  case 73: /* type_indicator: FUNC  */
#line 330 "parser/parser.y"
             { yyval = new TypeNode(VariableType::FUNC); }
#line 2141 "parser/parser.cpp"
    break;

  case 74: /* type_indicator: OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET  */
#line 331 "parser/parser.y"
                                               { yyval = new TypeNode(VariableType::ARRAY); }
#line 2147 "parser/parser.cpp"
    break;

  case 75: /* type_indicator: OPEN_BRACE CLOSE_BRACE  */
#line 332 "parser/parser.y"
                                               { yyval = new TypeNode(VariableType::TUPLE); }
#line 2153 "parser/parser.cpp"
    break;

  case 85: /* array_literal: OPEN_SQUARE_BRACKET opt_argument_list CLOSE_SQUARE_BRACKET  */
#line 348 "parser/parser.y"
                                                               {
        yyval = new Array(take<Expression>(yyvsp[-1]));
    }
#line 2161 "parser/parser.cpp"
    break;

  case 86: /* tuple_literal: OPEN_BRACE opt_tuple_element_list CLOSE_BRACE  */
#line 354 "parser/parser.y"
                                                  {
        yyval = new Tuple(take<TupleElement>(yyvsp[-1]));
    }
#line 2169 "parser/parser.cpp"
    break;

  case 87: /* tuple_element_list: tuple_element  */
#line 360 "parser/parser.y"
                  {
        yyval = list_of(yyvsp[0]);
    }
#line 2177 "parser/parser.cpp"
    break;

  case 88: /* tuple_element_list: tuple_element_list COMMA tuple_element  */
#line 363 "parser/parser.y"
                                             {
        yyval = list_add(yyvsp[-2], yyvsp[0]);
    }
#line 2185 "parser/parser.cpp"
    break;

  case 89: /* opt_tuple_element_list: %empty  */
#line 369 "parser/parser.y"
           { yyval = new_list(); }
#line 2191 "parser/parser.cpp"
    break;

  case 91: /* tuple_element: expression  */
#line 374 "parser/parser.y"
               {
        yyval = new TupleElement(expr(yyvsp[0]));
    }
#line 2199 "parser/parser.cpp"
    break;

  case 92: /* tuple_element: IDENTIFIER ASSIGN expression  */
#line 377 "parser/parser.y"
                                   {
        yyval = new TupleElement(expr(yyvsp[0]), take_name(yyvsp[-2]));
    }
#line 2207 "parser/parser.cpp"
    break;

  case 93: /* function_literal: FUNC function_params function_body  */
#line 383 "parser/parser.y"
                                       {
        yyval = new FunctionLiteral(take_names(yyvsp[-1]), static_cast<FunctionBody*>(yyvsp[0]));
    }
#line 2215 "parser/parser.cpp"
    break;

  case 94: /* function_params: %empty  */
#line 389 "parser/parser.y"
           { yyval = new_list(); }
#line 2221 "parser/parser.cpp"
    break;

  case 95: /* function_params: OPEN_ROUND_BRACKET CLOSE_ROUND_BRACKET  */
#line 390 "parser/parser.y"
                                             {
        yyval = new_list();
    }
#line 2229 "parser/parser.cpp"
    break;

  case 96: /* function_params: OPEN_ROUND_BRACKET parameter_list CLOSE_ROUND_BRACKET  */
#line 393 "parser/parser.y"
                                                            {
        yyval = yyvsp[-1];
    }
#line 2237 "parser/parser.cpp"
    break;

  case 97: /* parameter_list: IDENTIFIER  */
#line 399 "parser/parser.y"
               {
        yyval = list_of(yyvsp[0]);
    }
#line 2245 "parser/parser.cpp"
    break;

  case 98: /* parameter_list: parameter_list COMMA IDENTIFIER  */
#line 402 "parser/parser.y"
                                      {
        yyval = list_add(yyvsp[-2], yyvsp[0]);
    }
#line 2253 "parser/parser.cpp"
    break;

  case 99: /* function_body: opt_newlines IS body END  */
#line 408 "parser/parser.y"
                             {
        yyval = new BlockBody(take<Statement>(yyvsp[-1]));
    }
#line 2261 "parser/parser.cpp"
    break;

  case 100: /* function_body: ARROW expression  */
#line 411 "parser/parser.y"
                                       {
        yyval = new ShortFunctionBody(expr(yyvsp[0]));
    }
#line 2269 "parser/parser.cpp"
    break;

  case 101: /* opt_newlines: %empty  */
#line 417 "parser/parser.y"
           { yyval = nullptr; }
#line 2275 "parser/parser.cpp"
    break;


#line 2279 "parser/parser.cpp"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      {
        yypcontext_t yyctx
          = {yyssp, yytoken, &yylloc};
        char const *yymsgp = YY_("syntax error");
        int yysyntax_error_status;
        yysyntax_error_status = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
        if (yysyntax_error_status == 0)
          yymsgp = yymsg;
        else if (yysyntax_error_status == -1)
          {
            if (yymsg != yymsgbuf)
              YYSTACK_FREE (yymsg);
            yymsg = YY_CAST (char *,
                             YYSTACK_ALLOC (YY_CAST (YYSIZE_T, yymsg_alloc)));
            if (yymsg)
              {
                yysyntax_error_status
                  = yysyntax_error (&yymsg_alloc, &yymsg, &yyctx);
                yymsgp = yymsg;
              }
            else
              {
                yymsg = yymsgbuf;
                yymsg_alloc = sizeof yymsgbuf;
                yysyntax_error_status = YYENOMEM;
              }
          }
        yyerror (root, error, yymsgp);
        if (yysyntax_error_status == YYENOMEM)
          YYNOMEM;
      }
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
         error, discard it.  */

      if (yychar <= YYEOF)
        {
          /* Return failure if at end of input.  */
          if (yychar == YYEOF)
            YYABORT;
        }
      else
        {
          yydestruct ("Error: discarding",
                      yytoken, &yylval, &yylloc, root, error);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp, root, error);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (root, error, YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc, root, error);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp, root, error);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
  return yyresult;
}

#line 421 "parser/parser.y"

