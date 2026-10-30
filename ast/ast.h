#pragma once


#include <vector>
#include <string>
#include <optional>
#include <iostream>


struct SourceLocation {
    unsigned int first_line = 0;
    unsigned int first_column = 0;
    unsigned int last_line = 0;
    unsigned int last_column = 0;
};


class ASTNode {
public:
    SourceLocation loc;

    virtual ~ASTNode() = default;
    virtual std::string print(int level = 0) = 0;
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


class Expression : public ASTNode {};

class Statement : public ASTNode {};


// Expressions
class BinaryExpression : public Expression {
public:
    Expression* left;
    Expression* right;
    BinaryOperator op;

    BinaryExpression(Expression* left, Expression* right, BinaryOperator op);

    std::string print(int level) override;
};

class UnaryExpression : public Expression {
public:
    Expression* expr;
    UnaryOperator op;

    UnaryExpression(Expression* expr, UnaryOperator op);

    std::string print(int level) override;
};

class Literal : public Expression {};

class IntLiteral : public Literal {
public:
    int value;

    IntLiteral(int value);

    std::string print(int level) override;
};

class RealLiteral : public Literal {
public:
    double value;

    RealLiteral(double value);

    std::string print(int level) override;
};

class StringLiteral : public Literal {
public:
    std::string value;

    StringLiteral(std::string value);

    std::string print(int level) override;
};

class BoolLiteral : public Literal {
public:
    bool value;

    BoolLiteral(bool value);

    std::string print(int level) override;
};

class NoneLiteral : public Literal {
public:
    NoneLiteral() = default;

    std::string print(int level) override;
};

class FunctionBody : public ASTNode {};

class BlockBody : public FunctionBody {
public:
    std::vector<Statement*> statements;

    BlockBody(std::vector<Statement*> statements);

    std::string print(int level) override;
};

class ShortFunctionBody : public FunctionBody {
public:
    Expression* expr;

    ShortFunctionBody(Expression* expr);

    std::string print(int level) override;
};

class FunctionLiteral : public Literal {
public:
    std::vector<std::string> args;
    FunctionBody* body;

    FunctionLiteral(std::vector<std::string> args, FunctionBody* body);

    std::string print(int level) override;
};

class Array : public Literal {
public:
    std::vector<Expression*> items;

    Array(std::vector<Expression*> items);

    std::string print(int level) override;
};

class TupleElement : public ASTNode {
public:
    std::optional<std::string> identifier;
    Expression* expr;

    TupleElement(Expression* expr, std::optional<std::string> identifier = std::nullopt);

    std::string print(int level) override;
};

class Tuple : public Literal {
public:
    std::vector<TupleElement*> items;

    Tuple(std::vector<TupleElement*> items);

    std::string print(int level) override;
};

class ReferenceSuffix : public ASTNode {};

class ArrayIndexSuffix : public ReferenceSuffix {
public:
    Expression* index;

    ArrayIndexSuffix(Expression* index);

    std::string print(int level) override;
};

class CallSuffix : public ReferenceSuffix {
public:
    std::vector<Expression*> args;

    CallSuffix(std::vector<Expression*> args = {});

    std::string print(int level) override;
};

class MemberSuffix : public ReferenceSuffix {
public:
    std::string identifier;

    MemberSuffix(std::string identifier);

    std::string print(int level) override;
};

class TupleIndexSuffix : public ReferenceSuffix {
public:
    IntLiteral* index;

    TupleIndexSuffix(IntLiteral* index);

    std::string print(int level) override;
};

class Reference : public Expression {
public:
    std::string identifier;
    std::vector<ReferenceSuffix*> suffixes;

    Reference(std::string identifier, std::vector<ReferenceSuffix*> suffixes = {});

    std::string print(int level) override;
};

class VariableDefinition : public ASTNode {
public:
    std::string identifier;
    Expression* expr;

    VariableDefinition(std::string identifier, Expression* expr = nullptr);

    std::string print(int level) override;
};

class Is : public Expression {
public:
    Reference* ref;
    VariableType type;

    Is(Reference* ref, VariableType type);

    std::string print(int level) override;
};


// Statements
class Program : public ASTNode {
public:
    std::vector<Statement*> statements;

    Program(std::vector<Statement*> statements);

    std::string print(int level) override;
};

class Declaration : public Statement {
public:
    std::vector<VariableDefinition*> definitions;

    Declaration(std::vector<VariableDefinition*> definitions);

    std::string print(int level) override;
};

class If : public Statement {
public:
    Expression* condition;
    std::vector<Statement*> then_statements;
    std::optional<std::vector<Statement*>> else_statements;

    If(Expression* condition, std::vector<Statement*> then_statements, std::optional<std::vector<Statement*>> else_statements = std::nullopt);

    std::string print(int level) override;
};

class IfShort : public Statement {
public:
    Expression* condition;
    std::vector<Statement*> statements;

    IfShort(Expression* condition, std::vector<Statement*> statements);

    std::string print(int level) override;
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

    std::string print(int level) override;
};

class ForEachLoop : public Loop {
public:
    std::optional<std::string> identifier;
    Expression* expr;

    ForEachLoop(Expression* expr, std::vector<Statement*> statements, std::optional<std::string> identifier = std::nullopt);

    std::string print(int level) override;
};

class ForRangeLoop : public Loop {
public:
    std::optional<std::string> identifier;
    Expression* first_expr;
    Expression* second_expr;

    ForRangeLoop(Expression* first_expr, Expression* second_expr, std::vector<Statement*> statements, std::optional<std::string> identifier = std::nullopt);

    std::string print(int level) override;
};

class InfiniteLoop : public Loop {
public:
    InfiniteLoop(std::vector<Statement*> statements);

    std::string print(int level) override;
};

class Exit : public Statement {
public:
    Exit() = default;

    std::string print(int level) override;
};

class Return : public Statement {
public:
    Expression* result;

    Return(Expression* result = nullptr);

    std::string print(int level) override;
};

class Print : public Statement {
public:
    std::vector<Expression*> exprs;

    Print(std::vector<Expression*> exprs);

    std::string print(int level) override;
};

class Assignment : public Statement {
public:
    Reference* ref;
    Expression* expr;

    Assignment(Reference* ref, Expression* expr);

    std::string print(int level) override;
};
