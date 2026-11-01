#pragma once


#include "../ast/ast.h"

#include <unordered_set>


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


    void eliminate_dead_statement(Statement*& statement);
    void eliminate_dead_expression(Expression*& expression);
    void eliminate_dead_reference(Reference* reference);
    void eliminate_dead_block(std::vector<Statement*>& statements);


    std::unordered_set<std::string> used_names;
    void collect_used_names_statement(Statement* statement);
    void collect_used_names_expression(Expression* expression);
    void collect_used_names_reference(Reference* reference);
    void remove_unused_statement(Statement*& statement);
    void remove_unused_expression(Expression*& expression);
    void remove_unused_reference(Reference* reference);
    void remove_unused_block(std::vector<Statement*>& statements);
};