#include "semantic.hpp"
#include <lang/ast.hpp>
#include <format>

bool is_builtin(types::Type type) {
    return types::BUILTIN.contains(type.name);
}

bool is_builtin(std::string& name) {
    return types::BUILTIN.contains(name);
}

constexpr bool is_void(types::Type type) {
    return type.kind == types::TypeKind::VOID;
}

Analyzer::Analyzer(AST::Program* program) : program(program), current(nullptr) {}
Analyzer::~Analyzer() {}

void Analyzer::error(std::string message, Span span) {
    errors.push_back(SemanticError {message, span});
}

std::vector<SemanticError>& Analyzer::analyze() {
    check_program(program);
    return errors;
}

void Analyzer::check_program(AST::Program* program) {
    declare_functions(program);

    for (auto func : program->funcs) {
        check_function(func);
    }
}

void Analyzer::declare_functions(AST::Program* program) {
    for (auto func : program->funcs) {
        if (functions.contains(func->name)) {
            error(std::format("function `{}` is already declared", func->name), func->span);
            continue;
        }

        types::Type return_type = resolve_type(func->return_type);

        Signature sig;
        sig.name = func->name;
        sig.return_type = return_type;
        functions[func->name] = sig;
    }
}

void Analyzer::check_function(AST::Function* func) {
    Signature sig = functions[func->name];
    current = &sig;

    check_block(func->body, false);

    current = nullptr;
}

void Analyzer::check_block(std::vector<AST::Node*>& block, bool newScope) {
    for (auto stmt : block) {
        check_statement(stmt);
    }
}

void Analyzer::check_statement(AST::Node* node) {
    switch (node->kind) {
        case AST::NodeKind::Return:
            check_return((AST::Return*) node);
            break;
        default:
            error("invalid/unsupported statement", node->span);
    }
}

void Analyzer::check_return(AST::Return* node) {
    types::Type expected = current->return_type;
    if (node->value == nullptr) {
        if (!is_void(expected))
            error(std::format("expected to return `{}`, got void", expected.name), node->span);
        return;
    }

    types::Type actual = check_expression(node->value);
    if (is_void(expected)) {
        error(std::format("expected to return void, got `{}`", actual.name), node->value->span);
        return;
    }

    match(node->value, expected, actual);
}

types::Type Analyzer::check_expression(AST::Node* node) {
    if (node->kind != AST::NodeKind::NumberLiteral) {
        error("fuck you", node->span);
        return types::ERROR;
    }

    return types::I32;
}

types::Type Analyzer::resolve_type(AST::Type* node) {
    std::string name = node->name;
    if (is_builtin(name)) {
        types::Type type = types::BUILTIN.at(name);
        node->type = type;
        return type;
    }
    error(std::format("unknown type `{}`", name), node->span);
    return types::ERROR;
}

void Analyzer::match(AST::Node* node, types::Type expected, types::Type actual) {
    if (expected == types::ERROR || actual == types::ERROR)
        return;
    if (expected != actual) {
        error(std::format("expected `{}`, got `{}`", expected.name, actual.name), node->span);
    }
}
