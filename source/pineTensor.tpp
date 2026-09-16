#pragma once

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace pine
{

template<typename T> pineTensor<T>::pineTensor() : m_rows(0), m_columns(0), m_depth(0), m_size(0), m_data(nullptr){}

template<typename T>
pineTensor<T>::pineTensor(std::size_t rows, std::size_t columns, std::size_t depth)
    : m_rows(rows), m_columns(columns), m_depth(depth), m_size(rows * columns * depth), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
    }
}

template<typename T>
pineTensor<T>::pineTensor(std::size_t rows, std::size_t columns, std::size_t depth, const T& value)
    : m_rows(rows), m_columns(columns), m_depth(depth), m_size(rows * columns * depth), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
        fill(value);
    }
}

template<typename T>
pineTensor<T>::pineTensor(std::initializer_list<std::initializer_list<std::initializer_list<T>>> values)
    : m_rows(0), m_columns(0), m_depth(0), m_size(0), m_data(nullptr)
{
    if (values.size() == 0) {
        return;
    }

    m_rows = values.size();
    auto firstRow = values.begin();
    m_columns = firstRow->size();
    if (m_columns == 0) {
        return;
    }

    auto firstColumn = firstRow->begin();
    m_depth = firstColumn->size();

    for (const auto& row : values) {
        if (row.size() != m_columns) {
            throw std::invalid_argument("All tensor rows must have the same number of columns.");
        }
        for (const auto& column : row) {
            if (column.size() != m_depth) {
                throw std::invalid_argument("All tensor columns must have the same depth.");
            }
        }
    }

    m_size = m_rows * m_columns * m_depth;
    if (m_size == 0) {
        return;
    }

    m_data = std::make_unique<T[]>(m_size);
    std::size_t pos = 0;
    for (const auto& row : values) {
        for (const auto& column : row) {
            for (const T& value : column) {
                m_data[pos++] = value;
            }
        }
    }
}

template<typename T>
pineTensor<T>::pineTensor(const pineTensor& other)
    : m_rows(other.m_rows), m_columns(other.m_columns), m_depth(other.m_depth), m_size(other.m_size), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
        std::copy(other.m_data.get(), other.m_data.get() + m_size, m_data.get());
    }
}

template<typename T>
pineTensor<T>& pineTensor<T>::operator=(const pineTensor& other)
{
    if (this != &other) {
        pineTensor copy(other);
        std::swap(m_rows, copy.m_rows);
        std::swap(m_columns, copy.m_columns);
        std::swap(m_depth, copy.m_depth);
        std::swap(m_size, copy.m_size);
        std::swap(m_data, copy.m_data);
    }
    return *this;
}

template<typename T> pineTensor<T>::~pineTensor() = default;
template<typename T> std::size_t pineTensor<T>::rows() const { return m_rows; }
template<typename T> std::size_t pineTensor<T>::columns() const { return m_columns; }
template<typename T> std::size_t pineTensor<T>::depth() const { return m_depth; }
template<typename T> std::size_t pineTensor<T>::size() const { return m_size; }
template<typename T> bool pineTensor<T>::empty() const { return m_size == 0; }
template<typename T> T* pineTensor<T>::data() { return m_data.get(); }
template<typename T> const T* pineTensor<T>::data() const { return m_data.get(); }

template<typename T>
std::size_t pineTensor<T>::index(std::size_t row, std::size_t column, std::size_t depthIndex) const
{
    if (row >= m_rows || column >= m_columns || depthIndex >= m_depth) {
        throw std::out_of_range("Tensor index out of range.");
    }
    return ((row * m_columns + column) * m_depth) + depthIndex;
}

template<typename T>
T& pineTensor<T>::at(std::size_t row, std::size_t column, std::size_t depthIndex)
{
    return m_data[index(row, column, depthIndex)];
}

template<typename T>
const T& pineTensor<T>::at(std::size_t row, std::size_t column, std::size_t depthIndex) const
{
    return m_data[index(row, column, depthIndex)];
}

template<typename T>
T& pineTensor<T>::operator()(std::size_t row, std::size_t column, std::size_t depthIndex)
{
    return at(row, column, depthIndex);
}

template<typename T>
const T& pineTensor<T>::operator()(std::size_t row, std::size_t column, std::size_t depthIndex) const
{
    return at(row, column, depthIndex);
}

