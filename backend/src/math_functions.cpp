#include <cmath>
#include <memory>
#include <string>
#include <unordered_map>

#include "math_functions.h"

// Uses basic entropy calculation with log functions
double calculateEntropy(DynamicArray<std::string> target_labels)
{
	// Count the number of samples in each target label group
	std::unordered_map<std::string, int> sample_label_counter;
	for (size_t i = 0; i < target_labels.number_of_elements; ++i)
	{
		std::string current_label = target_labels.elements[i];
		
		// If the target label of the current sample doesn't exist in the map yet, then set it as 0
		if (sample_label_counter.find(current_label) == sample_label_counter.end())
			sample_label_counter[current_label] = 0;

		// Increase the count of the current label
		sample_label_counter[current_label]++;
	}

	// Calculate the entropy of each group and add it to the entropy sum
	double entropy_sum = 0;
	for (const auto& [_, count] : sample_label_counter) 
	{
		// If the number of samples is zero for the current target label, then add 0/just continue
		if (count == 0)
			continue;
		
		// Else, add the current entropy of the samples
		double probability = static_cast<double>(count) / target_labels.number_of_elements;
		entropy_sum += probability * log2(probability);
	}

	// Flip the negative of the entropy sum
	entropy_sum = -entropy_sum;

	return entropy_sum;
}