#ifndef NSCRIPT_DIAGNOSTIC_HPP
#define NSCRIPT_DIAGNOSTIC_HPP

#include <string>
#include <lang/span.hpp>

#define CONTEXT_LINES 3

std::string render(const std::string& source, Span span, const std::string& message, const std::string& path);

#endif
