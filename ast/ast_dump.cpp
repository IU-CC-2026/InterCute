#include "ast_dump.h"
#include <iomanip>
#include <limits>
#include <sstream>
#include <stdexcept>

namespace {
void line(std::ostream& out, int depth, const std::string& text) {
    out << std::string(depth * 2, ' ') << text << '\n';
}


std::string escaped(const std::string& value) {
    std::ostringstream out;
    for (unsigned char c : value) {
        switch (c) {
            case '\n': out << "\\n"; break;
            case '\r': out << "\\r"; break;
            case '\t': out << "\\t"; break;
            case '\\': out << "\\\\"; break;
            default:
                if (c < 32 || c == 127)
                    out << "\\x" << std::hex << std::setw(2)
                        << std::setfill('0') << static_cast<int>(c) << std::dec;
                else out << c;
        }
    }
    return out.str();
}

void child(std::ostream& out, int depth, const std::string& role, const ASTNode* node) {
    line(out, depth, role);
    dump_ast(node, out, depth + 1);
}

template<class T>
void children(std::ostream& out, int depth, const std::vector<T*>& nodes) {
    for (const auto* node : nodes) dump_ast(node, out, depth);
}

template<class T>
void group(std::ostream& out, int depth, const std::string& role,
           const std::vector<T*>& nodes) {
    line(out, depth, role + (nodes.empty() ? " (empty)" : ""));
    children(out, depth + 1, nodes);
}
}

