/**
 * @file astbinop.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/expr/astbinop.h>

ASTBinop::ASTBinop(enum op op, ASTExpNode *e1, ASTExpNode *e2)
    : op(op),
      e1(e1),
      e2(e2)
{
}

ASTBinop::~ASTBinop() {
    delete e1;
    delete e2;
}

enum ASTBinop::op ASTBinop::getOp() {
    return op;
}

ASTExpNode *ASTBinop::getE1() {
    return e1;
}

ASTExpNode *ASTBinop::getE2() {
    return e2;
}

void ASTBinop::print(std::ostream & ss) {
    ss << "bin(";
    e1->print(ss);
    ss << ", ";

    switch(op) {
    case ADD:
        ss << "+";
        break;
    case SUB:
        ss << "-";
        break;
    case MUL:
        ss << "*";
        break;
    case DIV:
        ss << "/";
        break;
    case MOD:
        ss << "%";
        break;
    case SHL:
        ss << "<<";
        break;
    case SHR:
        ss << ">>";
        break;
    case AND:
        ss << "&&";
        break;
    case OR:
        ss << "||";
        break;
    case BAND:
        ss << "&";
        break;
    case BOR:
        ss << "|";
        break;
    case BXOR:
        ss << "^";
        break;
    case LT:
        ss << "<";
        break;
    case GT:
        ss << ">";
        break;
    case LEQ:
        ss << "<=";
        break;
    case GEQ:
        ss << ">=";
        break;
    case EQ:
        ss << "==";
        break;
    case NEQ:
        ss << "!=";
        break;
    }

    ss << ", ";
    e2->print(ss);
    ss << ")";
}