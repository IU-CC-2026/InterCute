#pragma once


#include <vector>
#include <string>
#include <optional>
#include <iostream>


class ASTNode {
public:
    virtual ~ASTNode() = default;
    virtual void print() = 0;
};


enum class VariableType {
    INT, REAL, STRING, BOOL,
    ARRAY, TUPLE, FUNC, NONE
};

std::string variableTypeToString(VariableType type);


enum class BinaryOperator {
    ADD, SUB, MUL, DIV,
    EQ, NEQ, LT, LE, GT, GE,
    AND, OR, XOR
};

std::string binaryOperatorToString(BinaryOperator op);


enum class UnaryOperator {
    PLUS, MINUS, NOT
};

std::string unaryOperatorToString(UnaryOperator op);


int bin_precedence(BinaryOperator op);
int unary_precedence(UnaryOperator op);


class Expression : public ASTNode {
public:
    virtual void printWithParentPrecedence(int parent_precedence = 0, bool is_right_child = false);
};

class Statement : public ASTNode {};


// Expressions
class BinaryExpression : public Expression {
public:
    Expression* left;
    Expression* right;
    BinaryOperator op;

    BinaryExpression(Expression* left, Expression* right, BinaryOperator op);

    void print() override;

    void printWithParentPrecedence(int parent_precedence = 0, bool is_right_child = false) override;
};

class UnaryExpression : public Expression {
public:
    Expression* expr;
    UnaryOperator op;

    UnaryExpression(Expression* expr, UnaryOperator op);

    void print() override;

    void printWithParentPrecedence(int parent_precedence = 0, bool is_right_child = false) override;
};

class Literal : public Expression {};

class IntLiteral : public Literal {
public:
    int value;

    IntLiteral(int value);

    void print() override;
};

class RealLiteral : public Literal {
public:
    double value;

    RealLiteral(double value);

    void print() override;
};

class StringLiteral : public Literal {
public:
    std::string value;

    StringLiteral(std::string value);

    void print() override;
};

class BoolLiteral : public Literal {
public:
    bool value;

    BoolLiteral(bool value);

    void print() override;
};

class NoneLiteral : public Literal {
public:
    NoneLiteral() = default;

    void print() override;
};

class FunctionBody : public ASTNode {};

class BlockBody : public FunctionBody {
public:
    std::vector<Statement*> statements;

    BlockBody(std::vector<Statement*> statements);

    void print() override;
};

class ShortFunctionBody : public FunctionBody {
public:
    Expression* expr;

    ShortFunctionBody(Expression* expr);

    void print() override;
};

class FunctionLiteral : public Literal {
public:
    std::vector<std::string> args;
    FunctionBody* body;

    FunctionLiteral(std::vector<std::string> args, FunctionBody* body);

    void print() override;
};

class Array : public Literal {
public:
    std::vector<Expression*> items;

    Array(std::vector<Expression*> items);

    void print() override;
};

class TupleElement : public ASTNode {
public:
    std::optional<std::string> identifier;
    Expression* expr;

    TupleElement(Expression* expr, std::optional<std::string> identifier = std::nullopt);

    void print() override;
};

class Tuple : public Literal {
public:
    std::vector<TupleElement*> items;

    Tuple(std::vector<TupleElement*> items);

    void print() override;
};

class ReferenceSuffix : public ASTNode {};

class ArrayIndexSuffix : public ReferenceSuffix {
public:
    Expression* index;

    ArrayIndexSuffix(Expression* index);

    void print() override;
};

class CallSuffix : public ReferenceSuffix {
public:
    std::vector<Expression*> args;

    CallSuffix(std::vector<Expression*> args = {});

    void print() override;
};

class MemberSuffix : public ReferenceSuffix {
public:
    std::string identifier;

    MemberSuffix(std::string identifier);

    void print() override;
};

class TupleIndexSuffix : public ReferenceSuffix {
public:
    IntLiteral* index;

    TupleIndexSuffix(IntLiteral* index);

    void print() override;
};

class Reference : public Expression {
public:
    std::string identifier;
    std::vector<ReferenceSuffix*> suffixes;

    Reference(std::string identifier, std::vector<ReferenceSuffix*> suffixes = {});

    void print() override;
};

class VariableDefinition : public ASTNode {
public:
    std::string identifier;
    Expression* expr;

    VariableDefinition(std::string identifier, Expression* expr = nullptr);

    void print() override;
};

class Declaration : public Statement {
public:
    std::vector<VariableDefinition*> definitions;

    Declaration(std::vector<VariableDefinition*> definitions);

    void print() override;
};

class Is : public Expression {
public:
    Reference* ref;
    VariableType type;

    Is(Reference* ref, VariableType type);

    void print() override;
};



// Statements
class Program : public ASTNode {
public:
    std::vector<Statement*> statements;

    Program(std::vector<Statement*> statements);

    void print() override;
};

class If : public Statement {
public:
    Expression* condition;
    std::vector<Statement*> then_statements;
    std::optional<std::vector<Statement*>> else_statements;

    If(Expression* condition, std::vector<Statement*> then_statements, std::optional<std::vector<Statement*>> else_statements = std::nullopt);

    void print() override;
};

class IfShort : public Statement {
public:
    Expression* condition;
    std::vector<Statement*> statements;

    IfShort(Expression* condition, std::vector<Statement*> statements);

    void print() override;
};

class Loop : public Statement {
public:
    std::vector<Statement*> statements;

    Loop(std::vector<Statement*> statements);
};

class WhileLoop : public Loop {
public:
    Expression* condition;

    WhileLoop(Expression* condition, std::vector<Statement*> statements);

    void print() override;
};

class ForEachLoop : public Loop {
public:
    std::optional<std::string> identifier;
    Expression* expr;

    ForEachLoop(Expression* expr, std::vector<Statement*> statements, std::optional<std::string> identifier = std::nullopt);

    void print() override;
};

class ForRangeLoop : public Loop {
public:
    std::optional<std::string> identifier;
    Expression* first_expr;
    Expression* second_expr;

    ForRangeLoop(Expression* first_expr, Expression* second_expr, std::vector<Statement*> statements, std::optional<std::string> identifier = std::nullopt);

    void print() override;
};

class InfiniteLoop : public Loop {
public:
    InfiniteLoop(std::vector<Statement*> statements);

    void print() override;
};

class Exit : public Statement {
public:
    Exit() = default;

    void print() override;
};

class Return : public Statement {
public:
    Expression* result;

    Return(Expression* result = nullptr);

    void print() override;
};

class Print : public Statement {
public:
    std::vector<Expression*> exprs;

    Print(std::vector<Expression*> exprs);

    void print() override;
};

class Assignment : public Statement {
public:
    Reference* ref;
    Expression* expr;

    Assignment(Reference* ref, Expression* expr);

    void print() override;
};
