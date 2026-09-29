#ifndef NSCRIPT_ERROR_HPP
#define NSCRIPT_ERROR_HPP

#include <iostream>
#include <sstream>

using Manip = std::ostream& (*)(std::ostream&);

class LogStream {
public:
    LogStream();
    ~LogStream();

    template<typename T>
    LogStream& operator<<(const T& value) {
        buffer << value;
        return *this;
    }

    LogStream& operator<<(Manip manip);

private:
    std::ostringstream buffer;
};

inline LogStream error() { return LogStream(); }


#endif
