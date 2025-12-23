#pragma once

template<class T>
struct DynamicArray
{
    std::unique_ptr<T[]> elements;
    size_t number_of_elements;

    DynamicArray() : elements(nullptr), number_of_elements(0) { }

    explicit DynamicArray(size_t count) : elements(std::make_unique<T[]>(count)), number_of_elements(count) {}

};

struct DataSamplesArray
{
    // Flattened 2D array where the ith feature of the nth sample can be accessed via sample_data[n * number_of_features + i]
    std::unique_ptr<double[]> sample_data;
    size_t number_of_samples;
    size_t number_of_features;

    DataSamplesArray() : sample_data(nullptr), number_of_samples(0), number_of_features(0) { }

    explicit DataSamplesArray(size_t n_of_samples, size_t n_of_features) : 
        sample_data(std::make_unique<double[]>(n_of_samples * n_of_features)),
        number_of_samples(n_of_samples),
        number_of_features(n_of_features) 
        {}
};
