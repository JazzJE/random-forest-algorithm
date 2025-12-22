#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace Constants
{
    constexpr size_t DAT_FILE_IDENTIFIER = 0xDEADBEEF;
    constexpr std::string_view CONTINUOUS_FEATURE_IDENTIFIER = "isAContinuousFeature";
}