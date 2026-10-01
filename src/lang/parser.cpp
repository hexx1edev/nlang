#include "parser.hpp"
#include "lang/ast.hpp"
#include "lang/tokenizer.hpp"
#include <cstddef>
#include <format>

ParserError::ParserError(std::string message, const Token& token, Span span) : message(message), token(token),
    formatted(std::format("`{}`: {}", token.value, message)), span(span) {}
ParserError::~ParserError() {}

const char* ParserError::what() const noexcept {
    return formatted.c_str();
}

Parser::Parser(const std::vector<Token>& tokens, std::string name) : tokens(tokens), name(name), pos(0) {}
Parser::~Parser() {}

const Token& Parser::peek(bool ahead) {
    size_t _pos = pos + (ahead ? 1 : 0);

    if (_pos >= tokens.size())
        throw ParserError("unexpected EOF", tokens[pos - 1], tokens[pos - 1].span);

    return tokens[_pos];
}

const Token& Parser::next() {
    pos++;
    return peek();
}

const Token& Parser::prev() {
    pos--;
    return peek();
}

const Token& Parser::advance() {
    const Token& token = peek();
    pos++;
    return token;
}

const Token& Parser::expect(TokenKind kind, std::string_view value, std::string_view message, bool advance) {
    bool matches = true;
    const Token& token = peek();

    if (token.kind != kind)
        matches = false;
    if (!value.empty() && value != token.value)
        matches = false;

    if (!matches) {
        std::string expected;
        if (!value.empty())
            expected = std::format("`{}`", value);
        else
            expected = token_name(kind);

        std::string fallback = std::format("expected {}, got `{}`", expected, token.value);

        if (message.empty()) {
            throw ParserError(fallback, token, token.span);
        } else {
            throw ParserError(std::string(message), token, token.span);
        }
    }

    if (advance)
        this->advance();

    return token;
}

Span Parser::eof_span() {
    if (tokens.empty()) {
        return Span(0, 0);
    }

    size_t end = tokens.back().span.end;

    return Span(end, end);
}

Span Parser::prev_span() {
    return tokens[pos - 1].span;
}

AST::Program* Parser::parse() {
    return parse_program();
}

AST::Program* Parser::parse_program() {
    AST::Program* prog = new AST::Program(Span(0, eof_span().end), name, {});

    while (pos < tokens.size()) {
        prog->funcs.push_back(parse_function());
    }

    return prog;
}

AST::Function* Parser::parse_function() {
    AST::Function* func = new AST::Function(Span(0, 0), "", nullptr, {});

    Span start = expect(TokenKind::Keyword, "fn").span;

    func->name = expect(TokenKind::Identifier, "", "expected function name").value;

    expect(TokenKind::LParen);
    expect(TokenKind::RParen);

    const Token& token = peek();

    switch (token.kind) {
        case TokenKind::Operator:
            expect(TokenKind::Operator, "->");
            func->return_type = parse_type();
            break;
        case TokenKind::LBracket:
            break;
        default:
            throw ParserError("invalid syntax", token, token.span);
    }

    func->body = parse_block();
    func->span = start.to(prev_span());

    if (func->return_type == nullptr)
        func->return_type = new AST::Type(Span(0, 0), "void");

    return func;
}

AST::Type* Parser::parse_type() {
    const Token& type = expect(TokenKind::Identifier, "", "expected type");

    return new AST::Type(type.span, type.value);
}

std::vector<AST::Node*> Parser::parse_block() {
    std::vector<AST::Node*> block;

    expect(TokenKind::LBracket, "", "expected block");

    while (true) {
        const Token& token = peek();

        if (token.kind == TokenKind::RBracket) {
            advance();
            break;
        }

        block.push_back(parse_statement());
    }

    return block;
}

AST::Node* Parser::parse_statement() {
    const Token& token = peek();

    switch (token.kind) {
        case TokenKind::Keyword:
            if (token.value == "return")
                return parse_return();
            break;
        default:
            break;
    }

    throw ParserError("expected statement", token, token.span);
}

AST::Return* Parser::parse_return() {
    Span start = expect(TokenKind::Keyword, "return").span;

    AST::Node* value = parse_expression();

    expect(TokenKind::Semicolon);

    return new AST::Return(start.to(value->span), value);
}

AST::Node* Parser::parse_expression() {
    const Token& value = expect(TokenKind::Number);
    return new AST::NumberLiteral(value.span, std::stoi(value.value));
}
