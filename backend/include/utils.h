#pragma once

#include <string>
#include <cstdint>
#include "json.hpp"

#include "RandomForestModel.h"
#include "DecisionTree.h"
#include "containers.h"

nlohmann::json convertModelToJSON(RandomForestModel provided_model);
nlohmann::json convertTreeToJSON(DecisionTree provided_tree);

RandomForestModel convertJSONToModel(nlohmann::json provided_model_json);
DecisionTree convertJSONToTree(nlohmann::json provided_tree_json);

DynamicArray<std::string> parseStringArray(std::string string_array);