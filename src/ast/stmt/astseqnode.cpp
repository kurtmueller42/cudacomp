/**
 * @file astseqnode.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <ast/astseqnode.h>

ASTSeqNode::ASTSeqNode(ASTStmtNode *head, ASTSeqNode *tail)
    : head(head),
      tail(tail)
{
}

ASTSeqNode::~ASTSeqNode() {
    delete head;

    if (tail)
        delete tail;
}

ASTStmtNode *ASTSeqNode::getHead() {
    return head;
}

ASTSeqNode *ASTSeqNode::getTail() {
    return tail;
}

void ASTSeqNode::print(std::ostream & ss) {
    ss << "seq(";
    head->print(ss);
    ss << "," << std::endl;

    if (tail)
        tail->print(ss);
    else
        ss << "null";

    ss << ")";
}
