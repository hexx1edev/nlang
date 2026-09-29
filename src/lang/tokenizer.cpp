#include "tokenizer.hpp"
#include <cctype>
#include <format>
#include <ostream>
#include <vector>
#include <array>

using namespace std::literals;

constexpr auto KEYWORDS = std::array{
    "fn"sv,
    "return"sv,
    "let"sv,
    "const"sv,
    "if"sv,
    "else"sv,
    "while"sv,
    "continue"sv,
    "break"sv
};

constexpr auto BOOLEAN = std::array{
    "true"sv,
    "false"sv
};

constexpr auto BASE = std::array{
    '+',
    '-',
    '*',
    '/',
    '=',
    '<',
    '>',
    '!',
    '%',
    '|',
    '&',
    '^',
    '~'
};

constexpr auto SPECIAL = std::array{
    "->"sv,
    "="sv
};

constexpr auto GENERAL_OPERATORS = std::array{
    "+"sv,
    "-"sv,
    "*"sv,
    "/"sv,
    "%"sv,
    "|"sv,
    "&"sv,
    "^"sv
};

constexpr auto UNARY_OPERATORS = std::array{
    "~"sv,
    "!"sv
};

constexpr auto ASSIGN_OPERATORS = std::array{
    "+="sv,
    "-="sv,
    "*="sv,
    "/="sv,
    "%="sv,
    "|="sv,
    "&="sv,
    "^="sv,
    "~="sv
};

constexpr auto CONDITION_OPERATORS = std::array{
    "<"sv,
    ">"sv,
    "<="sv,
    ">="sv,
    "!="sv,
    "=="sv,
    "&&"sv,
    "||"sv,
    "^^"sv
};

constexpr bool is_keyword(std::string_view value) {
    return std::ranges::find(KEYWORDS, value) != KEYWORDS.end();
}

constexpr bool is_operator(std::string_view value) {
    return std::ranges::find(SPECIAL, value) != SPECIAL.end()
            || std::ranges::find(GENERAL_OPERATORS, value) != GENERAL_OPERATORS.end()
            || std::ranges::find(UNARY_OPERATORS, value) != UNARY_OPERATORS.end()
            || std::ranges::find(ASSIGN_OPERATORS, value) != ASSIGN_OPERATORS.end()
            || std::ranges::find(CONDITION_OPERATORS, value) != CONDITION_OPERATORS.end();
}

constexpr bool is_operator_base(char value) {
    return std::ranges::find(BASE, value) != BASE.end();
}

bool is_empty(char c) {
    return c == ' ' || c == '\r' || c == '\n' || c == '\t';
}

Token::Token(TokenKind kind, std::string value, Span span) : kind(kind), value(value), span(span) {}
Token::~Token() {}

std::string Token::repr() const {
    switch (kind) {
        case TokenKind::Identifier:
            return std::format("Identifier({})", value);
        case TokenKind::Keyword:
            return std::format("Keyword({})", value);
        case TokenKind::LParen:
            return "LParen";
        case TokenKind::RParen:
            return "RParen";
        case TokenKind::Operator:
            return std::format("Operator({})", value);
        case TokenKind::LBracket:
            return "LBracket";
        case TokenKind::RBracket:
            return "RBracket";
        case TokenKind::Number:
            return std::format("Number({})", value);
        case TokenKind::Semicolon:
            return "Semicolon";
        default:
            return "Unknown";
    }
}

std::string Token::name() const {
    switch (kind) {
        case TokenKind::Identifier:
            return "identifer";
        case TokenKind::Keyword:
            return "keyword";
        case TokenKind::LParen:
            return "left parenthesis";
        case TokenKind::RParen:
            return "right parenthesis";
        case TokenKind::Operator:
            return "operator";
        case TokenKind::LBracket:
            return "left bracket";
        case TokenKind::RBracket:
            return "right bracket";
        case TokenKind::Number:
            return "number";
        case TokenKind::Semicolon:
            return "semicolon";
        default:
            return "unknown";
    }
}

std::ostream& operator<<(std::ostream& os, const Token& token) {
    os << token.repr();
    return os;
}

TokenizerError::TokenizerError(std::string message, Span span) : message(std::move(message)), span(span) {}
TokenizerError::~TokenizerError() noexcept {}

const char* TokenizerError::what() const noexcept {
    return message.c_str();
}

Tokenizer::Tokenizer(std::string source) : source(source), pos(0) {}
Tokenizer::~Tokenizer() {}

char Tokenizer::peek(bool ahead) {
    int _pos = pos + (ahead ? 1 : 0);

    if (_pos >= source.size())
        return '\0';

    return source[_pos];
}

char Tokenizer::next() {
    pos++;
    return peek();
}

char Tokenizer::advance() {
    char current = peek();
    pos++;
    return current;
}

std::vector<Token> Tokenizer::tokenize() {
    std::vector<Token> tokens;

    while (pos < source.size()) {
        char current = peek();

        if (std::isalpha(current)) {
            tokens.push_back(read_identifier());
            continue;
        } else if (std::isdigit(current)) {
            tokens.push_back(read_number());
            continue;
        } else if (is_empty(current)) {
            next();
            continue;
        }

        switch (current) {
            case '(':
                tokens.push_back(Token(
                    TokenKind::LParen,
                    "(",
                    Span(pos, pos + 1)
                )); next(); break;
            case ')':
                tokens.push_back(Token(
                    TokenKind::RParen,
                    ")",
                    Span(pos, pos + 1)
                )); next(); break;
            case '{':
                tokens.push_back(Token(
                    TokenKind::LBracket,
                    "{",
                    Span(pos, pos + 1)
                )); next(); break;
            case '}':
                tokens.push_back(Token(
                    TokenKind::RBracket,
                    "}",
                    Span(pos, pos + 1)
                )); next(); break;
            case ';':
                tokens.push_back(Token(
                    TokenKind::Semicolon,
                    ";",
                    Span(pos, pos + 1)
                )); next(); break;
            default:
                {
                    Token op = read_operator();
                    if (op.value.size() != 0) tokens.push_back(op);
                }
        }
    }

    return tokens;
}

Token Tokenizer::read_identifier() {
    int start = pos;
    std::string ident = "";

    while (std::isalnum(peek()) || peek() == '_') {
        ident += advance();
    }

    TokenKind kind = is_keyword(ident) ? TokenKind::Keyword : TokenKind::Identifier;

    return Token(kind, ident, Span(start, pos));
}

Token Tokenizer::read_number() {
    int start = pos;
    std::string num = "";

    while (std::isdigit(peek())) {
        num += advance();
    }

    if (std::isalpha(peek()))
        throw TokenizerError(std::format("invalid digit: {}", peek()), Span(pos, pos + 1));

    return Token(TokenKind::Number, num, Span(start, pos));
}

Token Tokenizer::read_operator() {
    int start = pos;
    std::string op = "";

    if (!is_operator_base(peek()))
        throw TokenizerError(std::format("unknown symbol: {}", peek()), Span(pos, pos + 1));

    while (is_operator_base(peek())) {
        op += advance();
    }

    if (op == "//") {
        skip_inline_comment();
        return Token(TokenKind::Operator, "", Span(0, 0));
    }

    if (!is_operator(op)) {
        throw TokenizerError(std::format("invalid operator: {}", op), Span(start, pos));
    }

    return Token(TokenKind::Operator, op, Span(start, pos));
}

void Tokenizer::skip_inline_comment() {
    while (pos < source.size() && peek() != '\n' && peek() != '\r') {
        advance();
    }
}
