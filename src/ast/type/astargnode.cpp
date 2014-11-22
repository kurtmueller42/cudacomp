/**
 * @file astargnode.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astargnode.h>

ASTArgNode::ASTArgNode(std::shared_ptr<ASTTypeNode> type, std::string name)
    : type(type),
      name(name)
{
}

std::shared_ptr<ASTTypeNode> ASTArgNode::getType() {
    return type;
}

std::string ASTArgNode::getName() {
    return name;
}

void ASTArgNode::print(std::ostream & ss) {
    ss << "param(";
    type->print(ss);
    ss << ", " << name << ")";
}