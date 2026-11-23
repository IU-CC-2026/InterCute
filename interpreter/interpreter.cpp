#include "interpreter.h"


#include <algorithm>
#include <charconv>
#include <system_error>


Value::Value(std::variant<
        std::monostate,
        int, 
        double, 
        std::string, 
        bool,
        std::shared_ptr<ArrayData>,
        std::shared_ptr<TupleData>,
        std::shared_ptr<FunctionData>> value) {
    this->value = value;

    if (std::holds_alternative<std::monostate>(this->value))
        this->type = VariableType::NONE;
    else if (std::holds_alternative<int>(this->value))
        this->type = VariableType::INT;
    else if (std::holds_alternative<double>(this->value))
        this->type = VariableType::REAL;
    else if (std::holds_alternative<std::string>(this->value))
        this->type = VariableType::STRING;
    else if (std::holds_alternative<bool>(this->value))
        this->type = VariableType::BOOL;
    else if (std::holds_alternative<std::shared_ptr<ArrayData>>(this->value))
        this->type = VariableType::ARRAY;
    else if (std::holds_alternative<std::shared_ptr<TupleData>>(this->value))
        this->type = VariableType::TUPLE;
    else
        this->type = VariableType::FUNC;
}

VariableType Value::get_type() const {
    return this->type;
}

const std::variant<
        std::monostate,
        int, 
        double, 
        std::string, 
        bool,
        std::shared_ptr<ArrayData>,
        std::shared_ptr<TupleData>,
        std::shared_ptr<FunctionData>>& Value::get_value() const {
    return this->value;
}

std::string value_to_string(const Value& val) {
    switch (val.get_type()) {
        case VariableType::INT:    return std::to_string(std::get<int>(val.get_value()));
        case VariableType::REAL: {
            double value = std::get<double>(val.get_value());
            char buffer[64];

            auto [ptr, ec] = std::to_chars(
                buffer,
                buffer + sizeof(buffer),
                value,
                std::chars_format::general
            );

            if (ec != std::errc{})
                throw std::runtime_error("Failed to format REAL value");

            std::string result(buffer, ptr);

            if (result.find('.') == std::string::npos &&
                result.find('e') == std::string::npos &&
                result.find('E') == std::string::npos) {
                result += ".0";
            }

            return result;
        }
        case VariableType::STRING: return std::get<std::string>(val.get_value());
        case VariableType::BOOL:   return (std::get<bool>(val.get_value()) ? "true" : "false");
        case VariableType::NONE:   return "none";
        case VariableType::ARRAY: {
            std::string result = "[";
            auto array_data = std::get<std::shared_ptr<ArrayData>>(val.get_value());

            int index = 1;
            for (const auto& [key, value] : array_data->items) {
                result += value_to_string(value);

                if (index < array_data->items.size())
                    result += ", ";

                index++;
            }

            result += "]";

            return result;
        }
        case VariableType::TUPLE: {
            std::string result = "{";
            auto tuple_data = std::get<std::shared_ptr<TupleData>>(val.get_value());

            int index = 1;
            for (const auto& pair : tuple_data->items) {
                if (pair.first.has_value())
                    result += *pair.first + " := ";
                
                result += value_to_string(pair.second);

                if (index < tuple_data->items.size())
                    result += ", ";

                index++;
            }

            result += "}";

            return result;
        }
        case VariableType::FUNC: {
            std::string result = "func(";
            auto function_data = std::get<std::shared_ptr<FunctionData>>(val.get_value());

            int index = 1;
            for (const auto& arg : function_data->args) {
                result += arg;

                if (index < function_data->args.size())
                    result += ", ";

                index++;
            }

            result += ")";

            return result;
        }
    }

    return "?";
}


void Interpreter::enter_scope() {
    this->scopes.push_back({});
}

void Interpreter::exit_scope() {
    if (this->scopes.size() <= 1)
        throw std::logic_error("Global scope cannot be exited");

    this->scopes.pop_back();
}

