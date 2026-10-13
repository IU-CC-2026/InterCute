#include "ast.h"


std::string variableTypeToString(VariableType type) {
    switch (type) {
        case VariableType::INT:    return "int";
        case VariableType::REAL:   return "real";
        case VariableType::STRING: return "string";
        case VariableType::BOOL:   return "bool";
        case VariableType::ARRAY:  return "array";
        case VariableType::TUPLE:  return "tuple";
        case VariableType::FUNC:   return "func";
        case VariableType::NONE:   return "none";
    }

    return "?";
}


std::string binaryOperatorToString(BinaryOperator op) {
    switch (op) {
        case BinaryOperator::ADD: return "+";
        case BinaryOperator::SUB: return "-";
        case BinaryOperator::MUL: return "*";
        case BinaryOperator::DIV: return "/";
        case BinaryOperator::EQ:  return "=";
        case BinaryOperator::NEQ: return "/=";
        case BinaryOperator::LT:  return "<";
        case BinaryOperator::LE:  return "<=";
        case BinaryOperator::GT:  return ">";
        case BinaryOperator::GE:  return ">=";
        case BinaryOperator::AND: return "and";
        case BinaryOperator::OR:  return "or";
        case BinaryOperator::XOR: return "xor";
    }

    return "?";
}


std::string unaryOperatorToString(UnaryOperator op) {
    switch (op) {
        case UnaryOperator::PLUS:  return "+";
        case UnaryOperator::MINUS: return "-";
        case UnaryOperator::NOT:   return "not ";
    }

    return "?";
}


int bin_precedence(BinaryOperator op) {
    switch (op) {
        case BinaryOperator::OR:
        case BinaryOperator::AND:
        case BinaryOperator::XOR:
            return 1;

        case BinaryOperator::EQ:
        case BinaryOperator::NEQ:
        case BinaryOperator::LT:
        case BinaryOperator::LE:
        case BinaryOperator::GT:
        case BinaryOperator::GE:
            return 2;

        case BinaryOperator::ADD:
        case BinaryOperator::SUB:
            return 3;

        case BinaryOperator::MUL:
        case BinaryOperator::DIV:
            return 4;
    }

    return 0;
}


int unary_precedence(UnaryOperator op) {
    switch (op) {
        case UnaryOperator::NOT:
            return 5;

        case UnaryOperator::PLUS:
        case UnaryOperator::MINUS:
            return 6;
    }

    return 0;
}


void Expression::printWithParentPrecedence(int parent_precedence, bool is_right_child) {
    print();
}


// Expressions
BinaryExpression::BinaryExpression(Expression* left, Expression* right, BinaryOperator op)
    : left(left), right(right), op(op) {}

void BinaryExpression::print() {
    printWithParentPrecedence();
}

void BinaryExpression::printWithParentPrecedence(int parent_precedence, bool is_right_child) {
    int cur_op_precedence = bin_precedence(op);

    bool need_parentheses = cur_op_precedence < parent_precedence ||
        (cur_op_precedence == parent_precedence && is_right_child);

    if (need_parentheses) std::cout << "(";

    left->printWithParentPrecedence(cur_op_precedence, false);
    std::cout << " " << binaryOperatorToString(op) << " ";
    right->printWithParentPrecedence(cur_op_precedence, true);

    if (need_parentheses) std::cout << ")";
}


UnaryExpression::UnaryExpression(Expression* expr, UnaryOperator op)
    : expr(expr), op(op) {}

void UnaryExpression::print() {
    printWithParentPrecedence();
}

void UnaryExpression::printWithParentPrecedence(int parent_precedence, bool is_right_child) {
    int cur_op_precedence = unary_precedence(op);

    bool need_parentheses = cur_op_precedence < parent_precedence;

    if (need_parentheses) std::cout << "(";

    std::cout << unaryOperatorToString(op);
    expr->printWithParentPrecedence(cur_op_precedence, false);

    if (need_parentheses) std::cout << ")";
}


IntLiteral::IntLiteral(int value) : value(value) {}

void IntLiteral::print() {
    std::cout << value;
}


RealLiteral::RealLiteral(double value) : value(value) {}

void RealLiteral::print() {
    std::cout << value;
}


