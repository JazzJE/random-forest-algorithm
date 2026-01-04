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
        
        // For constructing the model from a dataset
        RandomForestModel(const CSVDatasetFileContent& dataset_content, size_t selected_number_of_trees, size_t maximum_depth_level);
        
        // For constructing the model from a presaved JSON object
        RandomForestModel(const nlohmann::json& provided_model_json);

        ~RandomForestModel();

};