#include "error.hpp"
#include <util/color.hpp>

LogStream::LogStream() {}
LogStream::~LogStream() {
    std::cout << color::red << color::bold << "error" << color::reset << color::bold << ": " << color::reset << buffer.str() << std::endl;
}