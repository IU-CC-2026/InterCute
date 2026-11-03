// Applies constant folding, dead code elimination and unused code declaration removal.
// This actions are applied after running the semantic analyzer and they modify AST before it is printed or executed.

#pragma once


#include "../ast/ast.h"

#include <unordered_set>


class Optimizer {
public:
    // Runs constant folding, dead code elimination and unused variable removal.
    void optimize(Program* root);

    // Creates an optimizer with an empty set of collected names.
    Optimizer() = default;
private:    
    // Visits expressions and nested elemenets where folding may be applied.
    void fold_statement(Statement*& statement);
    // Fold child expression before attempting to fold parent ones.
    void fold_expression(Expression*& expression);
    // Folds array indexes and function arguments inside a reference.
    void fold_reference(Reference* reference);
    // Supported arithmetic operations re computed on integer and real literals.
    // Currently integer division is trancated to zero.
    Expression* fold_numeric(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc);
    // Converts comparisons between numeric literals into a boolean literal.
    Expression* fold_comparison(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc);
    // Folds or, xor, and when both operands are boolean literals.
    Expression* fold_logical(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc);
    // Appliesfolding for arithmetic, comparisons, logic and concatenation.
    Expression* try_fold_binary(BinaryExpression* bin);
    // Folds unary plus/minus for numeric literals and not for boolean literals.
    Expression* try_fold_unary(UnaryExpression* unary);

    // Processes a statement's expressions and nested blocks for dead code removal.
    void eliminate_dead_statement(Statement*& statement);
    // Traverses an expression to find and simplify nested function bodies.
    void eliminate_dead_expression(Expression*& expression);
    // Applies dead code traversal to array indices and call arguments.
    void eliminate_dead_reference(Reference* reference);
    // Simplifies literal boolean conditions and removes statements after a direct return or exit.
    // Also processes nested blocks.
    void eliminate_dead_block(std::vector<Statement*>& statements);


    // Names that appear in reference nodes.
    std::unordered_set<std::string> used_names;
    // Collects referenced names from a statement and its nested nodes.
    void collect_used_names_statement(Statement* statement);
    // Collects referenced names from an expression, including function bodies.
    void collect_used_names_expression(Expression* expression);
    // Records the base name and visits index and argument expressions
    void collect_used_names_reference(Reference* reference);
    // Detects calls inside an expression without entering function literal bodies.
    bool has_function_call(Expression* expression); // function calls may have side effects, do not remove them
    // Detects a function call inside a suffix or an array index.
    bool has_function_call(Reference* reference);

    // Removes unreferenced variable definitions unless their initializer contains a function call.
    void remove_unused_statement(Statement*& statement);
    // Traverses expressions to remove unused declarations in nested function bodies.
    void remove_unused_expression(Expression*& expression);
    // Processes index and argument expressions for unused declaration removal.
    void remove_unused_reference(Reference* reference);
    // Processes each statement and removes declarations left with no definitions.
    void remove_unused_block(std::vector<Statement*>& statements);
};