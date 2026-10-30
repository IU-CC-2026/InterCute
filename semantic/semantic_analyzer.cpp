#include "semantic_analyzer.h"

#include <stdexcept>


void SemanticAnalyzer::report(unsigned int line, unsigned int column, const std::string& message) {
    this->errors.push_back(SemanticError(line, column, message));
}


void SemanticAnalyzer::report(const std::string& message) {
    this->errors.push_back(SemanticError(message));
}


void SemanticAnalyzer::enter_scope() {
    this->scopes.push_back({});
}


void SemanticAnalyzer::exit_scope() {
    if (this->scopes.size() <= 1)
        throw std::logic_error("Global scope cannot be exited");
        
    this->scopes.pop_back();
}


void SemanticAnalyzer::declare_variable(const std::string& name) {
    if (this->scopes.empty())
        throw std::logic_error("At least global scope should exist");

    std::unordered_set<std::string>& current_scope = this->scopes[this->scopes.size() - 1];

    current_scope.insert(name);
}


bool SemanticAnalyzer::is_declared_in_cur_scope(const std::string& name) const {
    if (this->scopes.empty())
        throw std::logic_error("At least global scope should exist");

    const std::unordered_set<std::string>& current_scope = this->scopes[this->scopes.size() - 1];

    if (current_scope.find(name) != current_scope.end()) {
        return true;
    }

    return false;
}


bool SemanticAnalyzer::is_declared(const std::string& name) const {
    if (this->scopes.empty())
        throw std::logic_error("At least global scope should exist");

    for (int i = this->scopes.size() - 1; i >= 0; i--) {
        if (this->scopes[i].find(name) != this->scopes[i].end()) {
            return true;
        }
    }

    return false;
}


std::vector<SemanticError> SemanticAnalyzer::analyze(Program* root) {
    // Reset state
    this->errors.clear(); // clear previous errors
    this->scopes = {{}}; // initially has only global scope
    this->cycle_depth = 0;
    this->function_depth = 0;


    if (root == nullptr) {
        return this->errors;
    }

    for (Statement* statement : root->statements) {
        this->analyze_statement(statement);
    }

    return this->errors;
}


