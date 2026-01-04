#include <random>
#include <unordered_map>

#include "DecisionTree.h"
#include "math_functions.h"
#include "Constants.h"

DecisionTree::DecisionTree(const CSVDatasetFileContent& dataset_content, size_t maximum_depth_level)
{
    // Select a number of random samples with replacement for the initial group to be split
    DynamicArray<size_t> initial_sample_indices(dataset_content.number_of_samples);
    for (size_t i = 0; i < dataset_content.number_of_samples; ++i)
        initial_sample_indices.elements[i] = std::rand() % dataset_content.number_of_samples;

    size_t current_depth_level = 1;
    split_node(root_node, initial_sample_indices, current_depth_level, dataset_content, maximum_depth_level);
}

DecisionTree::DecisionTree(const nlohmann::json& provided_tree_json)
{
    return;
}

DecisionTree::~DecisionTree()
{ return; }

void DecisionTree::split_node(const std::unique_ptr<Node>& current_node_ptr, const DynamicArray<size_t>& current_sample_indices, size_t current_depth_level, const CSVDatasetFileContent& dataset_content, size_t max_depth_level)
{
    // If we've already reached the maximum depth level, then stop processing and make this a leaf node
    if (current_depth_level >= max_depth_level)
    {
        current_node_ptr->splitting_feature_index = Constants::IS_LEAF_NODE_IDENTIFIER;

    }

    // Select sqrt(n) features to randomly be chosen for the current splitting
}

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