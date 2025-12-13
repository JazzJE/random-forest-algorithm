#pragma once

#include "json.hpp"

class DecisionTree {
    private:

    public:
        DecisionTree(float** samples, size_t number_of_features);
        ~DecisionTree();

        nlohmann::json convertTreeToJSON();

};