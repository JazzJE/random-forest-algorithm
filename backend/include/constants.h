#pragma once

#include <cstdint>
#include <string_view>

namespace Constants
{
    constexpr std::string_view FIELD_EXISTS_MESSAGE = "Field found";
    constexpr size_t DAT_FILE_IDENTIFIER = 0xDEADBEEF;

    // Decision tree leaf identifier
    constexpr signed int IS_LEAF_NODE_IDENTIFIER = std::numeric_limits<unsigned>::max();

    // Priority queue constants
    constexpr size_t INIT_NUMBER_OF_NODES = 32;
    constexpr size_t INVALID_INDEX = std::numeric_limits<size_t>::max();

};