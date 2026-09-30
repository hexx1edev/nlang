#ifndef NSCRIPT_AST_HPP
#define NSCRIPT_AST_HPP

#include <lang/span.hpp>
#include <vector>

namespace AST {

enum class ASTNodeKind {
    Program,
    Type,
    Function,
    Return,
    NumberLiteral,
    Identifier
};

struct ASTNode {
    Span span;
    ASTNodeKind kind;

    ASTNode(Span span, ASTNodeKind kind)
        : span(span), kind(kind) {}

    virtual ~ASTNode() = default;
    virtual std::string repr() const = 0;
};

struct Type : public ASTNode {
    std::string type;

    Type(Span span, std::string type)
        : ASTNode(span, ASTNodeKind::Type),
            type(std::move(type)) {}

    std::string repr() const override;
};

struct Function : public ASTNode {
    std::string name;
    Type* return_type;
    std::vector<ASTNode*> body;

    Function(Span span, std::string name, Type* return_type, std::vector<ASTNode*> body)
        : ASTNode(span, ASTNodeKind::Function), name(std::move(name)), return_type(return_type),
            body(std::move(body)) {}

    std::string repr() const override;
};

struct Program : public ASTNode {
    std::string name;
    std::vector<Function*> funcs;

    Program(Span span, std::string name, std::vector<Function*> funcs)
        : ASTNode(span, ASTNodeKind::Program), name(std::move(name)), funcs(std::move(funcs)) {}

    std::string repr() const override;
};

struct NumberLiteral : public ASTNode {
    int value;

    NumberLiteral(Span span, int value)
        : ASTNode(span, ASTNodeKind::NumberLiteral), value(value) {}

    std::string repr() const override;
};

struct Identifier : public ASTNode {
    std::string name;

    Identifier(Span span, std::string name)
        : ASTNode(span, ASTNodeKind::Identifier), name(std::move(name)) {}

    std::string repr() const override;
};

struct Return : public ASTNode {
    ASTNode* value;

    Return(Span span, ASTNode* value)
        : ASTNode(span, ASTNodeKind::Return), value(value) {}

    std::string repr() const override;
};

}

#endif
