// Declares the semantic analyzer that stores declared names and scopes and the class that determines which types of semantic errors this semantic analyzer can return.
// The analyzer checks the AST without changing it.
#pragma once


#include <vector>
#include <string>
#include <unordered_set>
#include <stdexcept>

#include "../ast/ast.h"


class SemanticError : public std::runtime_error {
public:
    unsigned int line;
    unsigned int column;

    SemanticError(unsigned int line, unsigned int column, const std::string& message)
        : std::runtime_error("Semantic error at line " + std::to_string(line) +
                             ", column " + std::to_string(column) + ": " + message),
          line(line), column(column) {}

    explicit SemanticError(const std::string& message)
        : std::runtime_error("Semantic error: " + message),
          line(0), column(0) {}
};


class SemanticAnalyzer {
public:
    // Resets the analyzer state and starts the traversal of statements from the very top in source order.
    // Returns the collected errors without modifying the AST.
    std::vector<SemanticError> analyze(Program* root);

    SemanticAnalyzer() = default;
private:
    // Declared names are stored separately for each scope.
    // The last element represents the current scope.
    std::vector<std::unordered_set<std::string>> scopes;
    // Collects errors found during the current analysis.
    std::vector<SemanticError> errors;
    // Tracks the nesting of loops when checking the code of the current function.
    unsigned int cycle_depth = 0;
    // Tracks the nesting of function to verify return statements.
    unsigned int function_depth = 0;

    // Starts the new scope with no declared names. 
    void enter_scope();
    
    // Removes the current scope after its block have been checked.
    // Exception: the global scope must remain available.
    void exit_scope();

    // Adds a name in the current scope.
    void declare_variable(const std::string& name);

    // Checks in the current scope whether a name is declared in it.
    // Used to identify repetitive declarations of the same names in the same scope.
    bool is_declared_in_cur_scope(const std::string& name) const;

    // Searches from the current scope towards the global scope whether the name was declared or not.
    // We use this check because a name can be used if it is declared in any active scope.
    bool is_declared(const std::string& name) const;

    // Checks a statement and its child nodes using current scope and context.
    // Returns false if any of these checks fail.
    bool analyze_statement(Statement* statement);

    // Checks the expression and recursively valiadtes its child nodes.
    // Records errors without evaluating the expression or changing the AST.
    bool analyze_expression(Expression* expression);

    // Check that the base name is declared and we validate index(for example, array[x + 5]) and argument[for example, f[argument]] expressions.
    bool analyze_reference(Reference* reference);

    // Performs a partial check of the range boundary's of the node kind.
    // References and compound expressions are excepted without specifying their types.
    bool analyze_range_boundary(Expression* expression);

    // Performs a partial check of the iterable expressions's node kind.
    // Accepts arrays, tuples, references and binary expressions.
    bool analyze_iterable_expression(Expression* expression);

    // Recognizes NoneLiteral directly.
    // Rejects an explicit none Literal used as an operand.
    // Values stored in references are not tracked here.
    bool analyze_none_operand(Expression* expression);

    // defines how error is being added
    void report(unsigned int line, unsigned int column, const std::string& message);
    void report(const std::string& message);
};