template<typename T>
void pineTensor<T>::resize(std::size_t rows, std::size_t columns, std::size_t depth)
{
    std::size_t newSize = rows * columns * depth;
    std::unique_ptr<T[]> newData = nullptr;
    if (newSize > 0) {
        newData = std::make_unique<T[]>(newSize);
        std::size_t copyCount = std::min(m_size, newSize);
        for (std::size_t i = 0; i < copyCount; ++i) {
            newData[i] = m_data[i];
        }
    }

    m_rows = rows;
    m_columns = columns;
    m_depth = depth;
    m_size = newSize;
    m_data = std::move(newData);
}

template<typename T>
void pineTensor<T>::clear()
{
    m_rows = 0;
    m_columns = 0;
    m_depth = 0;
    m_size = 0;
    m_data.reset();
}

template<typename T>
void pineTensor<T>::fill(const T& value)
{
    if (m_data) {
        std::fill_n(m_data.get(), m_size, value);
    }
}

template<typename T>
pineTensor<T> pineTensor<T>::operator+(const pineTensor& other) const
{
    if (m_rows != other.m_rows || m_columns != other.m_columns || m_depth != other.m_depth) {
        throw std::invalid_argument("Tensor dimensions must match for addition.");
    }

    pineTensor<T> result(m_rows, m_columns, m_depth);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] + rhs[i];
    }
    return result;
}

template<typename T>
pineTensor<T> pineTensor<T>::operator-(const pineTensor& other) const
{
    if (m_rows != other.m_rows || m_columns != other.m_columns || m_depth != other.m_depth) {
        throw std::invalid_argument("Tensor dimensions must match for subtraction.");
    }

    pineTensor<T> result(m_rows, m_columns, m_depth);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] - rhs[i];
    }
    return result;
}

template<typename T>
pineTensor<T> pineTensor<T>::operator*(const pineTensor& other) const
{
    if (m_rows != other.m_rows || m_columns != other.m_columns || m_depth != other.m_depth) {
        throw std::invalid_argument("Tensor dimensions must match for element-wise multiplication.");
    }

    pineTensor<T> result(m_rows, m_columns, m_depth);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] * rhs[i];
    }
    return result;
}

template<typename T>
pineTensor<T> pineTensor<T>::operator/(const pineTensor& other) const
{
    if (m_rows != other.m_rows || m_columns != other.m_columns || m_depth != other.m_depth) {
        throw std::invalid_argument("Tensor dimensions must match for division.");
    }

    pineTensor<T> result(m_rows, m_columns, m_depth);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        if (rhs[i] == T{}) {
            throw std::domain_error("Division by zero in tensor element.");
        }
        out[i] = lhs[i] / rhs[i];
    }
    return result;
}

template<typename T>
pineTensor<T>& pineTensor<T>::operator+=(const pineTensor& other)
{
    if (m_rows != other.m_rows || m_columns != other.m_columns || m_depth != other.m_depth) {
        throw std::invalid_argument("Tensor dimensions must match for addition.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] += rhs[i];
    }
    return *this;
}

template<typename T>
pineTensor<T>& pineTensor<T>::operator-=(const pineTensor& other)
{
    if (m_rows != other.m_rows || m_columns != other.m_columns || m_depth != other.m_depth) {
        throw std::invalid_argument("Tensor dimensions must match for subtraction.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] -= rhs[i];
    }
    return *this;
}

template<typename T>
pineTensor<T>& pineTensor<T>::operator*=(const pineTensor& other)
{
    if (m_rows != other.m_rows || m_columns != other.m_columns || m_depth != other.m_depth) {
        throw std::invalid_argument("Tensor dimensions must match for multiplication.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] *= rhs[i];
    }
    return *this;
}

template<typename T>
pineTensor<T>& pineTensor<T>::operator/=(const pineTensor& other)
{
    if (m_rows != other.m_rows || m_columns != other.m_columns || m_depth != other.m_depth) {
        throw std::invalid_argument("Tensor dimensions must match for division.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        if (rhs[i] == T{}) {
            throw std::domain_error("Division by zero in tensor element.");
        }
        data[i] /= rhs[i];
    }
    return *this;
}

} // namespace pine
