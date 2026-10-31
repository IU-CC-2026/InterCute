#include "ast.h"

#include <string>


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


// Expressions
BinaryExpression::BinaryExpression(Expression* left, Expression* right, BinaryOperator op)
    : left(left), right(right), op(op) {}

std::string BinaryExpression::print(int level) {
    std::string pad(level, '\t');

    return pad + "BinaryExpression\n" +
    pad + "\toperator: " + binaryOperatorToString(op) + '\n' +
    pad + "\tleft:\n" + left->print(level + 2) + '\n' +
    pad + "\tright:\n" + right->print(level + 2);
}


UnaryExpression::UnaryExpression(Expression* expr, UnaryOperator op)
    : expr(expr), op(op) {}

std::string UnaryExpression::print(int level) {
    std::string pad(level, '\t');

    return pad + "UnaryExpression\n" +
    pad + "\toperator: " + unaryOperatorToString(op) + '\n' +
    pad + "\texpression:\n" + expr->print(level + 2);
}


IntLiteral::IntLiteral(int value) : value(value) {}

std::string IntLiteral::print(int level) {
    std::string pad(level, '\t');

    return pad + "IntLiteral: " + std::to_string(value);
}


RealLiteral::RealLiteral(double value) : value(value) {}

std::string RealLiteral::print(int level) {
    std::string pad(level, '\t');

    return pad + "RealLiteral: " + std::to_string(value);
}


StringLiteral::StringLiteral(std::string value) : value(value) {}

std::string StringLiteral::print(int level) {
    std::string pad(level, '\t');

    return pad + "StringLiteral: " + value;
}


BoolLiteral::BoolLiteral(bool value) : value(value) {}

std::string BoolLiteral::print(int level) {
    std::string pad(level, '\t');

    return pad + std::string("BoolLiteral: ") + (value ? "true" : "false");
}


std::string NoneLiteral::print(int level) {
    std::string pad(level, '\t');

    return pad + "NoneLiteral: none";
}


BlockBody::BlockBody(std::vector<Statement*> statements)
    : statements(statements) {}

std::string BlockBody::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "BlockBody\n";
    result += pad + "\tstatements:";

    if (statements.empty()) result += " []";
    else {
        for (int i = 0; i < statements.size(); i++) {
            result += '\n' + statements[i]->print(level + 2);
        }
    }

    return result;
}


ShortFunctionBody::ShortFunctionBody(Expression* expr) : expr(expr) {}

std::string ShortFunctionBody::print(int level) {
    std::string pad(level, '\t');

    return pad + "ShortFunctionBody\n" + expr->print(level + 1);
}


FunctionLiteral::FunctionLiteral(std::vector<std::string> args, FunctionBody* body)
    : args(args), body(body) {}

std::string FunctionLiteral::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "FunctionLiteral\n";
    result += pad + "\targuments:";

    if (args.empty()) result += " []";
    else {
        for (int i = 0; i < args.size(); i++) {
            result += '\n' + pad + "\t\t" + args[i];
        }
    }
    
    result += '\n' + pad + "\tbody:\n" + body->print(level + 2);

    return result;
}


Array::Array(std::vector<Expression*> items) : items(items) {}

std::string Array::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "Array\n";
    result += pad + "\titems:";

    if (items.empty()) result += " []";
    else {
        for (int i = 0; i < items.size(); i++) {
            result += "\n" + items[i]->print(level + 2);
        }
    }
    
    return result;
}


TupleElement::TupleElement(Expression* expr, std::optional<std::string> identifier)
    : identifier(identifier), expr(expr) {}

std::string TupleElement::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "TupleElement\n";
    if (identifier.has_value()) {
        result += pad + "\tidentifier: " + *identifier + '\n';
    }
    result += pad + "\texpression:\n" + expr->print(level + 2);

    return result;
}


Tuple::Tuple(std::vector<TupleElement*> items) : items(items) {}

std::string Tuple::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "Tuple\n";
    result += pad + "\titems:";

    if (items.empty()) result += " []";
    else {
        for (int i = 0; i < items.size(); i++) {
            result += "\n" + items[i]->print(level + 2);
        }
    }
    
    return result;
}


ArrayIndexSuffix::ArrayIndexSuffix(Expression* index) : index(index) {}

