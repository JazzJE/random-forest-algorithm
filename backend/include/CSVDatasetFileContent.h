#pragma once

#include <set>
#include <cstdint>
#include <memory>
#include <string>

#include "containers.h"

// Simple struct to return which contains all the dataset information
struct CSVDatasetFileContent
{
    // Flattened dynamic array containing all of the features of the samples
    // The ith sample's kth feature can be accessed by samples[(i - 1) * number_of_features + k]
    DynamicArray<double> sample_data;
    size_t number_of_samples;
    DynamicArray<std::string> feature_names;
    size_t number_of_features;

    // 1D array where each the target label of the ith sample is given by the (i-1)th element in this array
    DynamicArray<std::string> sample_labels;
    std::string target_label_column_name;

    // Stores which indices of the features are continuous, and if not in this set, then should be treated as boolean values 
    std::set<int> continuous_feature_indices;

    CSVDatasetFileContent(DynamicArray<double> sample_data, size_t number_of_samples, DynamicArray<std::string> feature_names, 
        size_t number_of_features, DynamicArray<std::string> sample_labels, std::string target_label_column_name, 
        std::set<int> continuous_feature_indices);

    CSVDatasetFileContent() = default;
};

CSVDatasetFileContent processCSVDatasetFile(const char* csvData, 
    const DynamicArray<std::string>& headers_array,
    const DynamicArray<std::string>& continuous_features_names_array, 
    std::string target_label_column_name);