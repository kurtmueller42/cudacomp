/**
 * @file astallocexp.cpp
 *
 * @author Kurt Mueller <kurtmueller42@gmail.com>
 */

#include <ast/expr/astallocexp.h>

ASTAllocExp::ASTAllocExp(std::shared_ptr<ASTTypeNode> type)
    : type(type)
{
}

std::shared_ptr<ASTTypeNode> ASTAllocExp::getElemType() {
    return type;
}


void ASTAllocExp::print(std::ostream & ss) {
    // TODO
}