bool SemanticAnalyzer::analyze_statement(Statement* statement) {
    if (statement == nullptr) {
        this->report("Null statement");
        return false;
    }

    if (auto* declaration = dynamic_cast<Declaration*>(statement)) {
        bool valid = true;

        for (VariableDefinition* vardef : declaration->definitions) {
            if (vardef == nullptr) {
                this->report("Null variable definition");
                valid = false;
                continue;
            }

            bool valid_initializer = vardef->expr == nullptr || this->analyze_expression(vardef->expr);
            bool valid_name = !this->is_declared_in_cur_scope(vardef->identifier);

            if (!valid_name) {
                this->report(vardef->loc.first_line, vardef->loc.first_column, "Two declarations of the same variable: " + vardef->identifier);
            }

            if (!valid_name || !valid_initializer) {
                valid = false;
            }

            if (valid_name) {
                this->declare_variable(vardef->identifier);
            }
        }

        return valid;
    } else if (auto* print = dynamic_cast<Print*>(statement)) {
        bool valid = true;

        for (Expression* expr : print->exprs) {
            if (!this->analyze_expression(expr)) {
                valid = false;
            }
        }

        return valid;
    } else if (auto* assignment = dynamic_cast<Assignment*>(statement)) {
        bool valid = true;

        if (!this->analyze_expression(assignment->expr)) {
            valid = false;
        }

        if (!this->analyze_reference(assignment->ref)) {
            valid = false;
        }

        return valid;
    } else if (auto* if_st = dynamic_cast<If*>(statement)) {
        bool valid = true;

        if (!this->analyze_expression(if_st->condition)) {
            valid = false;
        }

        this->enter_scope();
        for (Statement* statement : if_st->then_statements) {
            if (!this->analyze_statement(statement)) {
                valid = false;
            }
        }
        this->exit_scope();

        if (if_st->else_statements.has_value()) {
            this->enter_scope();
            for (Statement* statement : *(if_st->else_statements)) {
                if (!this->analyze_statement(statement)) {
                    valid = false;
                }
            }
            this->exit_scope();
        }

        return valid;
    } else if (auto* if_short = dynamic_cast<IfShort*>(statement)) {
        bool valid = true;

        if (!this->analyze_expression(if_short->condition)) {
            valid = false;
        }

        this->enter_scope();
        for (Statement* statement : if_short->statements) {
            if (!this->analyze_statement(statement)) {
                valid = false;
            }
        }
        this->exit_scope();

        return valid;
    } else if (auto* while_loop = dynamic_cast<WhileLoop*>(statement)) {
        bool valid = true;

        if (!this->analyze_expression(while_loop->condition)) {
            valid = false;
        }

        this->cycle_depth++;
        this->enter_scope();
        for (Statement* statement : while_loop->statements) {
            if (!this->analyze_statement(statement)) {
                valid = false;
            }
        }
        this->exit_scope();
        this->cycle_depth--;

        return valid;
    } else if (auto* for_each_loop = dynamic_cast<ForEachLoop*>(statement)) {
        bool valid = true;

        if (!this->analyze_expression(for_each_loop->expr)) {
            valid = false;
        }

        if (!this->analyze_iterable_expression(for_each_loop->expr)) {
            valid = false;
        }

        this->cycle_depth++;
        this->enter_scope();
        if (for_each_loop->identifier.has_value()) {
            this->declare_variable(*(for_each_loop->identifier));
        }
        for (Statement* statement : for_each_loop->statements) {
            if (!this->analyze_statement(statement)) {
                valid = false;
            }
        }
        this->exit_scope();
        this->cycle_depth--;

        return valid;
    } else if (auto* for_range_loop = dynamic_cast<ForRangeLoop*>(statement)) {
        bool valid = true;

        if (!this->analyze_expression(for_range_loop->first_expr)) {
            valid = false;
        }
        if (!this->analyze_expression(for_range_loop->second_expr)) {
            valid = false;
        }

        if (!this->analyze_range_boundary(for_range_loop->first_expr)) {
            valid = false;
        }
        if (!this->analyze_range_boundary(for_range_loop->second_expr)) {
            valid = false;
        }

        this->cycle_depth++;
        this->enter_scope();
        if (for_range_loop->identifier.has_value()) {
            this->declare_variable(*(for_range_loop->identifier));
        }
        for (Statement* statement : for_range_loop->statements) {
            if (!this->analyze_statement(statement)) {
                valid = false;
            }
        }
        this->exit_scope();
        this->cycle_depth--;

        return valid;
    } else if (auto* inf_loop = dynamic_cast<InfiniteLoop*>(statement)) {
        bool valid = true;

        this->cycle_depth++;
        this->enter_scope();
        for (Statement* statement : inf_loop->statements) {
            if (!this->analyze_statement(statement)) {
                valid = false;
            }
        }
        this->exit_scope();
        this->cycle_depth--;

        return valid;
    } else if (auto* return_st = dynamic_cast<Return*>(statement)) {
        bool valid = true;

        if (this->function_depth <= 0) {
            this->report(return_st->loc.first_line, return_st->loc.first_column, "Cannot use return outside a function");
            valid = false;
        }

        if (return_st->result && !analyze_expression(return_st->result)) {
            valid = false;
        }

        return valid;
    } else if (auto* exit_st = dynamic_cast<Exit*>(statement)) {
        bool valid = true;

        if (this->cycle_depth <= 0) {
            this->report(exit_st->loc.first_line, exit_st->loc.first_column, "Cannot use exit outside a loop");
            valid = false;
        }

        return valid;
    } else {
        this->report("Non-existent statement");
        return false;
    }
}


