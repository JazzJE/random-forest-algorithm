#include "utils.h"
#include <memory>
#include <iostream>
#include <iomanip>

// Return all of the trees along with corresponding model/meta data into a JSON object
nlohmann::json convertModelToJSON(const RandomForestModel& provided_model)
{
    nlohmann::json j;
    return j;
}

// Use a DFS algorithm to traverse the tree and return all the nodes as a JSON object
nlohmann::json convertTreeToJSON(const DecisionTree& provided_tree)
{
    nlohmann::json j;
    return j;
}

// Program will load the saved JSON object for the model
RandomForestModel convertJSONToModel(nlohmann::json provided_model_json)
{
    return RandomForestModel();
}

// Helper method for loading the model
DecisionTree convertJSONToTree(nlohmann::json provided_tree_json)
{
    const float* samples;
    size_t number_of_samples;
    size_t number_of_features;
    std::unordered_set<size_t> continuous_feature_indices; 
    size_t max_depth_level;

    return DecisionTree(samples,
             number_of_samples,
             number_of_features,
             continuous_feature_indices, 
             max_depth_level);
}

/**
*   @brief Take in a string of an array (i.e. '["Apples", "Oranges", "Grapes"]') and then return a dynamic array of it as individual strings
*   
*   @note Each string inside of the string array should not have any extra '"' characters, as it will 
*       mess up the logic flow
*/
DynamicArray<std::string> parseStringArray(std::string string_array)
{   
    // Create an empty dynamic array object of strings
    DynamicArray<std::string> new_array;
    
    // Count the number of commas (c), where the number of elements will be c + 1
    for (size_t i = 0; i < string_array.length(); i++)
        if (string_array[i] == ',')
            new_array.number_of_elements += 1;
    new_array.number_of_elements += 1;
    
    // Edge cases where if there are no commas, meaning 0 or 1 elements
    // If the number of commas is 0 and there are no quotation marks, then there are no elements
    if (new_array.number_of_elements == 0 && string_array.find('"') == std::string::npos)
        return new_array;
    // Else if the number of commas is 0, the number of elements is therefore at least 1
    else if (new_array.number_of_elements == 0)
        new_array.number_of_elements = 1;

    new_array.elements = std::make_unique<std::string[]>(new_array.number_of_elements);
    size_t current_char_index = 0;
    size_t current_array_index = 0;
    bool string_append = false;

    // While we haven't reached the number of elements
    while (current_array_index != new_array.number_of_elements)
    {
        // Go to the next character
        current_char_index += 1;

        // Edge case for if there's whitespaces, we skip these characters
        if (isspace(string_array[current_char_index]))
            continue;

        // If the string append is currently false, the current character is a '"', and the previous character
        // was a '[' or ',', then enable it and continue to next character
        if (!string_append && 
            string_array[current_char_index] == '"' &&
            (string_array[current_char_index - 1] == '[' || string_array[current_char_index - 1] == ','))
                {
                    string_append = true;
                    continue;
                }

        // If the string append is currently true, the current character is a '"', and the previous character
        //  was a '[' or ',', then disable it and continue to the next character as well as next string
        if (string_append && 
            string_array[current_char_index] == '"' &&
            (string_array[current_char_index + 1] == ']' || string_array[current_char_index + 1] == ','))
                {
                    string_append = false;
                    current_array_index += 1;
                    continue;
                }

        // If the string append is enabled currently, then append the current character to the current 
        // array index element
        if (string_append)
        {
            std::string& current_string = new_array.elements[current_array_index];
            current_string += string_array[current_char_index];
        }
    }
    
    return new_array;
}

void printDataset(const CSVDatasetFileContent& data)
{
    std::cout << "\n========== CSV DATASET ==========\n";

    std::cout << "Samples:   " << data.number_of_samples << "\n";
    std::cout << "Features:  " << data.number_of_features << "\n";
    std::cout << "Target:    " << data.target_label_column_name << "\n\n";

    // Feature names
    std::cout << "Feature Names:\n";
    for (size_t i = 0; i < data.number_of_features; ++i)
    {
        std::cout << "  [" << i << "] " << data.feature_names.elements[i];

        if (data.continuous_feature_indices.count(i))
            std::cout << "  (continuous)";
        else
            std::cout << "  (boolean)";

        std::cout << "\n";
    }

    std::cout << "\n";

    // Sample data
    std::cout << "Samples:\n";

    for (size_t i = 0; i < data.number_of_samples; ++i)
    {
        std::cout << "Line " << i + 2 << "  Label: " << data.sample_labels.elements[i] << "\n";

        for (size_t j = 0; j < data.number_of_features; ++j)
        {
            size_t idx = i * data.number_of_features + j;

            std::cout << "   "
                      << std::setw(15) << data.feature_names.elements[j]
                      << " = "
                      << std::setw(8) << data.sample_data.elements[idx]
                      << "\n";
        }

        std::cout << "\n";
    }

    std::cout << "=================================\n";
}
