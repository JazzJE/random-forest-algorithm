#pragma once

#include <cstdint>

#include "DecisionTree.h"

class RandomForestModel {
    private:
        uint8_t number_of_trees;
        DecisionTree* decision_tree_array;

    public:
        RandomForestModel();
        ~RandomForestModel();

};