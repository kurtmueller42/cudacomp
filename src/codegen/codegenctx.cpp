/**
 * @file codegenctx.cpp
 *
 * @author Sean James <seanjames777@gmail.com>
 */

#include <codegen/codegenctx.h>
#include <codegen/converttype.h>

namespace Codegen {

CodegenCtx::CodegenCtx(bool emit_device, FunctionInfo *func)
    : context(getGlobalContext()),
      emit_device(emit_device),
      func(func)
{
    module = new Module("", context);

    std::vector<Type *> argTypes;
    FunctionType *ftype;

    if (emit_device) {
        argTypes.push_back(Type::getInt32PtrTy(context));
        ftype = FunctionType::get(Type::getVoidTy(context), argTypes, false);
    }
    else
        ftype = FunctionType::get(Type::getInt32Ty(context), argTypes, false);

    cc_main = Function::Create(ftype, GlobalValue::ExternalLinkage, "_cc_main", module);

    def_bblock = BasicBlock::Create(context, "defs", cc_main, NULL);

    def_builder = new IRBuilder<>(def_bblock);

    first_bblock = createBlock();
    pushBlock(first_bblock);
}

BasicBlock *CodegenCtx::createBlock() {
    return BasicBlock::Create(context, "", cc_main, NULL);
}

void CodegenCtx::pushBlock(BasicBlock *block) {
    blocks.push_back(block);
    this->body_builder = new IRBuilder<>(block);
}

void CodegenCtx::popBlock() {
    blocks.pop_back();
    BasicBlock *block = blocks.back();
    this->body_builder = new IRBuilder<>(block);
}

void CodegenCtx::markKernel(Function *kernel) {
    std::vector<Value *> meta;
    meta.push_back(cc_main);
    meta.push_back(MDString::get(context, "kernel"));
    meta.push_back(ConstantInt::get(Type::getInt32Ty(context), 1));
    MDNode *node = MDNode::get(context, meta);

    NamedMDNode *cat = module->getOrInsertNamedMetadata("nvvm.annotations");
    cat->addOperand(node);
}

void CodegenCtx::emit(std::ostream & out) {
    def_builder->CreateBr(first_bblock);

    if (emit_device) {
        markKernel(cc_main);
        module->setTargetTriple("nvptx64-nvidia-cuda");
    }
    else {
        module->setTargetTriple("x86_64-apple-macosx10.10.0");
    }

    PassManager pm;

    raw_os_ostream outs(out);
    pm.add(createPrintModulePass(outs));

    pm.run(*module);
}

Module *CodegenCtx::getModule() {
    return module;
}

LLVMContext & CodegenCtx::getContext() {
    return context;
}

Function *CodegenCtx::getFunction() {
    return cc_main;
}

bool CodegenCtx::getEmitDevice() {
    return emit_device;
}

IRBuilder<> *CodegenCtx::getBuilder() {
    return body_builder;
}

Value *CodegenCtx::getSymbol(std::string id) {
    if (!symbols.hasSymbol(id)) {
        Value *instr = def_builder->CreateAlloca(convertType(func->getLocalType(id)));
        symbols.set(id, instr);
        return instr;
    }

    return symbols.get(id);
}

}
