#pragma once

#include <cstdint>

template<class T>
struct DynamicArray
{
    std::unique_ptr<T[]> elements;
    size_t number_of_elements;

    DynamicArray() : elements(nullptr), number_of_elements(0) { }

    explicit DynamicArray(size_t count) : elements(std::make_unique<T[]>(count)), number_of_elements(count) {}

    // Copy constructor in case if we pass the dynamic array into a function call by value
    DynamicArray(const DynamicArray& other) : number_of_elements(other.number_of_elements)
    {
        if (number_of_elements == 0) 
            elements = nullptr;
        else 
        {
            elements = std::make_unique<T[]>(number_of_elements);
            for (size_t i = 0; i < number_of_elements; ++i) 
                elements[i] = other.elements[i];
        }
    }

    // Preexisting objects
    DynamicArray& operator=(const DynamicArray& other)
    {
        if (this == &other)
            return *this;

        number_of_elements = other.number_of_elements;

        if (number_of_elements == 0)
        {
            elements.reset();
        }
        else
        {
            std::unique_ptr<T[]> new_elements = std::make_unique<T[]>(number_of_elements);
            for (size_t i = 0; i < number_of_elements; ++i)
                new_elements[i] = other.elements[i];

            elements = std::move(new_elements);   // commit only after success
        }

        return *this;
    }

    // Move semantics
    DynamicArray& operator=(DynamicArray&& other) noexcept
    {
        if (this != &other)
        {
            elements = std::move(other.elements);
            number_of_elements = other.number_of_elements;
            other.number_of_elements = 0;
        }
        return *this;
    }

    T& operator[] (size_t index)
    {
        return elements[index];
    }
};

struct ModelConfigs 
{
    // For the model
    size_t number_of_bootstrapped_samples_per_tree;
    size_t number_of_trees;

    // For each tree
    size_t maximum_depth_level;
    size_t minimum_sample_split;
    size_t maximum_leaf_nodes;
    double minimum_entropy;

    // Simple default constructor to set each value to the corresponding attribute
    ModelConfigs(size_t number_of_boostrapped_samples_per_tree, size_t number_of_trees, 
        size_t maximum_depth_level, size_t minimum_sample_split, size_t maximum_leaf_nodes, 
        double minimum_entropy) : 
        number_of_bootstrapped_samples_per_tree(number_of_boostrapped_samples_per_tree), number_of_trees(number_of_trees), maximum_depth_level(maximum_depth_level), minimum_sample_split(minimum_sample_split), maximum_leaf_nodes(maximum_leaf_nodes), minimum_entropy(minimum_entropy) {}

    // Copy constructor
    ModelConfigs(const ModelConfigs& other)
        : number_of_bootstrapped_samples_per_tree(other.number_of_bootstrapped_samples_per_tree),
          number_of_trees(other.number_of_trees),
          maximum_depth_level(other.maximum_depth_level),
          minimum_sample_split(other.minimum_sample_split),
          maximum_leaf_nodes(other.maximum_leaf_nodes),
          minimum_entropy(other.minimum_entropy)
    {}
};