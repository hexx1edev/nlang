#ifndef NLANG_IRGENERATOR_HPP
#define NLANG_IRGENERATOR_HPP

#include <lang/ast.hpp>
#include <llvm/IR/LLVMContext.h>
#include <llvm/IR/Module.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Type.h>
#include <string_view>
#include <lang/types.hpp>
#include <unordered_map>

class IRGenerator {
public:
    IRGenerator(AST::Program* program);
    ~IRGenerator();

    llvm::Module& generate();

private:
    AST::Program* program;
    llvm::LLVMContext ctx;
    llvm::Module module;
    llvm::IRBuilder<> builder;
    llvm::Function* current;

    std::unordered_map<std::string_view, llvm::Function*> functions;

    void declare_function(AST::Function* func);
    void gen_function(AST::Function* func);
    void gen_block(std::vector<AST::Node*>& block);
    void gen_statement(AST::Node* node);
    void gen_return(AST::Return* node);
    void gen_expression(AST::Node* node);

    bool is_signed(types::Type type);
    llvm::Type* llvm_type(AST::Type* type);
};

#endif
