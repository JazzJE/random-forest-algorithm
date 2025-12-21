#include "utils.h"

// Return all of the trees along with corresponding model/meta data into a JSON object
nlohmann::json convertModelToJSON(RandomForestModel provided_model)
{
    nlohmann::json j;
    return j;
}

// Use a DFS algorithm to traverse the tree and return all the nodes as a JSON object
nlohmann::json convertTreeToJSON(DecisionTree provided_tree)
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