StringLiteral::StringLiteral(std::string value) : value(value) {}

void StringLiteral::print() {
    std::cout << value;
}


BoolLiteral::BoolLiteral(bool value) : value(value) {}

void BoolLiteral::print() {
    if (value) std::cout << "true";
    else std::cout << "false";
}


void NoneLiteral::print() {
    std::cout << "none";
}


BlockBody::BlockBody(std::vector<Statement*> statements)
    : statements(statements) {}

void BlockBody::print() {
    std::cout << "is" << '\n';
    for (int i = 0; i < statements.size(); ++i) {
        if (i > 0) std::cout << '\n';
        statements[i]->print();
    }

    std::cout << '\n' << "end";
}


ShortFunctionBody::ShortFunctionBody(Expression* expr) : expr(expr) {}

void ShortFunctionBody::print() {
    std::cout << "=> ";
    expr->print();
}


FunctionLiteral::FunctionLiteral(std::vector<std::string> args, FunctionBody* body)
    : args(args), body(body) {}

void FunctionLiteral::print() {
    std::cout << "func(";
    for (int i = 0; i < args.size(); i++) {
        std::cout << args[i];
        if (i != args.size() - 1)
            std::cout << ", ";
    }
    std::cout << ") ";

    body->print();
}


Array::Array(std::vector<Expression*> items) : items(items) {}

void Array::print() {
    std::cout << "[";
    for (int i = 0; i < items.size(); i++) {
        items[i]->print();
        if (i != items.size() - 1)
            std::cout << ", ";
    }
    std::cout << "]";
}


TupleElement::TupleElement(Expression* expr, std::optional<std::string> identifier)
    : identifier(identifier), expr(expr) {}

void TupleElement::print() {
    if (identifier.has_value()) {
        std::cout << *identifier << " := ";
    }
    expr->print();
}


Tuple::Tuple(std::vector<TupleElement*> items) : items(items) {}

void Tuple::print() {
    std::cout << "{";
    for (int i = 0; i < items.size(); i++) {
        items[i]->print();
        if (i != items.size() - 1)
            std::cout << ", ";
    }
    std::cout << "}";
}


ArrayIndexSuffix::ArrayIndexSuffix(Expression* index) : index(index) {}

void ArrayIndexSuffix::print() {
    std::cout << "[";
    index->print();
    std::cout << "]";
}


CallSuffix::CallSuffix(std::vector<Expression*> args) : args(args) {}

void CallSuffix::print() {
    std::cout << "(";
    for (int i = 0; i < args.size(); i++) {
        args[i]->print();
        if (i != args.size() - 1)
            std::cout << ", ";
    }
    std::cout << ")";
}


MemberSuffix::MemberSuffix(std::string identifier) : identifier(identifier) {}

void MemberSuffix::print() {
    std::cout << ".";
    std::cout << identifier;
}


TupleIndexSuffix::TupleIndexSuffix(IntLiteral* index) : index(index) {}

void TupleIndexSuffix::print() {
    std::cout << ".";
    index->print();
}


Reference::Reference(std::string identifier, std::vector<ReferenceSuffix*> suffixes)
    : identifier(identifier), suffixes(suffixes) {}

void Reference::print() {
    std::cout << identifier;
    for (ReferenceSuffix* suffix : suffixes) {
        suffix->print();
    }
}


VariableDefinition::VariableDefinition(std::string identifier, Expression* expr)
    : identifier(identifier), expr(expr) {}

void VariableDefinition::print() {
    std::cout << identifier;

    if (expr != nullptr) {
        std::cout << " := ";
        expr->print();
    }
}


Declaration::Declaration(std::vector<VariableDefinition*> definitions)
    : definitions(definitions) {}

void Declaration::print() {
    std::cout << "var ";
    for (int i = 0; i < definitions.size(); i++) {
        definitions[i]->print();
        if (i != definitions.size() - 1)
            std::cout << ", ";
    }
}


Is::Is(Reference* ref, VariableType type) : ref(ref), type(type) {}

void Is::print() {
    ref->print();
    std::cout << " is ";
    std::cout << variableTypeToString(type);
}



