#include <sstream>
#include <string>
#include <set>

#include "CSVDatasetFileContent.h"

// The struct should not have to create these parameters itself, and instead is provided them
CSVDatasetFileContent::CSVDatasetFileContent(
    double* sample_features, 
    size_t* sample_labels, 
    size_t number_of_samples,     
    std::string* unique_labels_names, 
    size_t number_of_unique_labels_names,
    std::string* features_names, 
    size_t number_of_features, 
    std::set<int> continuous_feature_indices
) : 
    sample_features(sample_features), 
    sample_labels(sample_labels),
    number_of_samples(number_of_samples),
    unique_labels_names(unique_labels_names),
    number_of_unique_labels_names(number_of_unique_labels_names),
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
 * 
 * @note The last column of the dataset should ALWAYS be the target labels
 */
CSVDatasetFileContent processCSVDatasetFile(const char* csvData)
{
    double* sample_features = nullptr;
    size_t* sample_labels = nullptr;
    size_t number_of_samples = 0;
    std::string* unique_labels_names = nullptr;
    size_t number_of_unique_labels_names = 0;
    std::string* features_names = nullptr;
    size_t number_of_features = 0;
    std::set<int> continuous_feature_indices;

    std::set<std::string> labels;

    // Copy the file into the program
    std::istringstream file_content(csvData);

    // Count the number of features in the file by simply collecting the names of the headers, 
    // which should be in the first line
    std::string line, value;
    std::stringstream ss;
    getline(file_content, line);

    ss.clear();
    ss.str(line);

    return CSVDatasetFileContent(
        sample_features, 
        sample_labels, 
        number_of_samples,     
        unique_labels_names, 
        number_of_unique_labels_names,
        features_names, 
        number_of_features, 
        continuous_feature_indices
    );
}

