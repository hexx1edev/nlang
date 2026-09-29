#include "span.hpp"

Span::Span(int start, int end) : start(start), end(end) {}
Span::~Span() {}

Span Span::to(Span other) {
    return Span(start, other.end);
}