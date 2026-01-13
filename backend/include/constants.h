#pragma once

#include <cstdint>
#include <string_view>

namespace Constants
{
    constexpr std::string_view FIELD_EXISTS_MESSAGE = "Field found";
    constexpr size_t DAT_FILE_IDENTIFIER = 0xDEADBEEF;
    constexpr signed int IS_LEAF_NODE_IDENTIFIER = -1;
};