std::string ArrayIndexSuffix::print(int level) {
    std::string pad(level, '\t');

    return pad + "ArrayIndexSuffix\n" + 
    pad + "\tindex:\n" + index->print(level + 2);
}


CallSuffix::CallSuffix(std::vector<Expression*> args) : args(args) {}

std::string CallSuffix::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "CallSuffix\n";
    result += pad + "\targuments:";

    if (args.empty()) result += " []";
    else {
        for (int i = 0; i < args.size(); i++) {
            result += "\n" + args[i]->print(level + 2);
        }
    }

    return result;
}


MemberSuffix::MemberSuffix(std::string identifier) : identifier(identifier) {}

std::string MemberSuffix::print(int level) {
    std::string pad(level, '\t');

    return pad + "MemberSuffix\n" + pad + "\tidentifier: " + identifier;
}


TupleIndexSuffix::TupleIndexSuffix(IntLiteral* index) : index(index) {}

std::string TupleIndexSuffix::print(int level) {
    std::string pad(level, '\t');

    return pad + "TupleIndexSuffix\n" + pad + "\tindex:\n"
    + index->print(level + 2);
}


Reference::Reference(std::string identifier, std::vector<ReferenceSuffix*> suffixes)
    : identifier(identifier), suffixes(suffixes) {}

std::string Reference::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "Reference\n";
    result += pad + "\tidentifier: " + identifier;
    if (!suffixes.empty()) {
        result += '\n' + pad + "\tsuffixes:";

        for (int i = 0; i < suffixes.size(); i++) {
            result += "\n" + suffixes[i]->print(level + 2);
        }
    }

    return result;
}


VariableDefinition::VariableDefinition(std::string identifier, Expression* expr)
    : identifier(identifier), expr(expr) {}

std::string VariableDefinition::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "VariableDefinition\n";
    result += pad + "\tidentifier: " + identifier;

    if (expr != nullptr) {
        result += '\n' + pad + "\texpression:\n" + expr->print(level + 2);
    }

    return result;
}


Declaration::Declaration(std::vector<VariableDefinition*> definitions)
    : definitions(definitions) {}

std::string Declaration::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "Declaration\n";
    result += pad + "\tdefinitions:";

    if (definitions.empty()) result += " []";
    else {
        for (int i = 0; i < definitions.size(); i++) {
            result += '\n' + definitions[i]->print(level + 2);
        }
    }
    
    return result;
}


Is::Is(Reference* ref, VariableType type) : ref(ref), type(type) {}

std::string Is::print(int level) {
    std::string pad(level, '\t');

    return pad + "Is\n" + pad + "\treference:\n" + ref->print(level + 2) +
    '\n' + pad + "\ttype: " + variableTypeToString(type);
}


// Statements
Program::Program(std::vector<Statement*> statements) : statements(statements) {}

std::string Program::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "Program\n";
    result += pad + "\tstatements:";

    if (statements.empty()) result += " []";
    else {
        for (int i = 0; i < statements.size(); i++) {
            result += '\n' + statements[i]->print(level + 2);
        }
    }

    return result;
}


If::If(Expression* condition, std::vector<Statement*> then_statements, std::optional<std::vector<Statement*>> else_statements)
    : condition(condition), then_statements(then_statements), else_statements(else_statements) {}

std::string If::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "If\n";
    result += pad + "\tcondition:\n" + condition->print(level + 2);
    
    result += '\n' + pad + "\tthen:";

    if (then_statements.empty()) result += " []";
    else {
        for (int i = 0; i < then_statements.size(); i++) {
            result += "\n" + then_statements[i]->print(level + 2);
        }
    }

    if (else_statements.has_value()) {
        result += '\n' + pad + "\telse:";

        if ((*else_statements).empty()) result += " []";
        else {
            for (int i = 0; i < else_statements->size(); i++) {
                result += "\n" + (*else_statements)[i]->print(level + 2);
            }
        }
    }

    return result;
}


IfShort::IfShort(Expression* condition, std::vector<Statement*> statements)
    : condition(condition), statements(statements) {}

