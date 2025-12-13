#pragma once

#include <cstdint>
#include "json.hpp"

#include "decision_tree.h"

class RandomForestModel {
    private:
        uint8_t number_of_trees;
        DecisionTree* decision_tree_array;
        
        uint8_t number_of_target_labels;

    public:
        RandomForestModel();
        ~RandomForestModel();

        nlohmann::json convertModelToJSON();
};