bool Interpreter::declare_variable(const std::string& identifier, Value value) {
    if (this->scopes.empty())
        throw std::logic_error("At least global scope should exist");

    std::unordered_map<std::string, Value>& current_scope = this->scopes[this->scopes.size() - 1];

    if (current_scope.find(identifier) != current_scope.end())
        return false;

    current_scope.insert_or_assign(identifier, value);
    return true;
}


Value* Interpreter::get_variable(const std::string& identifier) {
    if (this->scopes.empty())
        throw std::logic_error("At least global scope should exist");

    for (int i = scopes.size() - 1; i >= 0; i--) {
        std::unordered_map<std::string, Value>& current_scope = this->scopes[i];

        auto it = current_scope.find(identifier);
        if (it != current_scope.end()) {
            return &it->second;
        }
    }

    // no scope contains target variable
    return nullptr;
}


Value Interpreter::evaluate_binary(Value left, Value right, BinaryOperator op, SourceLocation loc) {
    VariableType left_type = left.get_type();
    VariableType right_type = right.get_type();

    Value result = Value(std::monostate{});

    if (left_type == VariableType::BOOL && right_type == VariableType::BOOL) {
        bool left_value = std::get<bool>(left.get_value());
        bool right_value = std::get<bool>(right.get_value());

        switch (op) {
            case BinaryOperator::AND:
                result = Value(left_value && right_value);
                break;
            
            case BinaryOperator::OR:
                result = Value(left_value || right_value);
                break;

            case BinaryOperator::XOR:
                result = Value(left_value ^ right_value);
                break;

            default:
                throw RuntimeError(loc.first_line, loc.first_column, "Unsupported binary operator '" + binaryOperatorToString(op) + "' for '" + variableTypeToString(left_type) + "' and '" + variableTypeToString(right_type) + "'");
        }

        return result;
    }

    if (left_type == VariableType::STRING && right_type == VariableType::STRING) {
        std::string left_value = std::get<std::string>(left.get_value());
        std::string right_value = std::get<std::string>(right.get_value());

        switch (op) {
            case BinaryOperator::ADD:
                result = Value(left_value + right_value);
                break;

            default:
                throw RuntimeError(loc.first_line, loc.first_column, "Unsupported binary operator '" + binaryOperatorToString(op) + "' for '" + variableTypeToString(left_type) + "' and '" + variableTypeToString(right_type) + "'");
        }

        return result;
    }

    if (left_type == VariableType::ARRAY && right_type == VariableType::ARRAY) {
        auto left_value = std::get<std::shared_ptr<ArrayData>>(left.get_value());
        auto right_value = std::get<std::shared_ptr<ArrayData>>(right.get_value());

        switch (op) {
            case BinaryOperator::ADD: {
                auto merged = std::make_shared<ArrayData>();

                int index = 1;

                for (const auto& [key, value] : left_value->items) {
                    merged->items.insert_or_assign(index++, value);
                }
                for (const auto& [key, value] : right_value->items) {
                    merged->items.insert_or_assign(index++, value);
                }

                result = Value(merged);
                break;
            }

            default:
                throw RuntimeError(loc.first_line, loc.first_column, "Unsupported binary operator '" + binaryOperatorToString(op) + "' for '" + variableTypeToString(left_type) + "' and '" + variableTypeToString(right_type) + "'");
        }

        return result;
    }

    if (left_type == VariableType::TUPLE && right_type == VariableType::TUPLE) {
        auto left_value = std::get<std::shared_ptr<TupleData>>(left.get_value());
        auto right_value = std::get<std::shared_ptr<TupleData>>(right.get_value());

        switch (op) {
            case BinaryOperator::ADD: {
                auto merged = std::make_shared<TupleData>();
                std::unordered_map<std::string, int> name_to_index;

                for (const auto& pair : left_value->items) {
                    if (pair.first.has_value()) {
                        std::string name = *(pair.first);

                        if (name_to_index.find(name) != name_to_index.end()) {
                            merged->items[name_to_index[name]] = pair;
                        } else {
                            name_to_index[name] = merged->items.size();
                            merged->items.push_back(pair);
                        }
                    } else {
                        merged->items.push_back(pair);
                    }
                }

                for (const auto& pair : right_value->items) {
                    if (pair.first.has_value()) {
                        std::string name = *(pair.first);

                        if (name_to_index.find(name) != name_to_index.end()) {
                            merged->items[name_to_index[name]] = pair;
                        } else {
                            name_to_index[name] = merged->items.size();
                            merged->items.push_back(pair);
                        }
                    } else {
                        merged->items.push_back(pair);
                    }
                }

                result = Value(merged);
                break;
            }

            default:
                throw RuntimeError(loc.first_line, loc.first_column, "Unsupported binary operator '" + binaryOperatorToString(op) + "' for '" + variableTypeToString(left_type) + "' and '" + variableTypeToString(right_type) + "'");
        }

        return result;
    }

    bool left_is_num = (left_type == VariableType::INT) || (left_type == VariableType::REAL);
    bool right_is_num = (right_type == VariableType::INT) || (right_type == VariableType::REAL);

    if (!left_is_num || !right_is_num)
        throw RuntimeError(loc.first_line, loc.first_column, "Unsupported binary operator '" + binaryOperatorToString(op) + "' for '" + variableTypeToString(left_type) + "' and '" + variableTypeToString(right_type) + "'");

    double left_value = left_type == VariableType::INT 
        ? std::get<int>(left.get_value()) 
        : std::get<double>(left.get_value());
    double right_value = right_type == VariableType::INT 
        ? std::get<int>(right.get_value()) 
        : std::get<double>(right.get_value());

    switch (op) {
        case BinaryOperator::ADD:
            result = Value(left_value + right_value);
            break;
        
        case BinaryOperator::SUB:
            result = Value(left_value - right_value);
            break;

        case BinaryOperator::MUL:
            result = Value(left_value * right_value);
            break;

        case BinaryOperator::DIV:
            if (right_value == 0)
                throw RuntimeError(loc.first_line, loc.first_column, "Division by zero");

            result = Value(left_value / right_value);
            break;

        case BinaryOperator::EQ:
            result = Value(left_value == right_value);
            break;
        
        case BinaryOperator::NEQ:
            result = Value(left_value != right_value);
            break;

        case BinaryOperator::LT:
            result = Value(left_value < right_value);
            break;

        case BinaryOperator::LE:
            result = Value(left_value <= right_value);
            break;

        case BinaryOperator::GT:
            result = Value(left_value > right_value);
            break;

        case BinaryOperator::GE:
            result = Value(left_value >= right_value);
            break;

        default:
            throw RuntimeError(loc.first_line, loc.first_column, "Unsupported binary operator '" + binaryOperatorToString(op) + "' for '" + variableTypeToString(left_type) + "' and '" + variableTypeToString(right_type) + "'");
    }

    bool is_arithmetic = op == BinaryOperator::ADD ||
        op == BinaryOperator::SUB ||
        op == BinaryOperator::MUL ||
        op == BinaryOperator::DIV;

    if (is_arithmetic && left_type == VariableType::INT && right_type == VariableType::INT) {
        return Value(static_cast<int>(std::get<double>(result.get_value())));
    }

    return result;
}


