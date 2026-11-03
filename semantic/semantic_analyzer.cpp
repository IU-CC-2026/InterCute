// Traverses the AST and checks program nodes(their names, scopes and statement context). For example, for function it checks it parameters and body.
// Collects semantic errors for the caller.
#include "semantic_analyzer.h"

#include <stdexcept>


// Record an error with its source position.
void SemanticAnalyzer::report(unsigned int line, unsigned int column, const std::string& message) {
    this->errors.push_back(SemanticError(line, column, message));
}


// Record an error when source coordinates are unavailable.
// Errors are recorded in the vector, so analysis can continue. 
// That is why analyzer can procede with traversing and find several mistakes during one run.
void SemanticAnalyzer::report(const std::string& message) {
    this->errors.push_back(SemanticError(message));
}

// Adds an empty set to the end of the list.
// After that new declarations will be written to it.
void SemanticAnalyzer::enter_scope() {
    this->scopes.push_back({});
}


void SemanticAnalyzer::exit_scope() {
    // if the number of active scopes is 1, we can't remove it, as it's a global scope.
    if (this->scopes.size() <= 1)
        throw std::logic_error("Global scope cannot be exited");
    // deletes the last, e.g., the current scope.
    this->scopes.pop_back();
}


// This function only registers name.
// The message about the repeated declaration is formulated in the other place: before registration analyzer checks individually whether the name is available.
void SemanticAnalyzer::declare_variable(const std::string& name) {
    // Analyzing the current scope.
    if (this->scopes.empty())
        throw std::logic_error("At least global scope should exist");

    // Checks if at least the global scope exist.
    // Gets the last element of the vector - current scope.
    std::unordered_set<std::string>& current_scope = this->scopes[this->scopes.size() - 1];

    // Adds name
    current_scope.insert(name);
}


bool SemanticAnalyzer::is_declared_in_cur_scope(const std::string& name) const {
    if (this->scopes.empty())
        throw std::logic_error("At least global scope should exist");

    const std::unordered_set<std::string>& current_scope = this->scopes[this->scopes.size() - 1];

    // if condition is true, it means that the name was found in the current set.
    if (current_scope.find(name) != current_scope.end()) {
        return true;
    }

    return false;
}


bool SemanticAnalyzer::is_declared(const std::string& name) const {
    if (this->scopes.empty())
        throw std::logic_error("At least global scope should exist");

    // Loop starts search from the last scope, then moves to the outer scopes.
    for (int i = this->scopes.size() - 1; i >= 0; i--) {
        if (this->scopes[i].find(name) != this->scopes[i].end()) {
            return true;
        }
    }

    return false;
}


// The whole analyze is run in this function.
std::vector<SemanticError> SemanticAnalyzer::analyze(Program* root) {
    // Reset state
    this->errors.clear(); // clear previous errors
    this->scopes = {{}}; // initially has only global scope
    this->cycle_depth = 0;
    this->function_depth = 0;


    if (root == nullptr) {
        return this->errors;
    }

    // Triggers a recursive process that checks everything below these statements.
    // Each statement records its errors in the shared errors vector.
    for (Statement* statement : root->statements) {
        this->analyze_statement(statement);
    }

    return this->errors;
}


