#pragma once

#include <set>
#include <cstdint>
#include <memory>
#include <string>

// Simple struct to return which contains all the dataset information
struct CSVDatasetFileContent
{
    std::unique_ptr<double> sample_features;
    std::unique_ptr<size_t> sample_labels;
    size_t number_of_samples;
    std::unique_ptr<std::string> unique_labels_names;
    size_t number_of_unique_labels_names;
    std::unique_ptr<std::string> features_names;
    size_t number_of_features;
    std::set<int> continuous_feature_indices;

    CSVDatasetFileContent(double* sample_features, size_t* sample_labels, size_t number_of_samples, 
        std::string* labels_names, size_t number_of_unique_labels_names, std::string* features_names, 
        size_t number_of_features, std::set<int> continuous_feature_indices);
};

CSVDatasetFileContent processCSVDatasetFile(const char* csvData);