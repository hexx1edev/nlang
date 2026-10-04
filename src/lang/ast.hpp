#ifndef NLANG_AST_HPP
#define NLANG_AST_HPP

#include <lang/span.hpp>
#include <lang/types.hpp>
#include <vector>

namespace AST {

enum class NodeKind {
    Program,
    Type,
    Function,
    Return,
    NumberLiteral,
    Identifier
};

struct Node {
    Span span;
    NodeKind kind;

    Node(Span span, NodeKind kind)
        : span(span), kind(kind) {}

    virtual ~Node() = default;
    virtual std::string repr() const = 0;
};

struct Type : public Node {
    std::string name;
    types::Type type;

    Type(Span span, std::string type)
        : Node(span, NodeKind::Type),
            name(std::move(type)) {}

    std::string repr() const override;
};

struct Function : public Node {
    std::string name;
    Type* return_type;
    std::vector<Node*> body;

    Function(Span span, std::string name, Type* return_type, std::vector<Node*> body)
        : Node(span, NodeKind::Function), name(std::move(name)), return_type(return_type),
            body(std::move(body)) {}

    std::string repr() const override;
};

struct Program : public Node {
    std::string name;
    std::vector<Function*> funcs;

    Program(Span span, std::string name, std::vector<Function*> funcs)
        : Node(span, NodeKind::Program), name(std::move(name)), funcs(std::move(funcs)) {}

    std::string repr() const override;
};

struct NumberLiteral : public Node {
    int value;

    NumberLiteral(Span span, int value)
        : Node(span, NodeKind::NumberLiteral), value(value) {}

    std::string repr() const override;
};

struct Identifier : public Node {
    std::string name;

    Identifier(Span span, std::string name)
        : Node(span, NodeKind::Identifier), name(std::move(name)) {}

    std::string repr() const override;
};

struct Return : public Node {
    Node* value;

    Return(Span span, Node* value)
        : Node(span, NodeKind::Return), value(value) {}

    std::string repr() const override;
};

}

#endif
