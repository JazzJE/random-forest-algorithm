#pragma once

#include <string>
#include <cstdint>
#include "json.hpp"

#include "RandomForestModel.h"
#include "DecisionTree.h"
#include "containers.h"
#include "CSVDatasetFileContent.h"

nlohmann::json convertModelToJSON(const RandomForestModel& provided_model);
nlohmann::json convertTreeToJSON(const DecisionTree& provided_tree);

std::set<size_t> getRandomSetOfFeatureIndices(size_t number_of_desired_indices, size_t number_of_elements);

DynamicArray<std::string> parseStringArray(std::string string_array);

// Functions primarily used for testing purposes
void printDataset(const CSVDatasetFileContent& data);
void printDatasetMetadata(const CSVDatasetFileContent& data);
void printModelConfigs(const ModelConfigs& current_configs);