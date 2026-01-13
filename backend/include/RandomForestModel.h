#pragma once

#include <cstdint>
#include "json.hpp"

#include "DecisionTree.h"
#include "CSVDatasetFileContent.h"
#include "containers.h"

class RandomForestModel {
    private:
        DynamicArray<std::unique_ptr<DecisionTree>> decision_tree_array;

    public:
        
        // Store the options sent by the user for generating a model and its trees into a finite container
        // Such that trees can easily access the info for its termination limits
        ModelConfigs model_ref_current_configs;

        // For constructing the model from a dataset
        RandomForestModel(const CSVDatasetFileContent& dataset_content, const ModelConfigs& provided_configs);
        
        // For constructing the model from a presaved JSON object
        RandomForestModel(const nlohmann::json& provided_model_json);

        ~RandomForestModel();

};