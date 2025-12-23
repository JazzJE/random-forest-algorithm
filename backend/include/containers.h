#pragma once
#include <memory>
#include <cstddef>

template<class T>
struct DynamicArray
{
    std::unique_ptr<T[]> elements;
    size_t number_of_elements;

    DynamicArray() : elements(nullptr), number_of_elements(0) { }

    explicit DynamicArray(size_t count) : elements(std::make_unique<T[]>(count)), number_of_elements(count) {}

};
