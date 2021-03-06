/**
 * @file astvardeclstmt.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/stmt/astvardeclstmt.h>
#include <iostream>

ASTVarDeclStmt::ASTVarDeclStmt(std::shared_ptr<ASTTypeNode> type, std::string id, std::shared_ptr<ASTExpNode> exp)
    : id(id),
      exp(exp),
      type(type)
{
}

ASTVarDeclStmt::~ASTVarDeclStmt() {
}

std::string ASTVarDeclStmt::getId() {
    return id;
}

void ASTVarDeclStmt::setId(std::string id) {
    this->id = id;
}

std::shared_ptr<ASTExpNode> ASTVarDeclStmt::getExp() {
    return exp;
}

std::shared_ptr<ASTTypeNode> ASTVarDeclStmt::getType() {
    return type;
}

void ASTVarDeclStmt::setType(std::shared_ptr<ASTTypeNode> type) {
    this->type = type;
}

void ASTVarDeclStmt::print(std::ostream & ss) {
    ss << id;
}
