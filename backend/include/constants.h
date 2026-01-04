#pragma once

#include <cstdint>
#include <string>

namespace Constants
{
    constexpr size_t DAT_FILE_IDENTIFIER = 0xDEADBEEF;
    constexpr signed int IS_LEAF_NODE_IDENTIFIER = -1;
    
    enum class UploadStatus 
    {
        Success,
        CSVParseError,
    };
}

inline std::string statusToString (Constants::UploadStatus status)
{
    switch (status)
    {
        case Constants::UploadStatus::Success:           return "success";
        case Constants::UploadStatus::CSVParseError:     return "csv_parse_error";
        default:                                         return "no_file_inputted";
    }
}