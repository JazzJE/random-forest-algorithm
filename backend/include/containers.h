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
};