bool SemanticAnalyzer::analyze_statement(Statement* statement) {
    // Firstly, pointer is being checked.
    // If node is absent, we can't check its fields.
    // In this case we report about a missing statement node before accessing its fields.
    if (statement == nullptr) {
        this->report("Null statement");
        return false;
    }

    // Checking the specific type of instruction.
    if (auto* declaration = dynamic_cast<Declaration*>(statement)) {
        bool valid = true;

        for (VariableDefinition* vardef : declaration->definitions) {
            if (vardef == nullptr) {
                this->report("Null variable definition");
                valid = false;
                continue;
            }

            // Checking initializer before registering the new name.
            // However, a declaration without an explicit initializer is allowed.
            bool valid_initializer = vardef->expr == nullptr || this->analyze_expression(vardef->expr);

            // Only the current scope of name is being checked.
            bool valid_name = !this->is_declared_in_cur_scope(vardef->identifier);

            if (!valid_name) {
                this->report(vardef->loc.first_line, vardef->loc.first_column, "Two declarations of the same variable: " + vardef->identifier);
            }

            if (!valid_name || !valid_initializer) {
                valid = false;
            }

            // 
            if (valid_name) {
                this->declare_variable(vardef->identifier);
            }
        }

        return valid;
    } else if (auto* print = dynamic_cast<Print*>(statement)) {
        bool valid = true;

        // checking every expression passed to print.
        // this block does not print values, it checks expression, which future executer will have to print.
        for (Expression* expr : print->exprs) {
            if (!this->analyze_expression(expr)) {
                valid = false;
            }
        }

        return valid;
    } else if (auto* assignment = dynamic_cast<Assignment*>(statement)) {
        bool valid = true;

        // Checking the expression whose result will be assigned.
        if (!this->analyze_expression(assignment->expr)) {
            valid = false;
        }

        // Checking the target reference independently of the expression
        if (!this->analyze_reference(assignment->ref)) {
            valid = false;
        }

        return valid;
    } else if (auto* if_st = dynamic_cast<If*>(statement)) {
        bool valid = true;

        // Check the condition expression.
        if (!this->analyze_expression(if_st->condition)) {
            valid = false;
        }

        // Check the then branch in its own scope.
        this->enter_scope();
        for (Statement* statement : if_st->then_statements) {
            if (!this->analyze_statement(statement)) {
                valid = false;
            }
        }
        this->exit_scope();

        // Check the optional else branch in a separate scope.
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
    } else if (auto* if_short = dynamic_cast<IfShort*>(statement)) { // Check the short if condition and its body in a new scope.
        
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


        // Check the condition before entering the loop body's scope.
        if (!this->analyze_expression(while_loop->condition)) {
            valid = false;
        }

        // Increasing loop counter so exit statements are allowed in its body.
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
    } else if (auto* for_each_loop = dynamic_cast<ForEachLoop*>(statement)) { // Checks the iteration over the collection.
        bool valid = true;

        // Check expressions and reject unsupported iterable node ones.
        if (!this->analyze_expression(for_each_loop->expr)) {
            valid = false;
        }

        if (!this->analyze_iterable_expression(for_each_loop->expr)) {
            valid = false;
        }

        // Registering optional iteration variable in the loop body's scope.
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
        // We are restoring the scope and loop nesting level.
        this->exit_scope();
        this->cycle_depth--;

        return valid;
    } else if (auto* for_range_loop = dynamic_cast<ForRangeLoop*>(statement)) { // Checks the iteration over the range.
        bool valid = true;

        // Check both boundary expression for semantic errors.
        if (!this->analyze_expression(for_range_loop->first_expr)) {
            valid = false;
        }
        if (!this->analyze_expression(for_range_loop->second_expr)) {
            valid = false;
        }

        // Reject range boundaries whose node ones are not supported.
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
    } else if (auto* inf_loop = dynamic_cast<InfiniteLoop*>(statement)) { //Check the body without checking the loop condition as it is absent in a new scope and loop context.
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

        // Return statement is only allowed inside a function.
        if (this->function_depth <= 0) {
            this->report(return_st->loc.first_line, return_st->loc.first_column, "Cannot use return outside a function");
            valid = false;
        }

        // check the returned expression if it is present.
        if (return_st->result && !analyze_expression(return_st->result)) {
            valid = false;
        }

        return valid;
    } else if (auto* exit_st = dynamic_cast<Exit*>(statement)) {
        bool valid = true;

        // An exit statement requires a loop(for example, while, for and so on) in the current function context.
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

    // binary expressions a + b, x < y, p and q.
    if (auto* bin_expr = dynamic_cast<BinaryExpression*>(expression)) {
        // checking both operands independently.
        bool left_valid = this->analyze_expression(bin_expr->left);
        bool right_valid = this->analyze_expression(bin_expr->right);

        // Rejecting explicit none literals used as operands.
        bool left_none = this->analyze_none_operand(bin_expr->left);
        bool right_none = this->analyze_none_operand(bin_expr->right);

        return left_valid && right_valid & left_none && right_none;
    } else if (auto* unary_expr = dynamic_cast<UnaryExpression*>(expression)) { 
        // check the operand expression and reject the explicit none ones.
        bool expr_valid = this->analyze_expression(unary_expr->expr);

        bool expr_none = this->analyze_none_operand(unary_expr->expr);

        return expr_valid && expr_none;
    } else if (auto* ref = dynamic_cast<Reference*>(expression)) { // Validate the reference and its nested expressions.
        bool ref_valid = this->analyze_reference(ref);

        return ref_valid;
    } else if (auto* is = dynamic_cast<Is*>(expression)) { // Validate the reference used by the type check.
        bool is_ref_valid = this->analyze_reference(is->ref);

        return is_ref_valid;
    } else if (auto* array = dynamic_cast<Array*>(expression)) {
        bool valid = true;

        // Recursively checking each element of the array. 
        // Identical type of elements us not required.
        for (Expression* expr : array->items) {
            if (!this->analyze_expression(expr)) {
                valid = false;
            }
        }

        return valid;
    } else if (auto* tuple = dynamic_cast<Tuple*>(expression)) { // check the expressions and uniqueness of the field names.
        bool valid = true;
        // Tracking field names within this tuple.
        std::unordered_set<std::string> seen_names;

        for (TupleElement* tuple_el : tuple->items) {
            if (tuple_el == nullptr) {
                this->report("Null tuple element");
                valid = false;
                continue;
            }

            // Only named elements participate in the duplicate-name check.
            if (tuple_el->identifier.has_value()) {
                const std::string& name = *(tuple_el->identifier);
                if (seen_names.find(name) != seen_names.end()) {
                    this->report(tuple_el->loc.first_line, tuple_el->loc.first_column, "Duplicate tuple element name: " + name);
                    valid = false;
                } else {
                    seen_names.insert(name);
                }
            }
            // Check the elements's expression even if its name is duplicated.
            if (!this->analyze_expression(tuple_el->expr)) {
                valid = false;
            }
        }

        return valid;
    } else if (auto* func_literal = dynamic_cast<FunctionLiteral*>(expression)) { // Check the function parameters and body in a new function context.
        bool valid = true;

        // Loops outside this function must not allow exit inside its body when the place where this exit is located inside the function is not in the loop when we consider the scope of this function.
        unsigned int saved_cycle_depth = this->cycle_depth;
        this->cycle_depth = 0;

        // Allow return statements.
        this->function_depth++;
        this->enter_scope();

        // Reject duplicate parameter names in this function's scope.
        for (std::string arg : func_literal->args) {
            if (this->is_declared_in_cur_scope(arg)) {
                this->report(func_literal->loc.first_line, func_literal->loc.first_column, "Duplicate function argument: " + arg);
                valid = false;
            } else {
                this->declare_variable(arg);
            }
        }
        // Check the expression body or the statement body.
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
        // After check the external context is being restored.
        this->exit_scope();
        this->function_depth--;
        
        this->cycle_depth = saved_cycle_depth;

        return valid;
    } else if (dynamic_cast<Literal*>(expression)) { // We accept liternal nodes because compound literals were checked above.
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
        } else if (auto* call = dynamic_cast<CallSuffix*>(suffix)) { // check each argument of the function
            for (Expression* arg : call->args) {
                if (!this->analyze_expression(arg)) {
                    valid = false;
                }
            }
        } else if (dynamic_cast<MemberSuffix*>(suffix) || dynamic_cast<TupleIndexSuffix*>(suffix)) { // Member existence and tuple index validity are not checked by this pass.
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