void dump_ast(const ASTNode* node, std::ostream& out, int depth) {
    if (!node) { line(out, depth, "(absent)"); return; }

    if (const auto* n = dynamic_cast<const Program*>(node)) {
        line(out, depth, "Program");
        children(out, depth + 1, n->statements);

    } else if (const auto* n = dynamic_cast<const BinaryExpression*>(node)) {
        line(out, depth, "BinaryExpression (" + binaryOperatorToString(n->op) + ")");
        children(out, depth + 1, std::vector<Expression*>{n->left, n->right});

    } else if (const auto* n = dynamic_cast<const UnaryExpression*>(node)) {
        std::string op = unaryOperatorToString(n->op);
        if (!op.empty() && op.back() == ' ') op.pop_back();
        line(out, depth, "UnaryExpression (" + op + ")");
        dump_ast(n->expr, out, depth + 1);

    } else if (const auto* n = dynamic_cast<const IntLiteral*>(node)) {
        line(out, depth, "IntLiteral (" + std::to_string(n->value) + ")");

    } else if (const auto* n = dynamic_cast<const RealLiteral*>(node)) {
        std::ostringstream value;
        value << std::setprecision(std::numeric_limits<double>::max_digits10) << n->value;
        line(out, depth, "RealLiteral (" + value.str() + ")");

    } else if (const auto* n = dynamic_cast<const StringLiteral*>(node)) {
        line(out, depth, "StringLiteral (" + escaped(n->value) + ")");

    } else if (const auto* n = dynamic_cast<const BoolLiteral*>(node)) {
        line(out, depth, std::string("BoolLiteral (") + (n->value ? "true)" : "false)"));

    } else if (dynamic_cast<const NoneLiteral*>(node)) {
        line(out, depth, "NoneLiteral");

    } else if (const auto* n = dynamic_cast<const Reference*>(node)) {
        line(out, depth, "Reference (" + n->identifier + ")");
        children(out, depth + 1, n->suffixes);

    } else if (const auto* n = dynamic_cast<const ArrayIndexSuffix*>(node)) {
        line(out, depth, "ArrayIndexSuffix");
        dump_ast(n->index, out, depth + 1);

    } else if (const auto* n = dynamic_cast<const CallSuffix*>(node)) {
        line(out, depth, "CallSuffix");
        group(out, depth + 1, "Arguments", n->args);

    } else if (const auto* n = dynamic_cast<const MemberSuffix*>(node)) {
        line(out, depth, "MemberSuffix (" + n->identifier + ")");

    } else if (const auto* n = dynamic_cast<const TupleIndexSuffix*>(node)) {
        line(out, depth, "TupleIndexSuffix");
        dump_ast(n->index, out, depth + 1);

    } else if (const auto* n = dynamic_cast<const Array*>(node)) {
        line(out, depth, n->items.empty() ? "Array (empty)" : "Array");
        children(out, depth + 1, n->items);

    } else if (const auto* n = dynamic_cast<const Tuple*>(node)) {
        line(out, depth, n->items.empty() ? "Tuple (empty)" : "Tuple");
        children(out, depth + 1, n->items);

    } else if (const auto* n = dynamic_cast<const TupleElement*>(node)) {
        line(out, depth, "TupleElement" + (n->identifier ? " (" + *n->identifier + ")" : " (unnamed)"));
        dump_ast(n->expr, out, depth + 1);

    } else if (const auto* n = dynamic_cast<const FunctionLiteral*>(node)) {
        line(out, depth, "FunctionLiteral");
        line(out, depth + 1, n->args.empty() ? "Parameters (empty)" : "Parameters");
        for (const auto& arg : n->args) line(out, depth + 2, arg);
        dump_ast(n->body, out, depth + 1);

    } else if (const auto* n = dynamic_cast<const BlockBody*>(node)) {
        line(out, depth, "BlockBody");
        children(out, depth + 1, n->statements);

    } else if (const auto* n = dynamic_cast<const ShortFunctionBody*>(node)) {
        line(out, depth, "ShortFunctionBody");
        dump_ast(n->expr, out, depth + 1);

    } else if (const auto* n = dynamic_cast<const Is*>(node)) {
        line(out, depth, "Is (" + variableTypeToString(n->type) + ")");
        dump_ast(n->ref, out, depth + 1);

    } else if (const auto* n = dynamic_cast<const VariableDefinition*>(node)) {
        line(out, depth, "VariableDefinition (" + n->identifier + ")");
        if (n->expr) child(out, depth + 1, "Initializer", n->expr);
        else line(out, depth + 1, "Initializer (absent)");

    } else if (const auto* n = dynamic_cast<const Declaration*>(node)) {
        line(out, depth, "Declaration");
        children(out, depth + 1, n->definitions);

    } else if (const auto* n = dynamic_cast<const Assignment*>(node)) {
        line(out, depth, "Assignment");
        child(out, depth + 1, "Target", n->ref);
        child(out, depth + 1, "Value", n->expr);

    } else if (const auto* n = dynamic_cast<const Print*>(node)) {
        line(out, depth, "Print");
        children(out, depth + 1, n->exprs);

    } else if (const auto* n = dynamic_cast<const Return*>(node)) {
        line(out, depth, "Return");
        if (n->result) dump_ast(n->result, out, depth + 1);
        else line(out, depth + 1, "Value (absent)");

    } else if (dynamic_cast<const Exit*>(node)) {
        line(out, depth, "Exit");

    } else if (const auto* n = dynamic_cast<const If*>(node)) {
        line(out, depth, "If");
        child(out, depth + 1, "Condition", n->condition);
        group(out, depth + 1, "Then", n->then_statements);
        if (n->else_statements) group(out, depth + 1, "Else", *n->else_statements);

    } else if (const auto* n = dynamic_cast<const IfShort*>(node)) {
        line(out, depth, "IfShort");
        child(out, depth + 1, "Condition", n->condition);
        group(out, depth + 1, "Body", n->statements);

    } else if (const auto* n = dynamic_cast<const WhileLoop*>(node)) {
        line(out, depth, "WhileLoop");
        child(out, depth + 1, "Condition", n->condition);
        group(out, depth + 1, "Body", n->statements);

    } else if (const auto* n = dynamic_cast<const ForRangeLoop*>(node)) {
        line(out, depth, "ForRangeLoop" + (n->identifier ? " (" + *n->identifier + ")" : ""));
        child(out, depth + 1, "From", n->first_expr);
        child(out, depth + 1, "To", n->second_expr);
        group(out, depth + 1, "Body", n->statements);

    } else if (const auto* n = dynamic_cast<const ForEachLoop*>(node)) {
        line(out, depth, "ForEachLoop" + (n->identifier ? " (" + *n->identifier + ")" : ""));
        child(out, depth + 1, "Collection", n->expr);
        group(out, depth + 1, "Body", n->statements);

    } else if (const auto* n = dynamic_cast<const InfiniteLoop*>(node)) {
        line(out, depth, "InfiniteLoop");
        group(out, depth + 1, "Body", n->statements);
        
    } else {
        throw std::runtime_error("AST dump: unsupported node type");
    }
}
