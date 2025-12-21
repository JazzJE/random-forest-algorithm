#pragma once

#include "json.hpp"

#include "RandomForestModel.h"
#include "DecisionTree.h"
#include "CSVDatasetFileContent.h"

CSVDatasetFileContent processCSVDatasetFile(std::string file_content);

nlohmann::json convertModelToJSON(RandomForestModel provided_model);
nlohmann::json convertTreeToJSON(DecisionTree provided_tree);

RandomForestModel convertJSONToModel(nlohmann::json provided_model_json);
DecisionTree convertJSONToTree(nlohmann::json provided_tree_json);