# nLang

nLang is a simple C-like compiled programming language, that was designed to be a better and mixed version of C, C++ and Rust while keeping simplicity and beginner-friendly syntax.

The language's compiler is currently written in C++, but after first release I'll start working on rewriting it to itself.  
It uses LLVM as a compilation backend.

## Building

Install `cmake, C++ compiler, vcpkg`, run `vcpkg install --clean-buildtrees-after-build` to install LLVM.  
> [!NOTE]
> This will take some time and disk space because LLVM is a big library.

Run `cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE="<your vcpkg root>/scripts/buildsystems/vcpkg.cmake"` to bootstrap project,  
run `cmake --build build` to build project.

## Usage

Run `./build/nlang <path/to/file.ns>` to compile a program into an AST.

You can test some examples from `examples` directory and see the AST.

## Features

These are features of nLang:

- [x] Type inference
- [x] Rust-like easy to read syntax
- [x] (WIP) Full C compatibility
- [ ] Ligthweight RTTI
- [ ] Static compile-time reflection

These are language things:

- [x] Comments
- [x] Functions
- [x] Return
- [ ] Let definitions
- [ ] Constants
- [ ] Numeric types
- [ ] Boolean type
- [ ] If->else if->else statements
- [ ] While loops
- [ ] Raw pointers
- [ ] Structs
- [ ] Struct methods
- [ ] Strings
- [ ] References
- [ ] Floats
- [ ] Iterators
- [ ] For loops
- [ ] Interfaces
- [ ] Generics
- [ ] Decorators
- [ ] Modules
- [ ] Standart library
- [ ] Option
- [ ] Result
- [ ] Self-hosted

## License

The project is licensed under Apache-2.0 license.
