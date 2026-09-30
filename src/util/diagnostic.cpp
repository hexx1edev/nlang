#include "diagnostic.hpp"
#include <algorithm>
#include <util/color.hpp>
#include <vector>
#include <sstream>

std::vector<std::string> split_lines(const std::string& s) {
    std::vector<std::string> lines;
    size_t start = 0;
    size_t end = s.find('\n');
    while (end != std::string::npos) {
        lines.push_back(s.substr(start, end - start));
        start = end + 1;
        end = s.find('\n', start);
    }
    lines.push_back(s.substr(start));
    return lines;
}

std::string render(const std::string& source, Span span, const std::string& message, const std::string& path) {
    auto lines = split_lines(source);
    auto [line, column] = span.location(source);

    int first = std::max(1, static_cast<int>(line) - CONTEXT_LINES);
    int last = std::min(static_cast<int>(lines.size()), static_cast<int>(line) + CONTEXT_LINES);
    int width = std::to_string(last).length();

    std::ostringstream out;
    std::string spaces(width, ' ');

    out << message << std::endl;
    out << color::cyan << spaces << "--> " << color::reset << path << ":" << line << ":" << column << std::endl;
    out << color::cyan << spaces << " |" << color::reset;

    for (int number = first; number <= last; ++number) {
        out << std::endl;
        std::string text = lines[number - 1];

        // Strip trailing carriage return if present
        if (!text.empty() && text.back() == '\r') {
            text.pop_back();
        }
        // Replace tabs with spaces
        std::replace(text.begin(), text.end(), '\t', ' ');

        std::string num_str = std::to_string(number);
        std::string padded_num(width - num_str.length(), ' ');
        padded_num += num_str;

        std::string gutter = color::cyan + padded_num + " |" + color::reset;

        if (static_cast<size_t>(number) == line) {
            size_t col = column - 1;
            size_t length = std::max(size_t(1), std::min(span.end - span.start, text.length() - col));

            std::string highlighted = text.substr(0, col) +
                                    color::red + color::bold + text.substr(col, length) + color::reset +
                                    text.substr(col + length);

            out << gutter << " " << highlighted << std::endl;

            std::string carets(col, ' ');
            carets.append(length, '^');
            out << color::cyan << spaces << " |" << color::reset << " " << color::red << color::bold << carets << color::reset;
        } else {
            out << gutter << " " << text;
        }
    }

    return out.str();
}
