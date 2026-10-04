#ifndef NLANG_TOKENIZER_HPP
#define NLANG_TOKENIZER_HPP

#include <exception>
#include <string>
#include <vector>
#include <lang/span.hpp>

enum class TokenKind {
    Keyword,
    Identifier,
    LParen,
    RParen,
    Operator,
    LBracket,
    RBracket,
    Number,
    Semicolon
};

std::string token_name(TokenKind kind);

class Token {
public:
    Token(TokenKind kind, std::string value, Span span);
    ~Token();

    TokenKind kind;
    std::string value;

    Span span;

    std::string repr() const;
};

std::ostream& operator<<(std::ostream& os, const Token& token);

class TokenizerError : public std::exception {
public:
    explicit TokenizerError(std::string message, Span span);
    ~TokenizerError() noexcept override;

    Span span;

    const char* what() const noexcept override;
private:
    std::string message;
};

class Tokenizer {
public:
    Tokenizer(std::string source);
    ~Tokenizer();

    std::string source;

    std::vector<Token> tokenize();

private:
    int pos;

    char peek(bool ahead = false);
    char next();
    char advance();

    Token read_identifier();
    Token read_number();
    Token read_operator();
    void skip_inline_comment();
};

#endif
