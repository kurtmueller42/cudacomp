/**
 * @file astidtype.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/type/astidtype.h>

ASTIdType::ASTIdType(std::string id)
    : id(id)
{
}

std::string ASTIdType::getId() {
    return id;
}

bool ASTIdType::equal(std::shared_ptr<ASTTypeNode> other_type) {
    return false; // TODO
}

void ASTIdType::print(std::ostream & ss) {
    ss << id;
}

int ASTIdType::getSize() {
    throw std::runtime_error("ID types do not have a size");
}
