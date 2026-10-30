#pragma once


#include "../ast/ast.h"


class Optimizer {
public:
    void optimize(Program* root);

    Optimizer() = default;
private:
    void fold_statement(Statement*& statement);
    void fold_expression(Expression*& expression);
    void fold_reference(Reference* reference);
    Expression* fold_numeric(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc);
    Expression* fold_comparison(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc);
    Expression* fold_logical(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc);
    Expression* try_fold_binary(BinaryExpression* bin);
    Expression* try_fold_unary(UnaryExpression* unary);
};