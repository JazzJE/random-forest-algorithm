#pragma once
#include <stdexcept>
#include <string>

class CSVParseException : public std::runtime_error
{
public:
    explicit CSVParseException(const std::string& msg)
        : std::runtime_error(msg) {}
};