Value Interpreter::evaluate_unary(Value operand, UnaryOperator op, SourceLocation loc) {
    VariableType type = operand.get_type();

    switch (op) {
        case UnaryOperator::PLUS:
            if (type == VariableType::INT || type == VariableType::REAL)
                return operand;
            break;

        case UnaryOperator::MINUS:
            if (type == VariableType::INT)
                return Value(-std::get<int>(operand.get_value()));

            if (type == VariableType::REAL)
                return Value(-std::get<double>(operand.get_value()));
            break;

        case UnaryOperator::NOT:
            if (type == VariableType::BOOL)
                return Value(!std::get<bool>(operand.get_value()));
            break;
    }

    throw RuntimeError(loc.first_line, loc.first_column, "Unsupported unary operator '" + unaryOperatorToString(op) + "' for '" + variableTypeToString(type) + "'");
}


void Interpreter::interpret(Program* root) {
    if (root == nullptr) {
        return;
    }

    // Reset state
    this->scopes = {{}};
    this->exit_flag = false;
    this->return_flag = false;
    this->return_value = Value(std::monostate{});

    for (Statement* statement : root->statements) {
        this->interpret_statement(statement);
    }
}


void Interpreter::interpret_statement(Statement* statement) {
    if (statement == nullptr)
        throw RuntimeError("Null statement");

    if (auto* declaration = dynamic_cast<Declaration*>(statement)) {
        for (VariableDefinition* vardef : declaration->definitions) {
            if (vardef == nullptr)
                throw RuntimeError(declaration->loc.first_line, declaration->loc.first_column, "Null variable definition");

            Value val = Value(std::monostate{});

            if (vardef->expr != nullptr)
                val = this->interpret_expression(vardef->expr);
            
            if (!this->declare_variable(vardef->identifier, val))
                throw RuntimeError(vardef->loc.first_line, vardef->loc.first_column, "Variable '" + vardef->identifier + "' is already declared in current scope");
        }
    } else if (auto* print = dynamic_cast<Print*>(statement)) {
        for (Expression*& expr : print->exprs) {
            Value val = this->interpret_expression(expr);
            
            std::cout << value_to_string(val) << std::endl;
        }
    } else if (auto* assignment = dynamic_cast<Assignment*>(statement)) {
        Value val = this->interpret_expression(assignment->expr);

        Value& target = this->interpret_reference(assignment->ref, true);

        target = val;
    } else if (auto* if_st = dynamic_cast<If*>(statement)) {
        Value cond_value = this->interpret_expression(if_st->condition);

        if (cond_value.get_type() != VariableType::BOOL)
            throw RuntimeError(if_st->condition->loc.first_line, if_st->condition->loc.first_column, "If-else condition should be of type bool");

        auto bool_val = std::get<bool>(cond_value.get_value());

        if (bool_val) {
            this->enter_scope();

            for (Statement* statement : if_st->then_statements) {
                this->interpret_statement(statement);

                if (this->exit_flag || this->return_flag) break;
            }

            this->exit_scope();
        } else {
            if (if_st->else_statements.has_value()) {
                this->enter_scope();

                for (Statement* statement : *(if_st->else_statements)) {
                    this->interpret_statement(statement);

                    if (this->exit_flag || this->return_flag) break;
                }

                this->exit_scope();
            }
        }
    } else if (auto* if_short = dynamic_cast<IfShort*>(statement)) {
        Value cond_value = this->interpret_expression(if_short->condition);

        if (cond_value.get_type() != VariableType::BOOL)
            throw RuntimeError(if_short->condition->loc.first_line, if_short->condition->loc.first_column, "If-else condition should be of type bool");

        auto bool_val = std::get<bool>(cond_value.get_value());

        if (bool_val) {
            this->enter_scope();

            for (Statement* statement : if_short->statements) {
                this->interpret_statement(statement);

                if (this->exit_flag || this->return_flag) break;
            }

            this->exit_scope();
        }
    } else if (auto* while_loop = dynamic_cast<WhileLoop*>(statement)) {
        while (true) {
            Value cond_value = this->interpret_expression(while_loop->condition);

            if (cond_value.get_type() != VariableType::BOOL)
                throw RuntimeError(while_loop->condition->loc.first_line, while_loop->condition->loc.first_column, "While-loop condition should be of type bool");

            auto bool_val = std::get<bool>(cond_value.get_value());

            if (bool_val) {
                this->enter_scope();

                for (Statement* statement : while_loop->statements) {
                    this->interpret_statement(statement);

                    if (this->exit_flag || this->return_flag) break;
                }

                this->exit_scope();

                if (this->exit_flag) {
                    this->exit_flag = false;
                    break;
                }

                if (this->return_flag) break;
            } else break;
        }
    } else if (auto* for_each_loop = dynamic_cast<ForEachLoop*>(statement)) {
        Value iterable = this->interpret_expression(for_each_loop->expr);

        if (iterable.get_type() != VariableType::ARRAY && iterable.get_type() != VariableType::TUPLE)
            throw RuntimeError(for_each_loop->expr->loc.first_line, for_each_loop->expr->loc.first_column, "For-each-loop cannot iterate over non-iterable type");

        std::vector<Value> values;

        if (iterable.get_type() == VariableType::ARRAY) {
            auto array_data = std::get<std::shared_ptr<ArrayData>>(iterable.get_value());

            for (const auto& [_, value] : array_data->items) {
                values.push_back(value);
            }
        } else if (iterable.get_type() == VariableType::TUPLE) {
            auto tuple_data = std::get<std::shared_ptr<TupleData>>(iterable.get_value());

            for (const auto& pair : tuple_data->items) {
                values.push_back(pair.second);
            }
        }

        for (const auto& value : values) {
            this->enter_scope();

            if (for_each_loop->identifier.has_value()) {
                if (!declare_variable(*for_each_loop->identifier, value))
                    throw RuntimeError(for_each_loop->loc.first_line, for_each_loop->loc.first_column, "Variable '" + *for_each_loop->identifier + "' is already declared in current scope");
            }

            for (Statement* statement : for_each_loop->statements) {
                this->interpret_statement(statement);

                if (this->exit_flag || this->return_flag) break;
            }

            this->exit_scope();

            if (this->exit_flag) {
                this->exit_flag = false;
                break;
            }

            if (this->return_flag) break;
        }
    } else if (auto* for_range_loop = dynamic_cast<ForRangeLoop*>(statement)) {
        Value start = this->interpret_expression(for_range_loop->first_expr);
        Value end = this->interpret_expression(for_range_loop->second_expr);

        if (start.get_type() != VariableType::INT || end.get_type() != VariableType::INT)
            throw RuntimeError("Invalid type of boundaries: '" + variableTypeToString(start.get_type()) + "' and '" + variableTypeToString(end.get_type()) + "'");

        auto start_int = std::get<int>(start.get_value());
        auto end_int = std::get<int>(end.get_value());

        for (int i = start_int; i < end_int; i++) {
            this->enter_scope();

            if (for_range_loop->identifier.has_value()) {
                if (!declare_variable(*for_range_loop->identifier, Value(i)))
                    throw RuntimeError(for_range_loop->first_expr->loc.first_line, for_range_loop->first_expr->loc.first_column, "Variable '" + *for_range_loop->identifier + "' is already declared in current scope");
            }

            for (Statement* statement : for_range_loop->statements) {
                this->interpret_statement(statement);

                if (this->exit_flag || this->return_flag) break;
            }

            this->exit_scope();

            if (this->exit_flag) {
                this->exit_flag = false;
                break;
            }

            if (this->return_flag) break;
        }
    } else if (auto* inf_loop = dynamic_cast<InfiniteLoop*>(statement)) {
        while (true) {
            this->enter_scope();

            for (Statement* statement : inf_loop->statements) {
                this->interpret_statement(statement);

                if (this->exit_flag || this->return_flag) break;
            }

            this->exit_scope();

            if (this->exit_flag) {
                this->exit_flag = false;
                break;
            }

            if (this->return_flag) break;
        }
    } else if (auto* return_st = dynamic_cast<Return*>(statement)) {
        this->return_flag = true;
        
        if (return_st->result == nullptr)
            this->return_value = Value(std::monostate{});
        else
            this->return_value = this->interpret_expression(return_st->result);
    } else if (auto* exit_st = dynamic_cast<Exit*>(statement)) {
        this->exit_flag = true;
    } else if (auto* block = dynamic_cast<Block*>(statement)) {
        for (Statement* statement : block->statements) {
            this->interpret_statement(statement);

            if (this->exit_flag || this->return_flag) break;
        }
    }
}


