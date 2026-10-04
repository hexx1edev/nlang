#ifndef NLANG_DIAGNOSTIC_HPP
#define NLANG_DIAGNOSTIC_HPP

#include <string>
#include <lang/span.hpp>

#define CONTEXT_LINES 3

std::string render(const std::string& source, Span span, const std::string& message, const std::string& path);

#endif
