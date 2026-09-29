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

    Tokenizer tokenizer(buf.str());

    auto tokens = tokenizer.tokenize();

    std::cout << "[";

    for (auto token : tokens) {
        std::cout << token << ",";
    }

    std::cout << "]" << std::endl;

    return 0;
}