Value Interpreter::interpret_expression(Expression* expression) {
    if (expression == nullptr)
        throw RuntimeError("Null expression");

    if (auto* bin_expr = dynamic_cast<BinaryExpression*>(expression)) {
        Value left = this->interpret_expression(bin_expr->left);
        Value right = this->interpret_expression(bin_expr->right);

        return this->evaluate_binary(left, right, bin_expr->op, bin_expr->loc);
    } else if (auto* unary_expr = dynamic_cast<UnaryExpression*>(expression)) {
        Value operand = this->interpret_expression(unary_expr->expr);

        return this->evaluate_unary(operand, unary_expr->op, unary_expr->loc);
    } else if (auto* ref = dynamic_cast<Reference*>(expression)) {
        return this->interpret_reference(ref, false);
    } else if (auto* is = dynamic_cast<Is*>(expression)) {
        Value ref_value = this->interpret_reference(is->ref);

        return Value(ref_value.get_type() == is->type);
    } else if (auto* array = dynamic_cast<Array*>(expression)) {
        auto array_data = std::make_shared<ArrayData>();
        int index = 1;

        for (Expression* expr : array->items) {
            Value item_value = this->interpret_expression(expr);
            array_data->items.insert_or_assign(index, item_value);
            index++;
        }

        return Value(array_data);
    } else if (auto* tuple = dynamic_cast<Tuple*>(expression)) {
        auto tuple_data = std::make_shared<TupleData>();

        for (TupleElement* tuple_el : tuple->items) {
            if (tuple_el == nullptr)
                throw RuntimeError(tuple->loc.first_line, tuple->loc.first_column, "Null tuple element");

            Value tuple_item_value = this->interpret_expression(tuple_el->expr);
            tuple_data->items.push_back(std::make_pair(tuple_el->identifier, tuple_item_value));
        }

        return Value(tuple_data);
    } else if (auto* func_literal = dynamic_cast<FunctionLiteral*>(expression)) {
        auto function_data = std::make_shared<FunctionData>();

        for (std::string arg : func_literal->args) {
            function_data->args.push_back(arg);
        }

        function_data->body = func_literal->body;
        function_data->captured_scopes = this->scopes;

        return Value(function_data);
    } else if (auto* int_literal = dynamic_cast<IntLiteral*>(expression)) {
        return Value(int_literal->value);
    } else if (auto* real_literal = dynamic_cast<RealLiteral*>(expression)) {
        return Value(real_literal->value);
    } else if (auto* string_literal = dynamic_cast<StringLiteral*>(expression)) {
        return Value(string_literal->value);
    } else if (auto* bool_literal = dynamic_cast<BoolLiteral*>(expression)) {
        return Value(bool_literal->value);
    } else if (auto* none_literal = dynamic_cast<NoneLiteral*>(expression)) {
        return Value(std::monostate{});
    }

    // unsupported expression type
    throw RuntimeError(expression->loc.first_line, expression->loc.first_column, "Unsupported expression type");
}


