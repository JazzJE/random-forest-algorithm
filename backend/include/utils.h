#pragma once

#include "json.hpp"
#include "random_forest_model.h"
#include "decision_tree.h"

nlohmann::json convertModelToJSON(RandomForestModel provided_model);
nlohmann::json convertTreeToJSON(DecisionTree provided_tree);