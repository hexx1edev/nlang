cmake_minimum_required(VERSION 3.20)

project(nscript LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE NSCRIPT_SOURCES CONFIGURE_DEPENDS src/*.cpp src/*.hpp)

add_executable(nscript
    ${NSCRIPT_SOURCES}
)

target_include_directories(nscript PUBLIC src)