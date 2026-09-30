#ifndef NSCRIPT_SPAN_HPP
#define NSCRIPT_SPAN_HPP

#include <string>

class Span {
public:
    Span(size_t start, size_t end);
    ~Span();

    size_t start;
    size_t end;

    Span to(Span other);
    std::pair<size_t, size_t> location(const std::string& source) const;
};

#endif
