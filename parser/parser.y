%{
#include "../ast/ast.h"

#include <string>
#include <vector>

int yylex();
void yyerror(Program*& root, const char* s);

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

%precedence FUNC_BODY
%left OR AND XOR
%nonassoc LT LE GT GE EQ NEQ
%left PLUS MINUS
%left MUL DIV
%precedence UNARY

%%

program:
    body {
        root = new Program(take<Statement>($1));
        $$ = root;
    }
    ;

body:
    opt_seps {
        $$ = new_list();
    }
    | opt_seps statement_list opt_seps {
        $$ = $2;
    }
    ;

statement_list:
    statement {
        $$ = list_of($1);
    }
    | statement_list seps statement {
        $$ = list_add($1, $3);
    }
    ;

separator:
    NEWLINE
    | SEMICOLON
    ;

seps:
    separator
    | seps separator
    ;

opt_seps:
    %empty { $$ = nullptr; }
    | seps
    ;

statement:
    declaration
    | assignment
    | print_statement
    | return_statement
    | exit_statement
    | if_statement
    | loop_statement
    ;

declaration:
    VAR variable_definition_list {
        $$ = new Declaration(take<VariableDefinition>($2));
    }
    ;

variable_definition_list:
    variable_definition {
        $$ = list_of($1);
    }
    | variable_definition_list COMMA variable_definition {
        $$ = list_add($1, $3);
    }
    ;

variable_definition:
    IDENTIFIER {
        $$ = new VariableDefinition(take_name($1));
    }
    | IDENTIFIER ASSIGN expression {
        $$ = new VariableDefinition(take_name($1), expr($3));
    }
    ;

assignment:
    reference ASSIGN expression {
        $$ = new Assignment(static_cast<Reference*>($1), expr($3));
    }
    ;

print_statement:
    PRINT argument_list {
        $$ = new Print(take<Expression>($2));
    }
    ;

return_statement:
    RETURN {
        $$ = new Return();
    }
    | RETURN expression {
        $$ = new Return(expr($2));
    }
    ;

exit_statement:
    EXIT {
        $$ = new Exit();
    }
    ;

if_statement:
    IF expression THEN body END {
        $$ = new If(expr($2), take<Statement>($4));
    }
    | IF expression THEN body ELSE body END {
        $$ = new If(expr($2), take<Statement>($4), take<Statement>($6));
    }
    | IF expression ARROW statement {
        $$ = new IfShort(expr($2), std::vector<Statement*>{ static_cast<Statement*>($4) });
    }
    ;

loop_statement:
    WHILE expression LOOP body END {
        $$ = new WhileLoop(expr($2), take<Statement>($4));
    }
    | LOOP body END {
        $$ = new InfiniteLoop(take<Statement>($2));
    }
    | FOR IDENTIFIER IN expression LOOP body END {
        $$ = new ForEachLoop(expr($4), take<Statement>($6), take_name($2));
    }
    | FOR IDENTIFIER IN expression RANGE expression LOOP body END {
        $$ = new ForRangeLoop(expr($4), expr($6), take<Statement>($8), take_name($2));
    }
    | FOR expression LOOP body END {
        $$ = new ForEachLoop(expr($2), take<Statement>($4));
    }
    | FOR expression RANGE expression LOOP body END {
        $$ = new ForRangeLoop(expr($2), expr($4), take<Statement>($6));
    }
    ;

expression:
    expression OR expression    { $$ = binary($1, BinaryOperator::OR,  $3); }
    | expression AND expression { $$ = binary($1, BinaryOperator::AND, $3); }
    | expression XOR expression { $$ = binary($1, BinaryOperator::XOR, $3); }
    | expression EQ expression  { $$ = binary($1, BinaryOperator::EQ,  $3); }
    | expression NEQ expression { $$ = binary($1, BinaryOperator::NEQ, $3); }
    | expression LT expression  { $$ = binary($1, BinaryOperator::LT,  $3); }
    | expression LE expression  { $$ = binary($1, BinaryOperator::LE,  $3); }
    | expression GT expression  { $$ = binary($1, BinaryOperator::GT,  $3); }
    | expression GE expression  { $$ = binary($1, BinaryOperator::GE,  $3); }
    | expression PLUS expression  { $$ = binary($1, BinaryOperator::ADD, $3); }
    | expression MINUS expression { $$ = binary($1, BinaryOperator::SUB, $3); }
    | expression MUL expression   { $$ = binary($1, BinaryOperator::MUL, $3); }
    | expression DIV expression   { $$ = binary($1, BinaryOperator::DIV, $3); }
    | NOT expression %prec UNARY   { $$ = unary(UnaryOperator::NOT,   $2); }
    | PLUS expression %prec UNARY  { $$ = unary(UnaryOperator::PLUS,  $2); }
    | MINUS expression %prec UNARY { $$ = unary(UnaryOperator::MINUS, $2); }
    | OPEN_ROUND_BRACKET expression CLOSE_ROUND_BRACKET {
        $$ = $2;
    }
    | reference IS type_indicator {
        $$ = new Is(static_cast<Reference*>($1), static_cast<TypeNode*>($3)->type);
    }
    | reference
    | literal
    ;

reference:
    IDENTIFIER
    | reference OPEN_SQUARE_BRACKET expression CLOSE_SQUARE_BRACKET {
        $$ = add_suffix($1, new ArrayIndexSuffix(expr($3)));
    }
    | reference OPEN_ROUND_BRACKET opt_argument_list CLOSE_ROUND_BRACKET {
        $$ = add_suffix($1, new CallSuffix(take<Expression>($3)));
    }
    | reference DOT IDENTIFIER {
        $$ = add_suffix($1, new MemberSuffix(take_name($3)));
    }
    | reference DOT INT_LITERAL {
        $$ = add_suffix($1, new TupleIndexSuffix(static_cast<IntLiteral*>($3)));
    }
    ;

argument_list:
    expression {
        $$ = list_of($1);
    }
    | argument_list COMMA expression {
        $$ = list_add($1, $3);
    }
    ;

opt_argument_list:
    %empty { $$ = new_list(); }
    | argument_list
    ;

type_indicator:
    INT      { $$ = new TypeNode(VariableType::INT); }
    | REAL   { $$ = new TypeNode(VariableType::REAL); }
    | BOOL   { $$ = new TypeNode(VariableType::BOOL); }
    | STRING { $$ = new TypeNode(VariableType::STRING); }
    | NONE   { $$ = new TypeNode(VariableType::NONE); }
    | FUNC   { $$ = new TypeNode(VariableType::FUNC); }
    | OPEN_SQUARE_BRACKET CLOSE_SQUARE_BRACKET { $$ = new TypeNode(VariableType::ARRAY); }
    | OPEN_BRACE CLOSE_BRACE                   { $$ = new TypeNode(VariableType::TUPLE); }
    ;

literal:
    INT_LITERAL
    | REAL_LITERAL
    | STRING_LITERAL
    | TRUE
    | FALSE
    | NONE
    | array_literal
    | tuple_literal
    | function_literal
    ;

array_literal:
    OPEN_SQUARE_BRACKET opt_argument_list CLOSE_SQUARE_BRACKET {
        $$ = new Array(take<Expression>($2));
    }
    ;

tuple_literal:
    OPEN_BRACE opt_tuple_element_list CLOSE_BRACE {
        $$ = new Tuple(take<TupleElement>($2));
    }
    ;

tuple_element_list:
    tuple_element {
        $$ = list_of($1);
    }
    | tuple_element_list COMMA tuple_element {
        $$ = list_add($1, $3);
    }
    ;

opt_tuple_element_list:
    %empty { $$ = new_list(); }
    | tuple_element_list
    ;

tuple_element:
    expression {
        $$ = new TupleElement(expr($1));
    }
    | IDENTIFIER ASSIGN expression {
        $$ = new TupleElement(expr($3), take_name($1));
    }
    ;

function_literal:
    FUNC function_params function_body {
        $$ = new FunctionLiteral(take_names($2), static_cast<FunctionBody*>($3));
    }
    ;

function_params:
    %empty { $$ = new_list(); }
    | OPEN_ROUND_BRACKET CLOSE_ROUND_BRACKET {
        $$ = new_list();
    }
    | OPEN_ROUND_BRACKET parameter_list CLOSE_ROUND_BRACKET {
        $$ = $2;
    }
    ;

parameter_list:
    IDENTIFIER {
        $$ = list_of($1);
    }
    | parameter_list COMMA IDENTIFIER {
        $$ = list_add($1, $3);
    }
    ;

function_body:
    opt_newlines IS body END {
        $$ = new BlockBody(take<Statement>($3));
    }
    | ARROW expression %prec FUNC_BODY {
        $$ = new ShortFunctionBody(expr($2));
    }
    ;

opt_newlines:
    %empty { $$ = nullptr; }
    | opt_newlines NEWLINE
    ;

%%
