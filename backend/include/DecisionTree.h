#pragma once

#include <memory>
#include <unordered_set>
#include <string>
#include "json.hpp"

#include "CSVDatasetFileContent.h"

class DecisionTree {
    public:
        // If both left_node, right_node = nullptr, then the node is a leaf node
        struct TreeNode
        {
            std::unique_ptr<TreeNode> left_node;
            std::unique_ptr<TreeNode> right_node;

            // Note that the splitting_feature_index will be set to -1 if it is a leaf node
            size_t splitting_feature_index;

            // Splitting value threshold; if the feature is a boolean, then should be set to 1
            // so that target_labels >= threshold have their samples go right
            double split_threshold_value;

            // These should only be used if they are leaf nodes
            bool is_leaf_node;
            std::string leaf_node_value;
        };

    private: 

        // Used for creating a Best First Search algorithm and determining other terminating conditions 
        // Where the first node will always be the node with the highest amount of information gain; max priority queue
        struct InfoGainPriorityQueue
        {
            public: 
                // These nodes should not be able to edit the tree node they point to
                struct InfoGainQueueNode 
                {
                    double information_gain; 
                    size_t current_level;
                    TreeNode* node_ptr;
                    DynamicArray<size_t> sample_indices;

                    InfoGainQueueNode() : information_gain(0), current_level(0), node_ptr(nullptr) {}
                    InfoGainQueueNode(double information_gain, size_t current_level, TreeNode* split_node_ptr) : 
                    information_gain(information_gain), current_level(current_level), node_ptr(split_node_ptr) {}
                };

            private:
                size_t getParentIndex(size_t child_index);
                size_t getLeftChildIndex(size_t parent_index);
                size_t getRightChildIndex(size_t parent_index);
                void resizeHeapArray(size_t new_max_number_of_nodes);

            size_t max_number_of_nodes;
            size_t current_number_of_nodes;
            DynamicArray<InfoGainQueueNode> heap_array;

            public: 

                void insertQueueNode(double information_gain, size_t current_level, TreeNode* node_ptr);
                InfoGainQueueNode dequePriorityNode();

                size_t getCurrentNumberOfNodes();
        };

        std::unique_ptr<TreeNode> root_node;
        
        // Helper function for leaves to determine which value appears the most in the group
        std::string computeLeafNodeValue(const CSVDatasetFileContent& dataset_content, const DynamicArray<size_t> current_sample_indices);

        // Reference variable for accessing the model's config settings for splitting
        const ModelConfigs& tree_ref_current_configs;

    public:

        // For building a decision tree from a dataset
        DecisionTree(const CSVDatasetFileContent& dataset_content, const ModelConfigs& model_ref_current_configs);

        // For building a decision tree from a provided JSON object
        DecisionTree(const nlohmann::json& provided_tree_json, const ModelConfigs& model_ref_current_configs);

        ~DecisionTree();

};