bool SemanticAnalyzer::analyze_expression(Expression* expression) {
    if (expression == nullptr) {
        this->report("Null expression");
        return false;
    }

    if (auto* bin_expr = dynamic_cast<BinaryExpression*>(expression)) {
        bool left_valid = this->analyze_expression(bin_expr->left);
        bool right_valid = this->analyze_expression(bin_expr->right);

        bool left_none = this->analyze_none_operand(bin_expr->left);
        bool right_none = this->analyze_none_operand(bin_expr->right);

        return left_valid && right_valid & left_none && right_none;
    } else if (auto* unary_expr = dynamic_cast<UnaryExpression*>(expression)) {
        bool expr_valid = this->analyze_expression(unary_expr->expr);

        bool expr_none = this->analyze_none_operand(unary_expr->expr);

        return expr_valid && expr_none;
    } else if (auto* ref = dynamic_cast<Reference*>(expression)) {
        bool ref_valid = this->analyze_reference(ref);

        return ref_valid;
    } else if (auto* is = dynamic_cast<Is*>(expression)) {
        bool is_ref_valid = this->analyze_reference(is->ref);

        return is_ref_valid;
    } else if (auto* array = dynamic_cast<Array*>(expression)) {
        bool valid = true;

        for (Expression* expr : array->items) {
            if (!this->analyze_expression(expr)) {
                valid = false;
            }
        }

        return valid;
    } else if (auto* tuple = dynamic_cast<Tuple*>(expression)) {
        bool valid = true;

        std::unordered_set<std::string> seen_names;

        for (TupleElement* tuple_el : tuple->items) {
            if (tuple_el == nullptr) {
                this->report("Null tuple element");
                valid = false;
                continue;
            }

            if (tuple_el->identifier.has_value()) {
                const std::string& name = *(tuple_el->identifier);
                if (seen_names.find(name) != seen_names.end()) {
                    this->report(tuple_el->loc.first_line, tuple_el->loc.first_column, "Duplicate tuple element name: " + name);
                    valid = false;
                } else {
                    seen_names.insert(name);
                }
            }

            if (!this->analyze_expression(tuple_el->expr)) {
                valid = false;
            }
        }

        return valid;
    } else if (auto* func_literal = dynamic_cast<FunctionLiteral*>(expression)) {
        bool valid = true;

        unsigned int saved_cycle_depth = this->cycle_depth;
        this->cycle_depth = 0;

        this->function_depth++;
        this->enter_scope();

        for (std::string arg : func_literal->args) {
            if (this->is_declared_in_cur_scope(arg)) {
                this->report(func_literal->loc.first_line, func_literal->loc.first_column, "Duplicate function argument: " + arg);
                valid = false;
            } else {
                this->declare_variable(arg);
            }
        }

        if (auto* short_body = dynamic_cast<ShortFunctionBody*>(func_literal->body)) {
            if (!this->analyze_expression(short_body->expr)) {
                valid = false;
            }
        } else if (auto* block_body = dynamic_cast<BlockBody*>(func_literal->body)) {
            for (Statement* statement : block_body->statements) {
                if (!this->analyze_statement(statement)) {
                    valid = false;
                }
            }
        } else {
            this->report(func_literal->loc.first_line, func_literal->loc.first_column, "Null or incorrect function body");
            valid = false;
        }

        this->exit_scope();
        this->function_depth--;
        
        this->cycle_depth = saved_cycle_depth;

        return valid;
    } else if (dynamic_cast<Literal*>(expression)) {
        return true;
    } else {
        this->report("Incorrect expression");
        return false;
    }
}


bool SemanticAnalyzer::analyze_reference(Reference* reference) {
    if (reference == nullptr) {
        this->report("Null reference");
        return false;
    }

    bool valid = true;

    if (!this->is_declared(reference->identifier)) {
        this->report(reference->loc.first_line, reference->loc.first_column, "Identifier " + reference->identifier + " is not declared");
        valid = false;
    }

    for (ReferenceSuffix* suffix : reference->suffixes) {
        if (auto* array_idx = dynamic_cast<ArrayIndexSuffix*>(suffix)) {
            if (!this->analyze_expression(array_idx->index)) {
                valid = false;
            }

            if (!this->analyze_none_operand(array_idx->index)) {
                valid = false;
            }
        } else if (auto* call = dynamic_cast<CallSuffix*>(suffix)) {
            for (Expression* arg : call->args) {
                if (!this->analyze_expression(arg)) {
                    valid = false;
                }
            }
        } else if (dynamic_cast<MemberSuffix*>(suffix) || dynamic_cast<TupleIndexSuffix*>(suffix)) {
            // nothing to check
        } else {
            this->report(reference->loc.first_line, reference->loc.first_column, "Unknown reference suffix");
            valid = false;
        }
    }

    return valid;
}


bool SemanticAnalyzer::analyze_range_boundary(Expression* expression) {
    if (expression == nullptr) {
        this->report("Null range boundary");
        return false;
    }

    if (dynamic_cast<IntLiteral*>(expression) || dynamic_cast<Reference*>(expression)
        || dynamic_cast<BinaryExpression*>(expression) || dynamic_cast<UnaryExpression*>(expression)) {
        return true;
    }

    this->report(expression->loc.first_line, expression->loc.first_column, "Invalid type of range boundary\n" + expression->print());

    return false;
}


bool SemanticAnalyzer::analyze_iterable_expression(Expression* expression) {
    if (expression == nullptr) {
        this->report("Null iterable expression");
        return false;
    }

    if (dynamic_cast<Array*>(expression) || dynamic_cast<Tuple*>(expression)
        || dynamic_cast<BinaryExpression*>(expression) || dynamic_cast<Reference*>(expression)) {
        return true;
    }

    this->report(expression->loc.first_line, expression->loc.first_column, "Non-iterable object:\n" + expression->print());

    return false;
}


bool SemanticAnalyzer::analyze_none_operand(Expression* expression) {
    if (expression == nullptr) {
        this->report("Null operand");
        return false;
    }

    if (dynamic_cast<NoneLiteral*>(expression)) {
        this->report(expression->loc.first_line, expression->loc.first_column, "None cannot be used as an operand");
        return false;
    }

    return true;
}