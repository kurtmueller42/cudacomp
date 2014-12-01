/**
 * @file typeresolve.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <statics/typeresolve.h>
#include <ast/expr/astintegerexp.h>
#include <ast/expr/astbinopexp.h>
#include <ast/astseqnode.h>
#include <ast/stmt/astreturnstmt.h>
#include <ast/expr/astidentifierexp.h>
#include <ast/stmt/astvardeclstmt.h>
#include <ast/stmt/astassignstmt.h>
#include <ast/type/astintegertype.h>
#include <ast/expr/astunopexp.h>
#include <ast/type/astbooleantype.h>
#include <ast/stmt/astscopestmt.h>
#include <ast/stmt/astifstmt.h>
#include <ast/expr/astbooleanexp.h>
#include <ast/decl/astfundecl.h>
#include <ast/expr/astcallexp.h>
#include <ast/type/astvoidtype.h>
#include <ast/type/astptrtype.h>
#include <ast/stmt/astexprstmt.h>
#include <ast/decl/asttypedecl.h>
#include <ast/type/astidtype.h>

namespace Statics {

TypeResolve::TypeResolve(std::shared_ptr<ModuleInfo> module)
    : module(module)
{
}

std::shared_ptr<ASTTypeNode> TypeResolve::resolveType(std::shared_ptr<ASTTypeNode> type) {
    // Type definitions can only introduce one level of indirection. So, a newly defined type is
    // either a basic type like int or bool, or a single indirection into the existing types.

    // Resolve the 'to' type for pointers.
    if (std::shared_ptr<ASTPtrType> ptr_type = std::dynamic_pointer_cast<ASTPtrType>(type)) {
        ptr_type->setToType(resolveType(ptr_type->getToType()));
        return ptr_type;
    }
    // Resolve the element type for arrays. TODO: test this
    else if (std::shared_ptr<ASTArrType> arr_type = std::dynamic_pointer_cast<ASTArrType>(type)) {
        arr_type->setElemType(resolveType(arr_type->getElemType()));
        return arr_type;
    }
    // ID type. Look up the type, which should already be resolved.
    else if (std::shared_ptr<ASTIdType> id_type = std::dynamic_pointer_cast<ASTIdType>(type)) {
        std::shared_ptr<ASTTypeNode> resolved = module->getType(id_type->getId());

        // The type must be declared already.
        if (!resolved)
            throw UndeclaredTypeException(id_type->getId());

        return resolved;
    }
    // Record type. Look up the record information, which should already be resolved.
    else if (std::shared_ptr<ASTRecordType> record_type = std::dynamic_pointer_cast<ASTRecordType>(type)) {
        std::shared_ptr<ASTRecordType> resolved = module->getRecord(record_type->getId());

        // TODO: do I want a separate exception for this?
        if (!resolved)
            throw UndeclaredTypeException(record_type->getId());

        record_type->setFields(resolved->getFields());

        return resolved;
    }
    // Otherwise it's a basic type already
    else
        return type;
}

void TypeResolve::visitRecordType(std::shared_ptr<ASTRecordType> type) {
    // We already resolved the record and just have to retrieve the result from a table
    // No need to visit children of record, which are already resolved
    resolveType(type);
}

void TypeResolve::visitTypeNode(std::shared_ptr<ASTTypeNode> type) {
    // Just resolve the type in place. We don't need to worry about the return value, because we
    // aren't resolving recursively. That's done by resolveType() itself.
    resolveType(type);
    ASTVisitor::visitTypeNode(type);
}

void TypeResolve::visitArgNode(std::shared_ptr<ASTArgNode> argNode) {
    argNode->setType(resolveType(argNode->getType()));
    ASTVisitor::visitArgNode(argNode);
}

void TypeResolve::visitFunType(std::shared_ptr<ASTFunType> funType) {
    funType->setReturnType(resolveType(funType->getReturnType()));
    // Arguments are handled in visitArgNode()
    ASTVisitor::visitFunType(funType);
}

void TypeResolve::visitVarDeclStmt(std::shared_ptr<ASTVarDeclStmt> varDecl) {
    varDecl->setType(resolveType(varDecl->getType()));
    ASTVisitor::visitVarDeclStmt(varDecl);
}

void TypeResolve::visitAllocArrayExp(std::shared_ptr<ASTAllocArrayExp> allocExp) {
    allocExp->setElemType(resolveType(allocExp->getElemType()));
    ASTVisitor::visitAllocArrayExp(allocExp);
}

void TypeResolve::visitAllocExp(std::shared_ptr<ASTAllocExp> allocExp) {
    allocExp->setElemType(resolveType(allocExp->getElemType()));
    ASTVisitor::visitAllocExp(allocExp);
}

void TypeResolve::visitTypeDecl(std::shared_ptr<ASTTypeDecl> typeDecl) {
    // TypeDecl's define new type names, so we need to update the mapping in the module

    // Types must have a new name
    if (module->getType(typeDecl->getName()) != nullptr)
        throw RedeclaredTypeException(typeDecl->getName());

    // TODO: type cannot be defined in terms of itself
    // TODO: inconsistency between getName() and getId() all over the place

    // Add the new type to the mapping
    module->addType(typeDecl->getName(), resolveType(typeDecl->getType()));

    ASTVisitor::visitTypeDecl(typeDecl);
}

void TypeResolve::visitRecordDecl(std::shared_ptr<ASTRecordDecl> recordDecl) {

    // Nothing to do on a declaration without a definition
    if (!(recordDecl->isDefn()))
        return;

    // Records cannot be redefined
    if (module->getRecord(recordDecl->getName()) != nullptr)
        throw RedeclaredTypeException(recordDecl->getName());

    // TODO : FIX broken handling of recursive structs
    // TODO : ensure no repeat field names, void fields

    std::string name = recordDecl->getName();
    module->addRecord(name, recordDecl->getSignature());
    std::shared_ptr<ASTArgSeqNode> fields = recordDecl->getSignature()->getFields();

    ASTVisitor::visitArgSeqNode(fields);
    //manually weed out recursive structs
    while(fields) {
        std::shared_ptr<ASTArgNode> field = fields->getHead();
        if (std::shared_ptr<ASTRecordType> type = std::dynamic_pointer_cast<ASTRecordType>(field->getType())) {
            if ((name.compare(type->getId())) == 0)
                throw IllegalTypeException(); // TODO : better exception?
        }
//        else if (std::shared_ptr<ASTPtrType> type = std::dynamic_pointer_cast<ASTPtrType>(field->getType())) {
//            // Pointers to this record don't need to be resolved
//            if (std::shared_ptr<ASTRecordType> rcd_type = std::dynamic_pointer_cast<ASTRecordType>(type->getToType())) {
//                if ((name.compare(rcd_type->getId())) == 0){
//                    // Nothing to do
//                }
//                else {
//                    visitArgNode(field);
//                }
//            }
//        }
        else{
            visitNode(field);
        }
        fields = fields->getTail();
    }
}

void TypeResolve::visitRangeForStmt(std::shared_ptr<ASTRangeForStmt> forStmt) {
    forStmt->setIteratorType(resolveType(forStmt->getIteratorType()));
    ASTVisitor::visitRangeForStmt(forStmt);
}

void TypeResolve::run(std::shared_ptr<ASTDeclSeqNode> ast) {
    visitDeclSeqNode(ast);
}

};
