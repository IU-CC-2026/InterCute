#pragma once


#include <unordered_map>
#include <variant>
#include <stdexcept>
#include <memory>
#include <vector>
#include <string>
#include <optional>
#include <utility>


#include "../ast/ast.h"


class RuntimeError : public std::runtime_error {
public:
    unsigned int line;
    unsigned int column;

    RuntimeError(unsigned int line, unsigned int column, const std::string& message)
        : std::runtime_error("Runtime error at line " + std::to_string(line) +
                             ", column " + std::to_string(column) + ": " + message),
          line(line), column(column) {}

    explicit RuntimeError(const std::string& message)
        : std::runtime_error("Runtime error: " + message),
          line(0), column(0) {}
};


struct ArrayData;
struct TupleData;
struct FunctionData;


class Value {
public:
    Value(std::variant<
        std::monostate,
        int, 
        double, 
        std::string, 
        bool,
        std::shared_ptr<ArrayData>,
        std::shared_ptr<TupleData>,
        std::shared_ptr<FunctionData>> value);

    VariableType get_type() const;
    const std::variant<
        std::monostate,
        int, 
        double, 
        std::string, 
        bool,
        std::shared_ptr<ArrayData>,
        std::shared_ptr<TupleData>,
        std::shared_ptr<FunctionData>>& get_value() const;
private:
    VariableType type;
    std::variant<
        std::monostate,
        int, 
        double, 
        std::string, 
        bool,
        std::shared_ptr<ArrayData>,
        std::shared_ptr<TupleData>,
        std::shared_ptr<FunctionData>> value;
};

struct ResolvedReference {
    Value* value;
    std::shared_ptr<Value> owner;
};

std::string value_to_string(const Value& val);

struct ArrayData {
    std::unordered_map<int, Value> items;
};

struct TupleData {
    std::vector<std::pair<std::optional<std::string>, Value>> items;
};

struct FunctionData {
    std::vector<std::string> args;
    FunctionBody* body;
    std::vector<std::unordered_map<std::string, Value>> captured_scopes;
};


class Interpreter {
public:
    void interpret(Program* root);

    Interpreter() = default;
private:
    std::vector<std::unordered_map<std::string, Value>> scopes;
    bool exit_flag = false;
    bool return_flag = false;
    Value return_value{std::monostate{}};
    
    void enter_scope();
    void exit_scope();
    bool declare_variable(const std::string& identifier, Value value);
    Value* get_variable(const std::string& identifier);

    void interpret_statement(Statement* statement);
    Value interpret_expression(Expression* expression);
    ResolvedReference interpret_reference(Reference* reference, bool for_assignment = false);

    Value evaluate_binary(Value left, Value right, BinaryOperator op, SourceLocation loc);
    Value evaluate_unary(Value operand, UnaryOperator op, SourceLocation loc);
};