std::string IfShort::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "IfShort\n";
    result += pad + "\tcondition:\n" + condition->print(level + 2);
    
    result += '\n' + pad + "\tstatements:";

    if (statements.empty()) result += " []";
    else {
        for (int i = 0; i < statements.size(); i++) {
            result += "\n" + statements[i]->print(level + 2);
        }
    }

    return result;
}


Loop::Loop(std::vector<Statement*> statements) : statements(statements) {}


WhileLoop::WhileLoop(Expression* condition, std::vector<Statement*> statements)
    : Loop(statements), condition(condition) {}

std::string WhileLoop::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "WhileLoop\n";
    result += pad + "\tcondition:\n" + condition->print(level + 2);

    result += '\n' + pad + "\tbody:";

    if (statements.empty()) result += " []";
    else {
        for (int i = 0; i < statements.size(); i++) {
            result += "\n" + statements[i]->print(level + 2);
        }
    }

    return result;
}


ForEachLoop::ForEachLoop(Expression* expr, std::vector<Statement*> statements, std::optional<std::string> identifier)
    : Loop(statements), identifier(identifier), expr(expr) {}

std::string ForEachLoop::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "ForEachLoop\n";
    if (identifier.has_value())
        result += pad + "\tidentifier: " + *identifier + '\n';

    result += pad + "\texpr:\n" + expr->print(level + 2);

    result += '\n' + pad + "\tbody:";

    if (statements.empty()) result += " []";
    else {
        for (int i = 0; i < statements.size(); i++) {
            result += "\n" + statements[i]->print(level + 2);
        }
    }

    return result;
}


ForRangeLoop::ForRangeLoop(Expression* first_expr, Expression* second_expr, std::vector<Statement*> statements, std::optional<std::string> identifier)
    : Loop(statements), identifier(identifier), first_expr(first_expr), second_expr(second_expr) {}

std::string ForRangeLoop::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "ForRangeLoop\n";
    if (identifier.has_value())
        result += pad + "\tidentifier: " + *identifier + '\n';

    result += pad + "\tfirst_expr:\n" + first_expr->print(level + 2);
    result += '\n' + pad + "\tsecond_expr:\n" + second_expr->print(level + 2);

    result += '\n' + pad + "\tbody:";

    if (statements.empty()) result += " []";
    else {
        for (int i = 0; i < statements.size(); i++) {
            result += "\n" + statements[i]->print(level + 2);
        }
    }

    return result;
}


InfiniteLoop::InfiniteLoop(std::vector<Statement*> statements)
    : Loop(statements) {}

std::string InfiniteLoop::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "InfiniteLoop";

    result += '\n' + pad + "\tbody:";

    if (statements.empty()) result += " []";
    else {
        for (int i = 0; i < statements.size(); i++) {
            result += "\n" + statements[i]->print(level + 2);
        }
    }

    return result;
}


std::string Exit::print(int level) {
    std::string pad(level, '\t');

    return pad + "Exit";
}


Return::Return(Expression* result) : result(result) {}

std::string Return::print(int level) {
    std::string pad(level, '\t');

    std::string result_str = pad + "Return";

    if (result != nullptr) {
        result_str += '\n' + pad + "\tresult:\n" + result->print(level + 2);
    }

    return result_str;
}


Print::Print(std::vector<Expression*> exprs) : exprs(exprs) {}

std::string Print::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "Print\n";
    result += pad + "\texpressions:";

    if (exprs.empty()) result += " []";
    else {
        for (int i = 0; i < exprs.size(); i++) {
            result += "\n" + exprs[i]->print(level + 2);
        }
    }

    return result;
}


Assignment::Assignment(Reference* ref, Expression* expr) : ref(ref), expr(expr) {}

std::string Assignment::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "Assignment\n";

    result += pad + "\treference:\n" + ref->print(level + 2);
    result += '\n' + pad + "\texpression:\n" + expr->print(level + 2);

    return result;
}

Block::Block(std::vector<Statement*> statements) : statements(statements) {}

std::string Block::print(int level) {
    std::string pad(level, '\t');

    std::string result = pad + "Block\n";
    result += pad + "\tstatements:";

    if (statements.empty()) result += " []";
    else {
        for (int i = 0; i < statements.size(); i++) {
            result += '\n' + statements[i]->print(level + 2);
        }
    }

    return result;
}
