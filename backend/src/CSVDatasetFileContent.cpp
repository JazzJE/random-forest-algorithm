#include <sstream>
#include <string>
#include <set>
#include <limits>

#include "CSVDatasetFileContent.h"
#include "exceptions.h"

// The struct should not have to create these parameters itself, and instead is provided them
CSVDatasetFileContent::CSVDatasetFileContent(
    DynamicArray<double> sample_data, 
    size_t number_of_samples, 
    DynamicArray<std::string> feature_names,     
    size_t number_of_features, 
    DynamicArray<std::string> sample_labels, 
    std::string target_label_column_name, 
    std::set<int> continuous_feature_indices
) : 
    sample_data(sample_data),
    number_of_samples(number_of_samples),
    feature_names(feature_names),
    number_of_features(number_of_features),
    sample_labels(sample_labels),
    target_label_column_name(target_label_column_name),
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
 * @note This function will return an empty object if the headers array and the headers of the file are not in the same order
 * 
 */
CSVDatasetFileContent processCSVDatasetFile(
    const char* csvData, 
    const DynamicArray<std::string>& headers_array,
    const DynamicArray<std::string>& continuous_features_names_array, 
    std::string target_label_column_name)
{
    // Validate if the headers array and headers are in the same order and there are the same amount
    // The headers array should be the first line
    std::istringstream file_content(csvData);
    std::string line, value;
    std::stringstream ss;
    
    getline(file_content, line);
    ss.clear();
    ss.str(line);

    // Check if each header element is the same as the file line element
    size_t counter_index = 0;
    while (getline(ss, value, ','))
    {
        // Get rid of the \r character if it's at the end of the value
        if (!value.empty() && value.back() == '\r')
            value.pop_back();

        if (value != headers_array.elements[counter_index])
            return CSVDatasetFileContent();
        ++counter_index;
    }

    // If the number of headers counted inside of the file is not the same as the number of elements in the header array, they are not the same
    if (counter_index != headers_array.number_of_elements)
        return CSVDatasetFileContent();

    // If there's still a header to parse, that means they are not the same
    if (getline (ss, value,','))
        return CSVDatasetFileContent();


    // Temp variables for parsing
    size_t target_label_column_index = INT_MAX;
    bool passed_over_target_column_index = false;

    // The number of features is equal to the number of headers - 1 (as one will be for targets)
    size_t number_of_features = headers_array.number_of_elements - 1;
    
    // Get the features names, which is simply every header that is not the target label
    DynamicArray<std::string> feature_names(number_of_features);
    // Also get which positions the continuous features will be
    std::set<int> continuous_feature_indices;
    for (size_t i = 0; i < headers_array.number_of_elements; i++)
    {
        // Below selection structure for processing column names
        // If the current header is the target, then set the target_label_column_index to the current column index
        if (target_label_column_name == headers_array.elements[i])
        {
            target_label_column_index = i;
            passed_over_target_column_index = true;
            continue;
        }
        // If we skipped over the target column, then set the feature name as the (i-1)th element as the target label takes up that -1
        if (passed_over_target_column_index)  
            feature_names.elements[i - 1] = headers_array.elements[i];
        // Else then just set the features array to the current header
        else
            feature_names.elements[i] = headers_array.elements[i];

        // Below structure for getting indices of the continuous features, only if there are continuous features
        if (continuous_features_names_array.number_of_elements != 0)
        {
            bool is_continuous_feature = false;
            // Check if the current feature is in the continuous feature array
            for (size_t j = 0; j < continuous_features_names_array.number_of_elements; ++j)
                if (headers_array.elements[i] == continuous_features_names_array.elements[j])
                {
                    is_continuous_feature = true;
                    break;
                }
            // If the feature was identified as continuous, then identify which feature index it will be
            if (is_continuous_feature)
            {  
                // If the passed over is enabled, then it's the (i - 1)th index
                if (passed_over_target_column_index)
                    continuous_feature_indices.insert(i - 1);
                // If the passed over is disabled, then it's the ith index
                else   
                    continuous_feature_indices.insert(i);
            }
        }
    }


    // The number of lines following validating the first line/headers of the file is simply the number of samples
    size_t number_of_samples = 0;
    while (getline(file_content, line, '\n'))
        ++number_of_samples;

    // Create a 1D flattened array for the sample data and sample labels
    DynamicArray<double> sample_data(number_of_samples * number_of_features);
    DynamicArray<std::string> sample_labels(number_of_samples); 

    // Go to beginning of file and ignore the headers
    file_content.clear();     
    file_content.seekg(0, std::ios::beg);
    getline(file_content, line, '\n');
    size_t current_line = 1;

    // For each sample, parse in the features
    for (size_t sample_index = 0; sample_index < number_of_samples; ++sample_index)
    {
        getline(file_content, line, '\n');
        ++current_line;
        passed_over_target_column_index = false;

        ss.clear();
        ss.str(line);

        // For each sample value
        for (size_t column_index = 0; column_index < headers_array.number_of_elements; ++column_index)
        {
            // Get the value and parse it into a double
            getline(ss, value, ',');
            double double_value;

            // Ensure there is no Windows ending line
            if (!value.empty() && value.back() == '\r')
                value.pop_back();

            // If the double_value fails to parse, then throw an exception
            try { double_value = std::stod(value); }
            catch (...) { throw CSVParseException(std::string("Error found on line ") + std::to_string(current_line)); }

            // If we are at the target label index, then continue to the next iteration after placing the string in the sample labels array
            if (column_index == target_label_column_index)
            {
                sample_labels.elements[sample_index] = value;

                passed_over_target_column_index = true;
                continue;
            }

            // If we passed the target column, then set the feature to the (i - 1)th column index
            if (passed_over_target_column_index)
                sample_data.elements[sample_index * number_of_features + (column_index - 1)] = double_value;
            // Else, then set the feature to the current ith column index
            else
                sample_data.elements[sample_index * number_of_features + column_index] = double_value;
        }
    }

    return CSVDatasetFileContent(
        sample_data, 
        number_of_samples, 
        feature_names, 
        number_of_features, 
        sample_labels, 
        target_label_column_name, 
        continuous_feature_indices
    );
}

