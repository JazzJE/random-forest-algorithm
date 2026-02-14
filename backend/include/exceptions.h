#pragma once

#include <stdexcept>
#include <string>

class CSVParseException : public std::runtime_error
{
public:
    explicit CSVParseException(const std::string& msg)
        : std::runtime_error(msg) {}
};

enum {
    INVALID_INPUT_INDEX = std::numeric_limits<size_t>::max(),
    INACESSIBLE_CHILD_INDEX = std::numeric_limits<size_t>::max() - 1,
} EntropyQueueError;