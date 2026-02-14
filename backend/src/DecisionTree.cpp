#include <random>
#include <unordered_map>

#include "DecisionTree.h"
#include "math_functions.h"
#include "utils.h"
#include "Constants.h"

size_t DecisionTree::InfoGainPriorityQueue::getParentIndex(size_t child_index)
{
    // If index is the zero index OR if index computed is beyond the max node index 
    // (which is max_number_of_nodes - 1), then return invalid index constant
    if (child_index == 0 || child_index >= max_number_of_nodes)
        return Constants::INVALID_INDEX;

    // If the child is even, then do the first, else do the second if odd
    if (!(child_index & 1))
        return (child_index - 1) / 2;
    else 
        return child_index / 2;
}

size_t DecisionTree::InfoGainPriorityQueue::getLeftChildIndex(size_t parent_index)
{
    // Return invalid index if the inputted index is beyond the max number of nodes
    if (parent_index >= max_number_of_nodes)
        return Constants::INVALID_INDEX;
    
    // If the computed left child index is beyond or equal to the current number of nodes, 
    // then return an invalid index
    size_t left_child_index = parent_index * 2 + 1;
    if (left_child_index >= current_number_of_nodes)
        return Constants::INVALID_INDEX;

    return left_child_index;
}

size_t DecisionTree::InfoGainPriorityQueue::getRightChildIndex(size_t parent_index)
{
    // Return invalid index if the inputted index is beyond the max number of nodes
    if (parent_index >= max_number_of_nodes)
        return Constants::INVALID_INDEX;

    // If the computed right child index is beyond or equal to the max number of nodes,
    // then return an invalid index
    size_t right_child_index = parent_index * 2 + 2;
    if (right_child_index >= current_number_of_nodes)
        return Constants::INVALID_INDEX;

    return right_child_index;
}

void DecisionTree::InfoGainPriorityQueue::resizeHeapArray(size_t new_max_number_of_nodes)
{
    DynamicArray<InfoGainQueueNode> temp(new_max_number_of_nodes);

    for (size_t i = 0; i < current_number_of_nodes; i++)
        temp.elements[i] = std::move(heap_array.elements[i]);

    heap_array = std::move(temp);
}

void DecisionTree::InfoGainPriorityQueue::insertQueueNode(
    double information_gain, size_t current_level, TreeNode* node_ptr
)
{
    InfoGainQueueNode new_node = InfoGainQueueNode(information_gain, current_level, node_ptr);

    // Double the size of the heap array if there is not enough space to accompany the new node
    if (current_number_of_nodes >= max_number_of_nodes)
    {
        size_t new_max_number_of_nodes = max_number_of_nodes * 2;
        resizeHeapArray(new_max_number_of_nodes);

        max_number_of_nodes = new_max_number_of_nodes;
    }
    
    // Go to the end of the heap array and add the node
    size_t last_element_index = current_number_of_nodes;
    heap_array.elements[last_element_index] = std::move(new_node);
    current_number_of_nodes++;

    // Propagate the node up the tree depending on the info gain
    size_t current_node_index = last_element_index;
    bool heap_property_satisfied = false;
    while (!heap_property_satisfied)
    {
        size_t parent_index = getParentIndex(current_node_index);

        // Edge case for if the current index is the first element/index, then the heap property was satisfied
        if (parent_index == Constants::INVALID_INDEX)
        {
            heap_property_satisfied = true;
            continue;
        }
        
        InfoGainQueueNode& parent_node = heap_array.elements[parent_index];
        InfoGainQueueNode& current_node = heap_array.elements[current_node_index];
        // If the info gain of current node is greater than info gain of parent, then switch their places
        // and set the current_node_index to the current_parent_index
        if (current_node.information_gain > parent_node.information_gain)
            {
                InfoGainQueueNode temp = std::move(parent_node);
                parent_node = std::move(current_node);
                current_node = std::move(temp);

                current_node_index = parent_index;
            }
        // Else, then the heap property has been satisfied
        else   
            heap_property_satisfied = true;
    }
}

