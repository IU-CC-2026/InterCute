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
    std::vector<SemanticError> analyze(Program* root);

    SemanticAnalyzer() = default;
private:
    std::vector<std::unordered_set<std::string>> scopes;
    std::vector<SemanticError> errors;
    unsigned int cycle_depth = 0;
    unsigned int function_depth = 0;

    void enter_scope();

    void exit_scope();

    void declare_variable(const std::string& name);

    bool is_declared_in_cur_scope(const std::string& name) const;

    bool is_declared(const std::string& name) const;

    bool analyze_statement(Statement* statement);

    bool analyze_expression(Expression* expression);

    bool analyze_reference(Reference* reference);

    bool analyze_range_boundary(Expression* expression);

    bool analyze_iterable_expression(Expression* expression);

    bool analyze_none_operand(Expression* expression);

    void report(unsigned int line, unsigned int column, const std::string& message);
    void report(const std::string& message);
};