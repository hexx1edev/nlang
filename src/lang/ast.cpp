#include "ast.hpp"
#include <format>

namespace AST {

std::string Type::repr() const {
    return std::format("Type({})", type);
}

std::string Function::repr() const {
    std::string result = std::format("Function(name={}, return_type={}, body=[\n", name, return_type->repr());

    for (auto* node : body) {
        result += std::format("    {}\n", node->repr());
    }

    result += "])";

    return result;
}

std::string Program::repr() const {
    std::string result = std::format("Program(name={}, funcs=[\n", name);

    for (auto* node : funcs) {
        result += std::format("    {}\n", node->repr());
    }

    result += "])";

    return result;
}

std::string NumberLiteral::repr() const {
    return std::format("NumberLiteral({})", value);
}

std::string Identifier::repr() const {
    return std::format("Identifier({})", name);
}

std::string Return::repr() const {
    return std::format("Return({})", value->repr());
}

}
