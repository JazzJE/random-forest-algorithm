#pragma once

#include <string>

namespace APIResponseTypes
{
    // API response objects for when uploading a CSV dataset
    enum class FileUploadStatus 
    {
        Success,
        CSVParseError,
    };

    // API response objects for when generating a model with a dataset
    enum class ModelGenerateStatus
    {
        Success,
        ModelGenerationError,
    };
};

inline std::string fileUploadStatusToString(APIResponseTypes::FileUploadStatus status)
{
    switch (status)
    {
        case APIResponseTypes::FileUploadStatus::Success:           return "success";
        case APIResponseTypes::FileUploadStatus::CSVParseError:     return "csv_parse_error";
        default:                                         return "no_file_inputted";
    }
}

inline std::string modelGenerateStatusToString(APIResponseTypes::ModelGenerateStatus status)
{
    switch (status)
    {
        case APIResponseTypes::ModelGenerateStatus::Success:        return "success";
        case APIResponseTypes::ModelGenerateStatus::ModelGenerationError: return "model_failed_to_generate";
        default:                                                    return "no_model_generated";
    }
}