// Statements
Program::Program(std::vector<Statement*> statements) : statements(statements) {}

void Program::print() {
    for (int i = 0; i < statements.size(); i++) {
        if (i > 0) std::cout << '\n';
        statements[i]->print();
    }
}


If::If(Expression* condition, std::vector<Statement*> then_statements, std::optional<std::vector<Statement*>> else_statements)
    : condition(condition), then_statements(then_statements), else_statements(else_statements) {}

void If::print() {
    std::cout << "if ";
    condition->print();
    std::cout << " then" << '\n';

    for (int i = 0; i < then_statements.size(); i++) {
        if (i > 0) std::cout << '\n';
        then_statements[i]->print();
    }

    if (else_statements.has_value()) {
        std::cout << '\n' << "else" << '\n';
        for (int i = 0; i < else_statements->size(); i++) {
            if (i > 0) std::cout << '\n';
            (*else_statements)[i]->print();
        }
    }

    std::cout << '\n' << "end";
}


IfShort::IfShort(Expression* condition, std::vector<Statement*> statements)
    : condition(condition), statements(statements) {}

void IfShort::print() {
    std::cout << "if ";
    condition->print();
    std::cout << " =>" << '\n';

    for (int i = 0; i < statements.size(); ++i) {
        if (i > 0) std::cout << '\n';
        statements[i]->print();
    }
}


Loop::Loop(std::vector<Statement*> statements) : statements(statements) {}


WhileLoop::WhileLoop(Expression* condition, std::vector<Statement*> statements)
    : Loop(statements), condition(condition) {}

void WhileLoop::print() {
    std::cout << "while ";
    condition->print();
    std::cout << " loop" << '\n';

    for (int i = 0; i < statements.size(); ++i) {
        if (i > 0) std::cout << '\n';
        statements[i]->print();
    }

    std::cout << '\n' << "end";
}


ForEachLoop::ForEachLoop(Expression* expr, std::vector<Statement*> statements, std::optional<std::string> identifier)
    : Loop(statements), identifier(identifier), expr(expr) {}

void ForEachLoop::print() {
    std::cout << "for ";
    if (identifier.has_value()) {
        std::cout << *identifier << " in ";
    }
    expr->print();
    std::cout << " loop" << '\n';

    for (int i = 0; i < statements.size(); ++i) {
        if (i > 0) std::cout << '\n';
        statements[i]->print();
    }

    std::cout << '\n' << "end";
}


ForRangeLoop::ForRangeLoop(Expression* first_expr, Expression* second_expr, std::vector<Statement*> statements, std::optional<std::string> identifier)
    : Loop(statements), identifier(identifier), first_expr(first_expr), second_expr(second_expr) {}

void ForRangeLoop::print() {
    std::cout << "for ";
    if (identifier.has_value()) {
        std::cout << *identifier << " in ";
    }

    first_expr->print();
    std::cout << "..";
    second_expr->print();
    std::cout << " loop" << '\n';

    for (int i = 0; i < statements.size(); ++i) {
        if (i > 0) std::cout << '\n';
        statements[i]->print();
    }

    std::cout << '\n' << "end";
}


InfiniteLoop::InfiniteLoop(std::vector<Statement*> statements)
    : Loop(statements) {}

void InfiniteLoop::print() {
    std::cout << "loop" << '\n';

    for (int i = 0; i < statements.size(); ++i) {
        if (i > 0) std::cout << '\n';
        statements[i]->print();
    }

    std::cout << '\n' << "end";
}


void Exit::print() {
    std::cout << "exit";
}


Return::Return(Expression* result) : result(result) {}

void Return::print() {
    std::cout << "return";
    if (result != nullptr) {
        std::cout << " ";
        result->print();
    }
}


Print::Print(std::vector<Expression*> exprs) : exprs(exprs) {}

void Print::print() {
    std::cout << "print";
    for (int i = 0; i < exprs.size(); i++) {
        if (i > 0) std::cout << ", ";
        else std::cout << " ";

        exprs[i]->print();
    }
}


Assignment::Assignment(Reference* ref, Expression* expr) : ref(ref), expr(expr) {}

void Assignment::print() {
    ref->print();
    std::cout << " := ";
    expr->print();
}
