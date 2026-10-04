#ifndef NLANG_PARSER_HPP
#define NLANG_PARSER_HPP

#include <exception>
#include <lang/tokenizer.hpp>
#include <lang/span.hpp>
#include <lang/ast.hpp>
#include <string_view>

class ParserError : public std::exception {
public:
    explicit ParserError(std::string message, const Token& token, Span span);
    ~ParserError() override;

    Span span;

    const char* what() const noexcept override;

private:
    std::string message;
    const Token& token;
    std::string formatted;
};

class Parser {
public:
    Parser(const std::vector<Token>& tokens, std::string name);
    ~Parser();

    AST::Program* parse();

private:
    size_t pos;
    const std::vector<Token>& tokens;
    std::string name;

    const Token& peek(bool ahead = false);
    const Token& next();
    const Token& prev();
    const Token& advance();
    const Token& expect(TokenKind kind, std::string_view value = "", std::string_view message = "", bool advance = true);

    AST::Program* parse_program();
    AST::Function* parse_function();
    AST::Type* parse_type();
    std::vector<AST::Node*> parse_block();
    AST::Node* parse_statement();
    AST::Return* parse_return();
    AST::Node* parse_expression();

    Span eof_span();
    Span prev_span();
};

#endif
