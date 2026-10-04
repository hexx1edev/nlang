#include "backend/irgenerator.hpp"
#include "lang/ast.hpp"
#include "lang/parser.hpp"
#include "lang/semantic.hpp"
#include "util/diagnostic.hpp"
#include <backend/compiler.hpp>
#include "llvm/IR/Module.h"
#include <llvm/Support/raw_ostream.h>
#include <sstream>
#include <util/error.hpp>
#include <lang/tokenizer.hpp>
#include <lang/semantic.hpp>
#include <fstream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        error() << "usage: " << argv[0] << " <input file> <output file>";
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

    Analyzer anal(prog);

    auto errors = anal.analyze();

    if (!errors.empty()) {
        for (auto e : errors) {
            error() << render(source, e.span, std::string(e.message), argv[1]);
        }
        return 1;
    }

    IRGenerator gen(prog);

    llvm::Module& mod = gen.generate();

    if (!compile(mod, argv[2])) return 1;

    return 0;
}
