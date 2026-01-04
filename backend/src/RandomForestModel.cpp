#include "RandomForestModel.h"

RandomForestModel::RandomForestModel(const CSVDatasetFileContent& dataset_content, size_t selected_number_of_trees, size_t maximum_depth_level) :
    decision_tree_array(DynamicArray<std::unique_ptr<DecisionTree>>(selected_number_of_trees))
{ 
    // Build each decision tree using the dataset content
    for (size_t i = 0; i < selected_number_of_trees; i++)
        decision_tree_array.elements[i] = std::make_unique<DecisionTree>(dataset_content, maximum_depth_level);
}

RandomForestModel::RandomForestModel(const nlohmann::json& provided_model_json)
{
    return;
}

RandomForestModel::~RandomForestModel()
{ return; }