#include "optimizer.h"


void Optimizer::optimize(Program* root) {
    if (root == nullptr) return;

    for (Statement*& statement : root->statements) {
        this->fold_statement(statement);
    }
}


void Optimizer::fold_statement(Statement*& statement) {
    if (statement == nullptr) return;

    if (auto* declaration = dynamic_cast<Declaration*>(statement)) {
        for (VariableDefinition* vardef : declaration->definitions) {
            if (vardef == nullptr) continue;

            this->fold_expression(vardef->expr);
        }
    } else if (auto* print = dynamic_cast<Print*>(statement)) {
        for (Expression*& expr : print->exprs) {
            this->fold_expression(expr);
        }
    } else if (auto* assignment = dynamic_cast<Assignment*>(statement)) {
        this->fold_expression(assignment->expr);
        
        this->fold_reference(assignment->ref);
    } else if (auto* if_st = dynamic_cast<If*>(statement)) {
        this->fold_expression(if_st->condition);

        for (Statement*& statement : if_st->then_statements) {
            this->fold_statement(statement);
        }

        if (if_st->else_statements.has_value()) {
            for (Statement*& statement : *(if_st->else_statements)) {
                this->fold_statement(statement);
            }
        }
    } else if (auto* if_short = dynamic_cast<IfShort*>(statement)) {
        this->fold_expression(if_short->condition);

        for (Statement*& statement : if_short->statements) {
            this->fold_statement(statement);
        }
    } else if (auto* while_loop = dynamic_cast<WhileLoop*>(statement)) {
        this->fold_expression(while_loop->condition);

        for (Statement*& statement : while_loop->statements) {
            this->fold_statement(statement);
        }
    } else if (auto* for_each_loop = dynamic_cast<ForEachLoop*>(statement)) {
        this->fold_expression(for_each_loop->expr);

        for (Statement*& statement : for_each_loop->statements) {
            this->fold_statement(statement);
        }
    } else if (auto* for_range_loop = dynamic_cast<ForRangeLoop*>(statement)) {
        this->fold_expression(for_range_loop->first_expr);
        this->fold_expression(for_range_loop->second_expr);

        for (Statement*& statement : for_range_loop->statements) {
            this->fold_statement(statement);
        }
    } else if (auto* inf_loop = dynamic_cast<InfiniteLoop*>(statement)) {
        for (Statement*& statement : inf_loop->statements) {
            this->fold_statement(statement);
        }
    } else if (auto* return_st = dynamic_cast<Return*>(statement)) {
        this->fold_expression(return_st->result);
    }
}


void Optimizer::fold_expression(Expression*& expression) {
    if (expression == nullptr) return;

    if (auto* bin_expr = dynamic_cast<BinaryExpression*>(expression)) {
        this->fold_expression(bin_expr->left);
        this->fold_expression(bin_expr->right);

        if (Expression* folded = try_fold_binary(bin_expr)) {
            delete bin_expr->left;
            delete bin_expr->right;
            delete bin_expr;

            expression = folded;
        }
    } else if (auto* unary_expr = dynamic_cast<UnaryExpression*>(expression)) {
        this->fold_expression(unary_expr->expr);

        if (Expression* folded = try_fold_unary(unary_expr)) {
            delete unary_expr->expr;
            delete unary_expr;

            expression = folded;
        }
    } else if (auto* ref = dynamic_cast<Reference*>(expression)) {
        this->fold_reference(ref);
    } else if (auto* is = dynamic_cast<Is*>(expression)) {
        this->fold_reference(is->ref);
    } else if (auto* array = dynamic_cast<Array*>(expression)) {
        for (Expression*& expr : array->items) {
            this->fold_expression(expr);
        }
    } else if (auto* tuple = dynamic_cast<Tuple*>(expression)) {
        for (TupleElement* tuple_el : tuple->items) {
            if (tuple_el == nullptr) continue;

            this->fold_expression(tuple_el->expr);
        }
    } else if (auto* func_literal = dynamic_cast<FunctionLiteral*>(expression)) {
        if (auto* short_body = dynamic_cast<ShortFunctionBody*>(func_literal->body)) {
            this->fold_expression(short_body->expr);
        } else if (auto* block_body = dynamic_cast<BlockBody*>(func_literal->body)) {
            for (Statement*& statement : block_body->statements) {
                this->fold_statement(statement);
            }
        }
    }
}


