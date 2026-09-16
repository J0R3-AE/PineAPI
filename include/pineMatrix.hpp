/**
 * @file pineMatrix.hpp
 * @brief A simple matrix class for handling basic matrix operations.
 * @author Pine
 * @version 1.0
 * @copyright Copyright (c) 2024 Pine. All rights reserved.
 * @date 2024-06-15
 */

#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>

namespace pine
{

enum class pineDataType
{
    Float32,
    Float64,
    Int32,
    Int64,
    UInt32,
    UInt64
};

template<typename T>

class pineMatrix
{
public:

    // Constructors
    pineMatrix();
    pineMatrix(std::size_t rows, std::size_t columns);
    pineMatrix(std::size_t rows, std::size_t columns, const T& value);
    pineMatrix(std::size_t rows, std::size_t columns, std::initializer_list<T> values);
    pineMatrix(std::initializer_list<std::initializer_list<T>> values);

    // Copy constructor / assignment
    pineMatrix(const pineMatrix& other);
    ~pineMatrix();

    //information
    std::size_t rows() const;
    std::size_t columns() const;
    std::size_t size() const;

    bool empty() const;

    // Data access
    T* data();
    const T* data() const;

    //Element access
    T& at (std::size_t row, std::size_t column);
    T& operator()(std::size_t row, std::size_t column);
    const T& at (std::size_t row, std::size_t column) const;
    const T& operator()(std::size_t row, std::size_t column) const;

    //Memory
    void resize(std::size_t rows, std::size_t columns);
    void clear();

    //Fill
    void fill(const T& value);

    //Matrix operations
    pineMatrix transpose() const;
    pineMatrix matmul(const pineMatrix& other) const;

    //Arithmetic operations
    pineMatrix operator+(const pineMatrix& other) const;
    pineMatrix operator-(const pineMatrix& other) const;
    pineMatrix operator*(const pineMatrix& other) const;
    pineMatrix operator/(const pineMatrix& other) const;

    //in-place arithmetic operations
    pineMatrix& operator+=(const pineMatrix& other);
    pineMatrix& operator-=(const pineMatrix& other);
    pineMatrix& operator*=(const pineMatrix& other);
    pineMatrix& operator/=(const pineMatrix& other);

    // Copy assignment operator
    pineMatrix& operator=(const pineMatrix& other);

private:
    std::size_t m_rows;
    std::size_t m_columns;
    std::size_t m_size;
    std::unique_ptr<T[]> m_data;

    std::size_t index(std::size_t row, std::size_t column) const;
};

} // namespace pine

#include "pineMatrix.tpp"