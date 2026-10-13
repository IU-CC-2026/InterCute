#pragma once


#include "ast.h"
#include <ostream>


void dump_ast(const ASTNode* node, std::ostream& out, int depth = 0);
