#include "compiler.hpp"
#include "llvm/TargetParser/Triple.h"
#include <util/error.hpp>
#include <llvm/MC/TargetRegistry.h>
#include <llvm/Support/TargetSelect.h>
#include <llvm/Support/FileSystem.h>
#include <llvm/Target/TargetMachine.h>
#include <llvm/Target/TargetOptions.h>
#include <llvm/TargetParser/Host.h>
#include <llvm/IR/LegacyPassManager.h>

bool compile(llvm::Module& module, std::string out) {
    llvm::InitializeNativeTarget();
    llvm::InitializeNativeTargetAsmPrinter();
    llvm::InitializeNativeTargetAsmParser();

    llvm::Triple triple = llvm::Triple(llvm::sys::getDefaultTargetTriple());
    std::string err;
    const llvm::Target *target = llvm::TargetRegistry::lookupTarget(triple, err);
    if (!target) { error() << err; return false; }

    llvm::TargetOptions opt;
    llvm::TargetMachine *tm = target->createTargetMachine(
        triple, "generic", "", opt, llvm::Reloc::PIC_);

    module.setTargetTriple(triple);
    module.setDataLayout(tm->createDataLayout());

    std::error_code ec;
    llvm::raw_fd_ostream dest(out, ec, llvm::sys::fs::OF_None);
    if (ec) { error() << ec.message(); return false; }

    llvm::legacy::PassManager pm;
    if (tm->addPassesToEmitFile(pm, dest, nullptr, llvm::CodeGenFileType::ObjectFile)) {
        error() << "target can't emit an object file";
        return false;
    }
    pm.run(module);
    dest.flush();

    return true;
}
