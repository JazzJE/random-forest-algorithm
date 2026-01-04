#pragma once

#include <memory>
#include <unordered_set>
#include <string>
#include "json.hpp"

#include "CSVDatasetFileContent.h"

class DecisionTree {
    private:

        // If both left_node, right_node = nullptr, then the node is a leaf node
        struct Node
        {
            std::unique_ptr<Node> left_node;
            std::unique_ptr<Node> right_node;

            // Note that the splitting_feature_index will be set to -1 if it is a leaf node
            size_t splitting_feature_index;
            std::string leaf_node_value = "";

            virtual ~Node() {};  
        };

        // Splitting a discrete valued feature
        // Left nodes are always false groups
        // Right nodes are always true groups
        struct DiscreteSplitNode : public Node 
        {
            DiscreteSplitNode();
            ~DiscreteSplitNode() override;
        };

        // Splitting a continuous valued feature
        // Left nodes are always "less than the feature" groups
        // Right nodes are always "greater than or equal to the feature" groups
        struct ContinuousSplitNode : public Node
        {
            float split_value;
            ContinuousSplitNode();
            ~ContinuousSplitNode() override;
        };

        std::unique_ptr<Node> root_node;

        // Recursive function for building and stepping through each pointer to build the tree
        void split_node(const std::unique_ptr<Node>& current_node_ptr, 
            const DynamicArray<size_t>& current_sample_indices,
            size_t current_depth_level, 
            const CSVDatasetFileContent& dataset_content, 
            size_t max_depth_level);
        
        // Helper function for leaves to determine which value appears the most in the group
        std::string computeLeafNodeValue(const CSVDatasetFileContent& dataset_content, const DynamicArray<size_t> current_sample_indices);

    public:

        // For building a decision tree from a dataset
        DecisionTree(const CSVDatasetFileContent& dataset_content, size_t maximum_depth_level);

        // For building a decision tree from a provided JSON object
        DecisionTree(const nlohmann::json& provided_tree_json);

        ~DecisionTree();

};