%{
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

static BinaryExpression* binary(ASTNode* left, BinaryOperator op, ASTNode* right, SourceLocation loc) {
    BinaryExpression* b = new BinaryExpression(expr(left), expr(right), op);
    b->loc = loc;
    return b;
}

static UnaryExpression* unary(UnaryOperator op, ASTNode* operand, SourceLocation loc) {
    UnaryExpression* u = new UnaryExpression(expr(operand), op);
    u->loc = loc;
    return u;
}
%}

%code requires {
    #include "../ast/ast.h"
}

%code {
    SourceLocation make_loc(const YYLTYPE& y);
}

%parse-param { Program*& root }
%parse-param { std::string& error }

%locations
%define parse.error detailed

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
        root->loc = make_loc(@1);
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
        Declaration* decl = new Declaration(take<VariableDefinition>($2));
        decl->loc = make_loc(@1);
        $$ = decl;
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
        VariableDefinition* vd = new VariableDefinition(take_name($1));
        vd->loc = make_loc(@1);
        $$ = vd;
    }
    | IDENTIFIER ASSIGN expression {
        VariableDefinition* vd = new VariableDefinition(take_name($1), expr($3));
        vd->loc = make_loc(@1);
        $$ = vd;
    }
    ;

assignment:
    reference ASSIGN expression {
        Assignment* a = new Assignment(static_cast<Reference*>($1), expr($3));
        a->loc = make_loc(@1);
        $$ = a;
    }
    ;

print_statement:
    PRINT argument_list {
        Print* p = new Print(take<Expression>($2));
        p->loc = make_loc(@1);
        $$ = p;
    }
    ;

return_statement:
    RETURN {
        Return* r = new Return();
        r->loc = make_loc(@1);
        $$ = r;
    }
    | RETURN expression {
        Return* r = new Return(expr($2));
        r->loc = make_loc(@1);
        $$ = r;
    }
    ;

exit_statement:
    EXIT {
        Exit* e = new Exit();
        e->loc = make_loc(@1);
        $$ = e;
    }
    ;

if_statement:
    IF expression THEN body END {
        If* i = new If(expr($2), take<Statement>($4));
        i->loc = make_loc(@1);
        $$ = i;
    }
    | IF expression THEN body ELSE body END {
        If* i = new If(expr($2), take<Statement>($4), take<Statement>($6));
        i->loc = make_loc(@1);
        $$ = i;
    }
    | IF expression ARROW statement {
        IfShort* i = new IfShort(expr($2), std::vector<Statement*>{ static_cast<Statement*>($4) });
        i->loc = make_loc(@1);
        $$ = i;
    }
    ;

loop_statement:
    WHILE expression LOOP body END {
        WhileLoop* w = new WhileLoop(expr($2), take<Statement>($4));
        w->loc = make_loc(@1);
        $$ = w;
    }
    | LOOP body END {
        InfiniteLoop* il = new InfiniteLoop(take<Statement>($2));
        il->loc = make_loc(@1);
        $$ = il;
    }
    | FOR IDENTIFIER IN expression LOOP body END {
        ForEachLoop* f = new ForEachLoop(expr($4), take<Statement>($6), take_name($2));
        f->loc = make_loc(@1);
        $$ = f;
    }
    | FOR IDENTIFIER IN expression RANGE expression LOOP body END {
        ForRangeLoop* f = new ForRangeLoop(expr($4), expr($6), take<Statement>($8), take_name($2));
        f->loc = make_loc(@1);
        $$ = f;
    }
    | FOR expression LOOP body END {
        ForEachLoop* f = new ForEachLoop(expr($2), take<Statement>($4));
        f->loc = make_loc(@1);
        $$ = f;
    }
    | FOR expression RANGE expression LOOP body END {
        ForRangeLoop* f = new ForRangeLoop(expr($2), expr($4), take<Statement>($6));
        f->loc = make_loc(@1);
        $$ = f;
    }
    ;

expression:
    expression OR expression    { $$ = binary($1, BinaryOperator::OR,  $3, make_loc(@1)); }
    | expression AND expression { $$ = binary($1, BinaryOperator::AND, $3, make_loc(@1)); }
    | expression XOR expression { $$ = binary($1, BinaryOperator::XOR, $3, make_loc(@1)); }
    | expression EQ expression  { $$ = binary($1, BinaryOperator::EQ,  $3, make_loc(@1)); }
    | expression NEQ expression { $$ = binary($1, BinaryOperator::NEQ, $3, make_loc(@1)); }
    | expression LT expression  { $$ = binary($1, BinaryOperator::LT,  $3, make_loc(@1)); }
    | expression LE expression  { $$ = binary($1, BinaryOperator::LE,  $3, make_loc(@1)); }
    | expression GT expression  { $$ = binary($1, BinaryOperator::GT,  $3, make_loc(@1)); }
    | expression GE expression  { $$ = binary($1, BinaryOperator::GE,  $3, make_loc(@1)); }
    | expression PLUS expression  { $$ = binary($1, BinaryOperator::ADD, $3, make_loc(@1)); }
    | expression MINUS expression { $$ = binary($1, BinaryOperator::SUB, $3, make_loc(@1)); }
    | expression MUL expression   { $$ = binary($1, BinaryOperator::MUL, $3, make_loc(@1)); }
    | expression DIV expression   { $$ = binary($1, BinaryOperator::DIV, $3, make_loc(@1)); }
    | NOT expression %prec UNARY   { $$ = unary(UnaryOperator::NOT,   $2, make_loc(@1)); }
    | PLUS expression %prec UNARY  { $$ = unary(UnaryOperator::PLUS,  $2, make_loc(@1)); }
    | MINUS expression %prec UNARY { $$ = unary(UnaryOperator::MINUS, $2, make_loc(@1)); }
    | OPEN_ROUND_BRACKET expression CLOSE_ROUND_BRACKET {
        $$ = $2;
    }
    | reference IS type_indicator {
        Is* is = new Is(static_cast<Reference*>($1), static_cast<TypeNode*>($3)->type);
        is->loc = make_loc(@1);
        $$ = is;
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
        Array* a = new Array(take<Expression>($2));
        a->loc = make_loc(@1);
        $$ = a;
    }
    ;

tuple_literal:
    OPEN_BRACE opt_tuple_element_list CLOSE_BRACE {
        Tuple* t = new Tuple(take<TupleElement>($2));
        t->loc = make_loc(@1);
        $$ = t;
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
        TupleElement* te = new TupleElement(expr($1));
        te->loc = make_loc(@1);
        $$ = te;
    }
    | IDENTIFIER ASSIGN expression {
        TupleElement* te = new TupleElement(expr($3), take_name($1));
        te->loc = make_loc(@1);
        $$ = te;
    }
    ;

function_literal:
    FUNC function_params function_body {
        FunctionLiteral* f = new FunctionLiteral(take_names($2), static_cast<FunctionBody*>($3));
        f->loc = make_loc(@1);
        $$ = f;
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
        BlockBody* b = new BlockBody(take<Statement>($3));
        b->loc = make_loc(@1);
        $$ = b;
    }
    | ARROW expression %prec FUNC_BODY {
        ShortFunctionBody* b = new ShortFunctionBody(expr($2));
        b->loc = make_loc(@1);
        $$ = b;
    }
    ;

opt_newlines:
    %empty { $$ = nullptr; }
    | opt_newlines NEWLINE
    ;

%%
