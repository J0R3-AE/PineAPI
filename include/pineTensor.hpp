#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>

namespace pine
{

template<typename T>
class pineTensor
{
public:

    // Constructors
    pineTensor();
    pineTensor(std::size_t rows, std::size_t columns, std::size_t depth);
    pineTensor(std::size_t rows, std::size_t columns, std::size_t depth, const T& value);
    pineTensor(std::initializer_list<std::initializer_list<std::initializer_list<T>>> values);

    pineTensor(const pineTensor& other);
    ~pineTensor();

    // Information
    std::size_t rows() const;
    std::size_t columns() const;
    std::size_t depth() const;
    std::size_t size() const;

    bool empty() const;
    
    // Data access
    T* data();
    const T* data() const;

    // Element access
    T& at(std::size_t row, std::size_t column, std::size_t depthIndex);
    T& operator()(std::size_t row, std::size_t column, std::size_t depthIndex);
    const T& at(std::size_t row, std::size_t column, std::size_t depthIndex) const;
    const T& operator()(std::size_t row, std::size_t column, std::size_t depthIndex) const;

    // Memory management
    void resize(std::size_t rows, std::size_t columns, std::size_t depth);
    void clear();

    void fill(const T& value);

    // Arithmetic operations
    pineTensor operator+(const pineTensor& other) const;
    pineTensor operator-(const pineTensor& other) const;
    pineTensor operator*(const pineTensor& other) const;
    pineTensor operator/(const pineTensor& other) const;

    // In-place arithmetic operations
    pineTensor& operator+=(const pineTensor& other);
    pineTensor& operator-=(const pineTensor& other);
    pineTensor& operator*=(const pineTensor& other);
    pineTensor& operator/=(const pineTensor& other);

    // Copy assignment operator
    pineTensor& operator=(const pineTensor& other);

private:
    std::size_t m_rows;
    std::size_t m_columns;
    std::size_t m_depth;
    std::size_t m_size;
    std::unique_ptr<T[]> m_data;

    std::size_t index(std::size_t row, std::size_t column, std::size_t depthIndex) const;
};

} // namespace pine

#include "pineTensor.tpp"