void Optimizer::fold_reference(Reference* reference) {
    if (reference == nullptr) return;

    for (ReferenceSuffix* suffix : reference->suffixes) {
        if (auto* array_idx = dynamic_cast<ArrayIndexSuffix*>(suffix)) {
            this->fold_expression(array_idx->index);
        } else if (auto* call = dynamic_cast<CallSuffix*>(suffix)) {
            for (Expression*& arg : call->args) {
                this->fold_expression(arg);
            }
        }
    }
}


Expression* Optimizer::fold_numeric(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc) {
    if (left == nullptr || right == nullptr) return nullptr;

    auto* li = dynamic_cast<IntLiteral*>(left);
    auto* ri = dynamic_cast<IntLiteral*>(right);
    auto* lr = dynamic_cast<RealLiteral*>(left);
    auto* rr = dynamic_cast<RealLiteral*>(right);

    bool left_is_num = (li != nullptr) || (lr != nullptr);
    bool right_is_num = (ri != nullptr) || (rr != nullptr);
    if (!left_is_num || !right_is_num) return nullptr;

    if (li && ri) {
        IntLiteral* folded;

        switch (op) {
            case BinaryOperator::ADD:
                folded = new IntLiteral(li->value + ri->value);
                break;
            
            case BinaryOperator::SUB:
                folded = new IntLiteral(li->value - ri->value);
                break;

            case BinaryOperator::MUL:
                folded = new IntLiteral(li->value * ri->value);
                break;

            case BinaryOperator::DIV:
                if (ri->value == 0) return nullptr;
                folded = new IntLiteral(li->value / ri->value);
                break;

            default:
                return nullptr;
        }

        folded->loc = loc;

        return folded;
    } else {
        auto left_val = li ? li->value : lr->value;
        auto right_val = ri ? ri->value : rr->value;

        RealLiteral* folded;

        switch (op) {
            case BinaryOperator::ADD:
                folded = new RealLiteral(left_val + right_val);
                break;
            
            case BinaryOperator::SUB:
                folded = new RealLiteral(left_val - right_val);
                break;

            case BinaryOperator::MUL:
                folded = new RealLiteral(left_val * right_val);
                break;

            case BinaryOperator::DIV:
                if (right_val == 0) return nullptr;
                folded = new RealLiteral(left_val / right_val);
                break;

            default:
                return nullptr;
        }

        folded->loc = loc;

        return folded;
    }

    return nullptr;
}


Expression* Optimizer::fold_comparison(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc) {
    if (left == nullptr || right == nullptr) return nullptr;

    auto* li = dynamic_cast<IntLiteral*>(left);
    auto* ri = dynamic_cast<IntLiteral*>(right);
    auto* lr = dynamic_cast<RealLiteral*>(left);
    auto* rr = dynamic_cast<RealLiteral*>(right);

    bool left_is_num = (li != nullptr) || (lr != nullptr);
    bool right_is_num = (ri != nullptr) || (rr != nullptr);
    if (!left_is_num || !right_is_num) return nullptr;

    double left_val = li ? li->value : lr->value;
    double right_val = ri ? ri->value : rr->value;

    BoolLiteral* folded;

    switch (op) {
        case BinaryOperator::EQ:
            folded = new BoolLiteral(left_val == right_val);
            break;
        
        case BinaryOperator::NEQ:
            folded = new BoolLiteral(left_val != right_val);
            break;

        case BinaryOperator::LT:
            folded = new BoolLiteral(left_val < right_val);
            break;

        case BinaryOperator::LE:
            folded = new BoolLiteral(left_val <= right_val);
            break;

        case BinaryOperator::GT:
            folded = new BoolLiteral(left_val > right_val);
            break;

        case BinaryOperator::GE:
            folded = new BoolLiteral(left_val >= right_val);
            break;
            
        default:
            return nullptr;
    }

    folded->loc = loc;

    return folded;
}


