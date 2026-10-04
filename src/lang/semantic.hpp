#ifndef NLANG_SEMANTIC_HPP
#define NLANG_SEMANTIC_HPP

#include <lang/ast.hpp>
#include <unordered_map>

struct SemanticError {
    std::string message;
    Span span;
};

struct Signature {
    std::string_view name;
    types::Type return_type;
};

class Analyzer {
public:
    Analyzer(AST::Program* program);
    ~Analyzer();

    std::vector<SemanticError>& analyze();

private:
    std::vector<SemanticError> errors;
    std::unordered_map<std::string_view, Signature> functions;
    Signature* current;

    AST::Program* program;

    void error(std::string message, Span span);

    void declare_functions(AST::Program* program);

    void check_program(AST::Program* program);
    void check_function(AST::Function* func);
    void check_block(std::vector<AST::Node*>& block, bool newScope = true);
    void check_statement(AST::Node* node);
    void check_return(AST::Return* node);
    types::Type check_expression(AST::Node* node);

    types::Type resolve_type(AST::Type* node);
    void match(AST::Node* node, types::Type expected, types::Type actual);
};

#endif
