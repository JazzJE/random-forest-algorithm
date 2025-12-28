#pragma once

#include <cstdint>
#include <string>
#include <string_view>

namespace Constants
{
    constexpr size_t DAT_FILE_IDENTIFIER = 0xDEADBEEF;
    constexpr std::string_view CONTINUOUS_FEATURE_IDENTIFIER = "isAContinuousFeature";

    enum class UploadStatus 
    {
        Success,
        CSVParseError,
    };
}

std::string to_string(Constants::UploadStatus status)
{
    switch (status)
    {
        case Constants::UploadStatus::Success:           return "success";
        case Constants::UploadStatus::CSVParseError:     return "csv_parse_error";
        default:                                         return "no_file_inputted";
    }
}