Expression* Optimizer::fold_logical(Expression* left, Expression* right, BinaryOperator op, SourceLocation loc) {
    if (left == nullptr || right == nullptr) return nullptr;

    auto* left_bool = dynamic_cast<BoolLiteral*>(left);
    auto* right_bool = dynamic_cast<BoolLiteral*>(right);
    if (!left_bool || !right_bool) return nullptr;

    BoolLiteral* folded;

    switch (op) {
        case BinaryOperator::AND:
            folded = new BoolLiteral(left_bool->value && right_bool->value);
            break;
        
        case BinaryOperator::OR:
            folded = new BoolLiteral(left_bool->value || right_bool->value);
            break;

        case BinaryOperator::XOR:
            folded = new BoolLiteral(left_bool->value ^ right_bool->value);
            break;
            
        default:
            return nullptr;
    }

    folded->loc = loc;

    return folded;
}


Expression* Optimizer::try_fold_binary(BinaryExpression* bin) {
    if (bin == nullptr || bin->left == nullptr || bin->right == nullptr)
        return nullptr;

    switch (bin->op) {
        case BinaryOperator::ADD:
            if (auto* folded = this->fold_numeric(bin->left, bin->right, bin->op, bin->loc)) {
                return folded;
            } else if (auto* left_str = dynamic_cast<StringLiteral*>(bin->left)) {
                if (auto* right_str = dynamic_cast<StringLiteral*>(bin->right)) {
                    StringLiteral* folded = new StringLiteral(left_str->value + right_str->value);
                    folded->loc = bin->loc;
                    return folded;
                }
            } else if (auto* left_arr = dynamic_cast<Array*>(bin->left)) {
                if (auto* right_arr = dynamic_cast<Array*>(bin->right)) {
                    std::vector<Expression*> merged;
                    for (Expression*& expr : left_arr->items) {
                        if (expr != nullptr)
                            merged.push_back(expr);
                    }
                    for (Expression*& expr : right_arr->items) {
                        if (expr != nullptr)
                            merged.push_back(expr);
                    }

                    Array* folded = new Array(merged);
                    folded->loc = bin->loc;
                    return folded;
                }
            } else if (auto* left_tup = dynamic_cast<Tuple*>(bin->left)) {
                if (auto* right_tup = dynamic_cast<Tuple*>(bin->right)) {
                    std::vector<TupleElement*> merged;
                    for (TupleElement*& tuple_el : left_tup->items) {
                        if (tuple_el != nullptr)
                            merged.push_back(tuple_el);
                    }
                    for (TupleElement*& tuple_el : right_tup->items) {
                        if (tuple_el != nullptr)
                            merged.push_back(tuple_el);
                    }

                    Tuple* folded = new Tuple(merged);
                    folded->loc = bin->loc;
                    return folded;
                }
            }

            return nullptr;

        case BinaryOperator::SUB:
        case BinaryOperator::MUL:
        case BinaryOperator::DIV:
            return this->fold_numeric(bin->left, bin->right, bin->op, bin->loc);

        case BinaryOperator::EQ:
        case BinaryOperator::NEQ:
        case BinaryOperator::LT:
        case BinaryOperator::LE:
        case BinaryOperator::GT:
        case BinaryOperator::GE:
            return this->fold_comparison(bin->left, bin->right, bin->op, bin->loc);

        case BinaryOperator::AND:
        case BinaryOperator::OR:
        case BinaryOperator::XOR:
            return this->fold_logical(bin->left, bin->right, bin->op, bin->loc);

        default:
            return nullptr;
    }

    return nullptr;
}


Expression* Optimizer::try_fold_unary(UnaryExpression* unary) {
    if (unary == nullptr || unary->expr == nullptr)
        return nullptr;

    switch (unary->op) {
        case UnaryOperator::PLUS:
        case UnaryOperator::MINUS: {
            int sign = (unary->op == UnaryOperator::MINUS) ? -1 : 1;

            if (auto* int_lit = dynamic_cast<IntLiteral*>(unary->expr)) {
                IntLiteral* folded = new IntLiteral(sign * int_lit->value);
                folded->loc = unary->loc;
                return folded;
            } else if (auto* real_lit = dynamic_cast<RealLiteral*>(unary->expr)) {
                RealLiteral* folded = new RealLiteral(sign * real_lit->value);
                folded->loc = unary->loc;
                return folded;
            }

            return nullptr;
        }

        case UnaryOperator::NOT:
            if (auto* bool_lit = dynamic_cast<BoolLiteral*>(unary->expr)) {
                BoolLiteral* folded = new BoolLiteral(!bool_lit->value);
                folded->loc = unary->loc;
                return folded;
            }

            return nullptr;

        default:
            return nullptr;
    }

    return nullptr;
}