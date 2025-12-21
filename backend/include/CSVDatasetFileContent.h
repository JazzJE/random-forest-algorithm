#pragma once

#include <set>
#include <cstdint>
#include <memory>
#include <string>

// Simple struct to return which contains all the dataset information
struct CSVDatasetFileContent
{
    std::unique_ptr<double> sample_features;
    size_t number_of_samples;
    std::unique_ptr<std::string> features_names;
    size_t number_of_features;
    std::set<int> continuous_feature_indices;

    CSVDatasetFileContent(double* sample_features, size_t number_of_samples, std::string* features_names,
        size_t number_of_features, std::set<int> continuous_feature_indices);
};

CSVDatasetFileContent processCSVDatasetFile(std::string file_content);