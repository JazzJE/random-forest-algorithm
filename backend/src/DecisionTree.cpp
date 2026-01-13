#include <random>
#include <unordered_map>

#include "DecisionTree.h"
#include "math_functions.h"
#include "Constants.h"

DecisionTree::DecisionTree(
    const CSVDatasetFileContent& dataset_content, 
    const ModelConfigs& model_ref_current_configs) 
    : tree_ref_current_configs(model_ref_current_configs)
{
    // Select a number of random samples with replacement for the initial group to be split
    DynamicArray<size_t> initial_sample_indices(tree_ref_current_configs.number_of_bootstrapped_samples_per_tree);
    for (size_t i = 0; i < tree_ref_current_configs.number_of_bootstrapped_samples_per_tree; ++i)
        initial_sample_indices.elements[i] = std::rand() % dataset_content.number_of_samples;

    size_t current_depth_level = 1;
}

DecisionTree::DecisionTree(const nlohmann::json& provided_tree_json, const ModelConfigs& model_ref_current_configs)
// Temporary to make it pass compiler errors
: tree_ref_current_configs(model_ref_current_configs)
{
    return;
}

DecisionTree::~DecisionTree()
{ return; }

std::string DecisionTree::computeLeafNodeValue(const CSVDatasetFileContent& dataset_content, const DynamicArray<size_t> current_sample_indices)
{
    // Keep running sums of each of the key strings in the hash map
    std::unordered_map<std::string, size_t> current_target_sums;
    for (size_t i = 0; i < current_sample_indices.number_of_elements; ++i)
    {
        size_t current_sample_index = current_sample_indices.elements[i];
        std::string current_target_label = dataset_content.sample_labels.elements[current_sample_index];

        current_target_sums[current_target_label]++;
    }

    // Linear search to check which sum is the greatest; if there are two targets with the same maximum, then select the first target
    // As the maximum target
    size_t current_greatest_sum = 0;
    std::string current_greatest_sum_label = "";
    for (const auto& kv_pair : current_target_sums)
        if (kv_pair.second > current_greatest_sum)
        {
            current_greatest_sum = kv_pair.second;
            current_greatest_sum_label = kv_pair.first;
        }
    
    return current_greatest_sum_label;
}