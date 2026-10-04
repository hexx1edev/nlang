cmake_minimum_required(VERSION 3.20)

project(nlang LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(LLVM REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_VERSION}")

file(GLOB_RECURSE NLANG_SOURCES CONFIGURE_DEPENDS
    src/*.cpp
    src/*.hpp
)

add_executable(nlang
    ${NLANG_SOURCES}
)

target_include_directories(nlang
    PRIVATE
        src
        ${LLVM_INCLUDE_DIRS}
)

# LLVM libraries used by nlang.
llvm_map_components_to_libnames(LLVM_LIBS
    Core
    Support
    IRReader
    native
)

target_link_libraries(nlang
    PRIVATE
        ${LLVM_LIBS}
)

# Enable C++ exception handling.
if(MSVC)
    target_compile_options(nlang PRIVATE /EHsc)
else()
    target_compile_options(nlang PRIVATE -fexceptions)
endif()