Value& Interpreter::interpret_reference(Reference* reference, bool for_assignment) {
    if (reference == nullptr)
        throw RuntimeError("Null reference");

    Value* current = this->get_variable(reference->identifier);
    if (current == nullptr)
        throw RuntimeError(reference->loc.first_line, reference->loc.first_column, "Variable '" + reference->identifier + "' is not declared");
    
    for (int i = 0; i < reference->suffixes.size(); i++) {
        ReferenceSuffix* suffix = reference->suffixes[i];

        if (auto* array_idx = dynamic_cast<ArrayIndexSuffix*>(suffix)) {
            if (current->get_type() != VariableType::ARRAY)
                throw RuntimeError(reference->loc.first_line, reference->loc.first_column, "Non-array type cannot be indexed");

            Value index_val = this->interpret_expression(array_idx->index);

            if (index_val.get_type() != VariableType::INT)
                throw RuntimeError(array_idx->loc.first_line, array_idx->loc.first_column, "Index should be of int type");

            int index = std::get<int>(index_val.get_value());

            auto array_data = std::get<std::shared_ptr<ArrayData>>(current->get_value());

            auto it = array_data->items.find(index);
            if (it == array_data->items.end()) {
                if (!for_assignment)
                    throw RuntimeError(array_idx->index->loc.first_line, array_idx->index->loc.first_column, "Index not found: " + std::to_string(index));

                if (i == reference->suffixes.size() - 1) {
                    array_data->items.insert_or_assign(index, Value(std::monostate{}));
                } else if (dynamic_cast<ArrayIndexSuffix*>(reference->suffixes[i + 1])) {
                    array_data->items.insert_or_assign(index, Value(std::make_shared<ArrayData>()));
                } else {
                    throw RuntimeError(array_idx->index->loc.first_line, array_idx->index->loc.first_column, "Cannot infer type of missing array element");
                }
            }   
            
            it = array_data->items.find(index);
            current = &it->second;

        } else if (auto* call = dynamic_cast<CallSuffix*>(suffix)) {
            if (current->get_type() != VariableType::FUNC)
                throw RuntimeError(reference->loc.first_line, reference->loc.first_column, "Non-function type cannot be called");

            auto function_data = std::get<std::shared_ptr<FunctionData>>(current->get_value());

            if (function_data->args.size() != call->args.size())
                throw RuntimeError(call->loc.first_line, call->loc.first_column, "Incorrect number of arguments");

            std::vector<Value> evaluated_args;
            for (Expression* arg : call->args) {
                evaluated_args.push_back(this->interpret_expression(arg));
            }

            auto caller_scopes = this->scopes;
            this->scopes = function_data->captured_scopes;

            this->enter_scope();

            for (int i = 0; i < evaluated_args.size(); i++) {
                if (!this->declare_variable(function_data->args[i], evaluated_args[i]))
                    throw RuntimeError(call->loc.first_line, call->loc.first_column, "Variable '" + function_data->args[i] + "' is already declared in current scope");
            }

            this->return_value = Value(std::monostate{});
            this->return_flag = false;

            if (auto* short_body = dynamic_cast<ShortFunctionBody*>(function_data->body)) {
                this->return_value = this->interpret_expression(short_body->expr);
            } else if (auto* block_body = dynamic_cast<BlockBody*>(function_data->body)) {
                for (Statement* statement : block_body->statements) {
                    this->interpret_statement(statement);

                    if (this->return_flag) break;
                }
            }

            current = &this->return_value;

            this->exit_scope();

            this->scopes = caller_scopes;

            this->return_flag = false;
        } else if (auto* member = dynamic_cast<MemberSuffix*>(suffix)) {
            if (current->get_type() != VariableType::TUPLE)
                throw RuntimeError(reference->loc.first_line, reference->loc.first_column, "Non-tuple type does not have members");

            auto tuple_data = std::get<std::shared_ptr<TupleData>>(current->get_value());

            auto it = std::find_if(
                tuple_data->items.begin(),
                tuple_data->items.end(),
                [&](const auto& item) {
                    return item.first == member->identifier;
                }
            );

            if (it == tuple_data->items.end())
                throw RuntimeError(member->loc.first_line, member->loc.first_column, "Tuple member not found: '" + member->identifier + "'");

            current = &it->second;

        } else if (auto* tuple_idx = dynamic_cast<TupleIndexSuffix*>(suffix)) {
            if (current->get_type() != VariableType::TUPLE)
                throw RuntimeError(reference->loc.first_line, reference->loc.first_column, "Non-tuple type cannot be indexed");

            int index = tuple_idx->index->value - 1;

            auto tuple_data = std::get<std::shared_ptr<TupleData>>(current->get_value());

            if (index < 0 || index >= tuple_data->items.size())
                throw RuntimeError(tuple_idx->index->loc.first_line, tuple_idx->index->loc.first_column, "Index out of bound: " + std::to_string(index));
            
            current = &tuple_data->items[index].second;
        }
    }

    return *current;
}
