#ifndef NLANG_TYPES_HPP
#define NLANG_TYPES_HPP

#include <unordered_map>
#include <string_view>

namespace types {

enum class TypeKind {
    U8,
    U16,
    U32,
    U64,
    I8,
    I16,
    I32,
    I64,
    VOID,
    ERROR
};

struct Type {
    TypeKind kind;
    std::string_view name;

    constexpr bool operator==(const Type&) const = default;
};

constexpr Type U8{TypeKind::U8, "u8"};
constexpr Type U16{TypeKind::U16, "u16"};
constexpr Type U32{TypeKind::U32, "u32"};
constexpr Type U64{TypeKind::U64, "u64"};

constexpr Type I8{TypeKind::I8, "i8"};
constexpr Type I16{TypeKind::I16, "i16"};
constexpr Type I32{TypeKind::I32, "i32"};
constexpr Type I64{TypeKind::I64, "i64"};

constexpr Type VOID{TypeKind::VOID, "void"};
constexpr Type ERROR{TypeKind::ERROR, "<error>"};

const std::unordered_map<std::string_view, Type> BUILTIN = {
    {"u8",   U8},
    {"u16",  U16},
    {"u32",  U32},
    {"u64",  U64},
    {"i8",   I8},
    {"i16",  I16},
    {"i32",  I32},
    {"i64",  I64},
    {"void", VOID},
};

const std::vector<Type> SIGNED = {
    I8, I16, I32, I64
};

}

#endif
