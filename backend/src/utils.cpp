#include <memory>
#include <iostream>
#include <iomanip>

#include "utils.h"

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

// Return a set of random indices, that range from 0 to number_of_elements - 1
std::set<size_t> getRandomSetOfFeatureIndices(size_t number_of_desired_indices, size_t number_of_elements)
{
    std::set<size_t> set_indices = {};

    // Return an empty set if the number of desired indices is greater than the number of elements, or if the numbers are negative
    if (number_of_desired_indices > number_of_elements || number_of_desired_indices < 0 || number_of_elements < 0)
        return set_indices;

    for (size_t i = 0; i < number_of_desired_indices; ++i)
    {
        size_t selected_index = std::rand() % number_of_elements;
        while (set_indices.find(selected_index) != set_indices.end())
            selected_index = std::rand() % number_of_elements;
        set_indices.insert(selected_index);
    }

    return set_indices;
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

    // Edge case where there are zero elements, meaning there should be no quotation marks in the array
    if (string_array.find('"') == std::string::npos)
        return new_array;
    
    // Count the number of commas (c), where the number of elements will be c + 1
    for (size_t i = 0; i < string_array.length(); i++)
        if (string_array[i] == ',')
            new_array.number_of_elements += 1;
    new_array.number_of_elements += 1;

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
