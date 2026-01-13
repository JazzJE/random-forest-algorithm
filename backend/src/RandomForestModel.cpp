#include "RandomForestModel.h"

RandomForestModel::RandomForestModel(const CSVDatasetFileContent& dataset_content, 
    const ModelConfigs& provided_configs) :
    decision_tree_array(DynamicArray<std::unique_ptr<DecisionTree>>(provided_configs.number_of_trees)),
    model_ref_current_configs(provided_configs)
{ 
    // Build each decision tree using the dataset content
    for (size_t i = 0; i < model_ref_current_configs.number_of_trees; i++)
        decision_tree_array.elements[i] = std::make_unique<DecisionTree>(dataset_content, model_ref_current_configs);
}

RandomForestModel::RandomForestModel(const nlohmann::json& provided_model_json)
// Temporary to make it pass compiler errors
: model_ref_current_configs(ModelConfigs(0,0,0,0,0,0))
{
    return;
}

RandomForestModel::~RandomForestModel()
{ return; }