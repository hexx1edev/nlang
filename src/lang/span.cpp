#include "span.hpp"
#include <algorithm>
#include <string>

Span::Span(size_t start, size_t end) : start(start), end(end) {}
Span::~Span() {}

Span Span::to(Span other) {
    return Span(start, other.end);
}

std::pair<size_t, size_t> Span::location(const std::string& source) const {
    size_t actual_start = std::min(start, source.size());
    size_t line = 1;

    for (size_t i = 0; i < actual_start; ++i) {
        if (source[i] == '\n') {
            line++;
        }
    }

    size_t last_nl = (actual_start > 0) ? source.rfind('\n', actual_start - 1) : std::string::npos;
    size_t column = (last_nl == std::string::npos) ? (actual_start + 1) : (actual_start - last_nl);

    return {line, column};
}
