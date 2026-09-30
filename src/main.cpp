#include "lang/ast.hpp"
#include "lang/parser.hpp"
#include "util/diagnostic.hpp"
#include <sstream>
#include <util/error.hpp>
#include <lang/tokenizer.hpp>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        error() << "no file specified";
        return 1;
    }

    std::ifstream f(argv[1]);
    if (!f.is_open()) {
        error() << "failed to open " << argv[1];
        return 1;
    }

    std::stringstream buf;
    buf << f.rdbuf();

    if (f.bad()) {
        error() << "failed to read " << argv[1];
        return 1;
    }

    std::string source = buf.str();

    Tokenizer tokenizer(source);

    std::vector<Token> tokens;

    try {
        tokens = tokenizer.tokenize();
    } catch (const TokenizerError& e) {
        error() << render(source, e.span, e.what(), argv[1]);
        return 1;
    }

    Parser parser(tokens, std::string(argv[1]));

    AST::Program* prog;

    try {
        prog = parser.parse();
    } catch (const ParserError& e) {
        error() << render(source, e.span, e.what(), argv[1]);
        return 1;
    }

    std::cout << prog->repr();

    return 0;
}
