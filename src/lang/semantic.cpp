#include "semantic.hpp"
#include <lang/ast.hpp>
#include <format>

bool is_builtin(Type type) {
    return BUILTIN.contains(type.name);
}

bool is_builtin(std::string& name) {
    return BUILTIN.contains(name);
}

constexpr bool is_void(Type type) {
    return type == VOID;
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

        Type return_type = resolve_type(func->return_type);

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
    Type expected = current->return_type;
    if (node->value == nullptr) {
        if (!is_void(expected))
            error(std::format("expected to return `{}`, got void", expected.name), node->span);
        return;
    }

    Type actual = check_expression(node->value);
    if (is_void(expected)) {
        error(std::format("expected to return void, got `{}`", actual.name), node->value->span);
        return;
    }

    match(node->value, expected, actual);
}

Type Analyzer::check_expression(AST::Node* node) {
    if (node->kind != AST::NodeKind::NumberLiteral) {
        error("fuck you", node->span);
        return ERROR;
    }

    return I32;
}

Type Analyzer::resolve_type(AST::Type* type) {
    std::string name = type->type;
    if (is_builtin(name)) {
        return BUILTIN.at(name);
    }
    error(std::format("unknown type `{}`", name), type->span);
    return ERROR;
}

void Analyzer::match(AST::Node* node, Type expected, Type actual) {
    if (expected == ERROR || actual == ERROR)
        return;
    if (expected != actual) {
        error(std::format("expected `{}`, got `{}`", expected.name, actual.name), node->span);
    }
}
