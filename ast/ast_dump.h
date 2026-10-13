#pragma once
#include "ast.h"
#include <ostream>

// Print node types and children. This does not evaluate or modify the AST.
void dump_ast(const ASTNode* node, std::ostream& out, int depth = 0);