DecisionTree::InfoGainPriorityQueue::InfoGainQueueNode DecisionTree::InfoGainPriorityQueue::dequePriorityNode()
{
    // Edge case for if the list is empty
    if (current_number_of_nodes <= 0)
        return InfoGainQueueNode();
    
    // Get the pointer attached to the first node
    InfoGainQueueNode return_node = std::move(heap_array.elements[0]);

    // Move the last element to the first element
    --current_number_of_nodes;
    heap_array.elements[0] = std::move(heap_array.elements[current_number_of_nodes]);

    // Propagate the first node down the tree until max heap property is satisfied
    bool heap_property_satisfied = false;
    size_t current_index = 0;
    while (!heap_property_satisfied)
    {
        size_t left_child_index = getLeftChildIndex(current_index);
        size_t right_child_index = getRightChildIndex(current_index);
    
        // Find which indexed node has the largest information gain
        size_t largest_info_gain_index = current_index;
        if (left_child_index != Constants::INVALID_INDEX &&
            heap_array.elements[largest_info_gain_index].information_gain < heap_array.elements[left_child_index].information_gain)
            largest_info_gain_index = left_child_index;
        if (right_child_index != Constants::INVALID_INDEX &&
            heap_array.elements[largest_info_gain_index].information_gain < heap_array.elements[right_child_index].information_gain)
            largest_info_gain_index = right_child_index;

        // If the largest info gain index is the current index, then the heap property was satisfied
        if (largest_info_gain_index == current_index)
        {
            heap_property_satisfied = true;
            continue;
        }

        InfoGainQueueNode& current_node = heap_array.elements[current_index];
        InfoGainQueueNode& largest_info_gain_node = heap_array.elements[largest_info_gain_index];

        InfoGainQueueNode temp = std::move(current_node);
        current_node = std::move(largest_info_gain_node);
        largest_info_gain_node = std::move(temp);

        current_index = largest_info_gain_index;
    }

    return return_node;
}

size_t DecisionTree::InfoGainPriorityQueue::getCurrentNumberOfNodes()
{ return current_number_of_nodes; }

DecisionTree::DecisionTree(
    const CSVDatasetFileContent& dataset_content, 
    const ModelConfigs& model_ref_current_configs) 
    : tree_ref_current_configs(model_ref_current_configs)
{
    // Select a number of bootstrapped random samples with replacement for the initial node to split
    DynamicArray<size_t> initial_sample_indices(tree_ref_current_configs.number_of_bootstrapped_samples_per_tree);
    for (size_t i = 0; i < tree_ref_current_configs.number_of_bootstrapped_samples_per_tree; ++i)
        initial_sample_indices.elements[i] = std::rand() % dataset_content.number_of_samples;
    
    // Create the first node in memory
    root_node = std::make_unique<TreeNode>();

    // Add the first node to the queue
    InfoGainPriorityQueue node_queue;
    double init_information_gain = 1;
    size_t init_depth_level = 1;
    node_queue.insertQueueNode(init_information_gain, init_depth_level, root_node.get());

    // While there are still elements inside of the info gain priority queue, then continue adding tree nodes
    while (node_queue.getCurrentNumberOfNodes() > 0)
    {
        InfoGainPriorityQueue::InfoGainQueueNode priority_node = node_queue.dequePriorityNode();

        // Compute the best split for the first node given the original sample set
        std::set<size_t> random_feature_indices = getRandomSetOfFeatureIndices(
            sqrt(dataset_content.number_of_features), 
            dataset_content.number_of_features);

        // Calculate the next level depth, where if it's equal to or beyond the max depth level,
        // then create neither of the two child nodes
        // Else, add them to the priority queue
        size_t next_level_depth = priority_node.current_level + 1;
        if (next_level_depth >= tree_ref_current_configs.maximum_depth_level)
            continue;
            
        TreeNode* current_node = priority_node.node_ptr;
        current_node->right_node = std::make_unique<TreeNode>();
        current_node->left_node = std::make_unique<TreeNode>();
    }
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