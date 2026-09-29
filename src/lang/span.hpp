#ifndef NSCRIPT_SPAN_HPP
#define NSCRIPT_SPAN_HPP

class Span {
public:
    Span(int start, int end);
    ~Span();

    int start;
    int end;

    Span to(Span other);
};

#endif