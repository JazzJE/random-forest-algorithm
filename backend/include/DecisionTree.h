#pragma once

#include <memory>
#include <unordered_set>

class DecisionTree {
    private:

        // If both left_node, right_node = nullptr, then the node is a leaf node
        struct Node
        {
            std::unique_ptr<Node> left_node;
            std::unique_ptr<Node> right_node;
            size_t splitting_feature_index;
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

        Node* root_node;

    public:

        DecisionTree(
            const float* samples,
            size_t number_of_samples,
            size_t number_of_features,
            std::unordered_set<size_t> continuous_feature_indices, 
            size_t max_depth_level
        );
        ~DecisionTree();

};