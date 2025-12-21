#include "CSVDatasetFileContent.h"

// The struct should not have to create these parameters itself, and instead is provided them
CSVDatasetFileContent::CSVDatasetFileContent(
    double* sample_features, 
    size_t number_of_samples, 
    std::string* features_names,
    size_t number_of_features, 
    std::set<int> continuous_feature_indices
) : 
    sample_features(sample_features), 
    number_of_samples(number_of_samples),
    features_names(features_names),
    number_of_features(number_of_features),
    continuous_feature_indices(continuous_feature_indices)
{ }

/**
 * @brief Returns a JSON object that contains all of the attributes of the CSV dataset
 * 
 * @param file_content String of the file content returned from an HTTP request
 * 
 * @return A struct with the following attributes:
 *      2D float array of values for the samples' features; each value can be accessed by ptr[number_of_features * sample_number + i]
 *      The number of samples
 *      The names of each feature
 *      The number of features each sample has
 *      The indices of the features which are continuously valued
 */
CSVDatasetFileContent processCSVDatasetFile(std::string file_content)
{
    double* sample_features = nullptr;
    size_t number_of_samples = 0;
    std::string* features_names = nullptr;
    size_t number_of_features = 0;
    std::set<int> continuous_feature_indices;

    // Count the number of features in the file by simply collecting the names of the headers, 
    // which should be the first line
    

    return CSVDatasetFileContent(sample_features, number_of_samples, features_names,
        number_of_features, continuous_feature_indices);
}

