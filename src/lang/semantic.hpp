#ifndef NSCRIPT_SEMANTIC_HPP
#define NSCRIPT_SEMANTIC_HPP

#include <lang/ast.hpp>
#include <unordered_map>
#include <array>

struct Type {
    std::string_view name;

    constexpr bool operator==(const Type&) const = default;
};

constexpr Type U8{"u8"};
constexpr Type U16{"u16"};
constexpr Type U32{"u32"};
constexpr Type U64{"u64"};

constexpr Type I8{"i8"};
constexpr Type I16{"i16"};
constexpr Type I32{"i32"};
constexpr Type I64{"i64"};

constexpr Type VOID{"void"};
constexpr Type ERROR{"<error>"};

constexpr auto NUMERIC = std::array{
    U8, U16, U32, U64, I8, I16, I32, I64
};

const std::unordered_map<std::string_view, Type> BUILTIN = {
    {"u8", U8},
    {"u16", U16},
    {"u32", U32},
    {"u64", U64},
    {"i8", I8},
    {"i16", I16},
    {"i32", I32},
    {"i64", I64},
    {"void", VOID},
};

struct SemanticError {
    std::string message;
    Span span;
};

struct Signature {
    std::string_view name;
    Type return_type;
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
    Type check_expression(AST::Node* node);

    Type resolve_type(AST::Type* type);
    void match(AST::Node* node, Type expected, Type actual);
};

#endif
