#include "irgenerator.hpp"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Value.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/DerivedTypes.h>
#include <lang/types.hpp>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <lang/ast.hpp>

IRGenerator::IRGenerator(AST::Program* program) : program(program), ctx(), module(program->name, ctx), builder(ctx) {}
IRGenerator::~IRGenerator() {}

llvm::Module& IRGenerator::generate() {
    for (auto func : program->funcs) {
        declare_function(func);
    }
    for (auto func : program->funcs) {
        gen_function(func);
    }

    return module;
}

void IRGenerator::gen_function(AST::Function* func) {
    llvm::Function* fn = functions.at(func->name);

    llvm::BasicBlock* entry = llvm::BasicBlock::Create(ctx, "entry", fn);
    builder.SetInsertPoint(entry);

    current = fn;

    gen_block(func->body);

    current = nullptr;
}

void IRGenerator::gen_block(std::vector<AST::Node*>& block) {
    for (auto stmt : block) {
        gen_statement(stmt);
    }
}

void IRGenerator::gen_statement(AST::Node* node) {
    switch (node->kind) {
        case AST::NodeKind::Return:
            gen_return((AST::Return*) node);
            break;
        default:
            return;
    }
}

void IRGenerator::gen_return(AST::Return* node) {
    AST::NumberLiteral* num = (AST::NumberLiteral*) node->value;
    llvm::Value* val = llvm::ConstantInt::get(current->getReturnType(), num->value);
    builder.CreateRet(val);
}

void IRGenerator::declare_function(AST::Function* func) {
    llvm::Type* return_type = llvm_type(func->return_type);

    llvm::FunctionType* fnty = llvm::FunctionType::get(return_type, {}, false);
    llvm::Function* fn = llvm::Function::Create(fnty, llvm::Function::CommonLinkage, func->name, module);

    functions[func->name] = fn;
}

bool IRGenerator::is_signed(types::Type type) {
    return std::ranges::find(types::SIGNED, type) != types::SIGNED.end();
}

llvm::Type* IRGenerator::llvm_type(AST::Type* type) {
    switch (type->type.kind) {
        case types::TypeKind::I8:
        case types::TypeKind::U8:
            return builder.getInt8Ty();
        case types::TypeKind::I16:
        case types::TypeKind::U16:
            return builder.getInt16Ty();
        case types::TypeKind::I32:
        case types::TypeKind::U32:
            return builder.getInt32Ty();
        case types::TypeKind::I64:
        case types::TypeKind::U64:
            return builder.getInt64Ty();
        case types::TypeKind::VOID:
            return builder.getVoidTy();
        default:
            return nullptr;
    }
}
