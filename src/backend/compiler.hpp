#ifndef NLANG_COMPILER_HPP
#define NLANG_COMPILER_HPP

#include <llvm/IR/Module.h>

bool compile(llvm::Module& module, std::string out);

#endif
