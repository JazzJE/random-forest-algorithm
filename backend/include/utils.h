#pragma once

#include "json.hpp"
#include <string>

#include "RandomForestModel.h"
#include "DecisionTree.h"
#include "containers.h"

nlohmann::json convertModelToJSON(const RandomForestModel& provided_model);
nlohmann::json convertTreeToJSON(const DecisionTree& provided_tree);

RandomForestModel convertJSONToModel(nlohmann::json provided_model_json);
DecisionTree convertJSONToTree(nlohmann::json provided_tree_json);

DynamicArray<std::string> parseStringArray(std::string string_array);