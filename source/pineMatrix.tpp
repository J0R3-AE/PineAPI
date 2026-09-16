#pragma once

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace pine
{

template<typename T> pineMatrix<T>::pineMatrix() : m_rows(0), m_columns(0), m_size(0), m_data(nullptr){}

template<typename T>
pineMatrix<T>::pineMatrix(std::size_t rows, std::size_t columns)
    : m_rows(rows), m_columns(columns), m_size(rows * columns), m_data(nullptr)
{
    if (m_size > 0) {m_data = std::make_unique<T[]>(m_size);}
}

template<typename T>
pineMatrix<T>::pineMatrix(std::size_t rows, std::size_t columns, const T& value)
    : m_rows(rows), m_columns(columns), m_size(rows * columns), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
        fill(value);
    }
}

template<typename T>
pineMatrix<T>::pineMatrix(std::size_t rows, std::size_t columns, std::initializer_list<T> values)
    : m_rows(rows), m_columns(columns), m_size(rows * columns), m_data(nullptr)
{
    if (rows == 0 || columns == 0) {
        if (values.size() != 0) {
            throw std::invalid_argument("Matrix dimensions and value count do not match.");
        }
        return;
    }

    if (values.size() != m_size) {
        throw std::invalid_argument("Matrix dimensions and value count do not match.");
    }

    m_data = std::make_unique<T[]>(m_size);
    std::size_t index = 0;
    for (const T& value : values) {
        m_data[index++] = value;
    }
}

template<typename T>
pineMatrix<T>::pineMatrix(std::initializer_list<std::initializer_list<T>> values)
    : m_rows(0), m_columns(0), m_size(0), m_data(nullptr)
{
    if (values.size() == 0) {return;}

    m_rows = values.size();
    m_columns = values.begin()->size();

    for (const auto& row : values) {
        if (row.size() != m_columns) {throw std::invalid_argument("All rows must have the same size.");}
    }

    m_size = m_rows * m_columns;

    if (m_size == 0) {return;}

    m_data = std::make_unique<T[]>(m_size);

    std::size_t rowIndex = 0;
    for (const auto& row : values) {
        std::size_t columnIndex = 0;
        for (const T& value : row) {
            (*this)(rowIndex, columnIndex) = value;
            ++columnIndex;
        }
        ++rowIndex;
    }
}

template<typename T>
pineMatrix<T>::pineMatrix(const pineMatrix& other)
    : m_rows(other.m_rows), m_columns(other.m_columns), m_size(other.m_size), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
        std::copy(other.m_data.get(), other.m_data.get() + m_size, m_data.get());
    }
}

template<typename T>
pineMatrix<T>& pineMatrix<T>::operator=(const pineMatrix& other)
{
    if (this != &other) {
        pineMatrix copy(other);
        std::swap(m_rows, copy.m_rows);
        std::swap(m_columns, copy.m_columns);
        std::swap(m_size, copy.m_size);
        std::swap(m_data, copy.m_data);
    }
    return *this;
}

template<typename T> pineMatrix<T>::~pineMatrix() = default;
template<typename T> std::size_t pineMatrix<T>::rows() const { return m_rows; }
template<typename T> std::size_t pineMatrix<T>::columns() const { return m_columns; }
template<typename T> std::size_t pineMatrix<T>::size() const { return m_size; }
template<typename T> bool pineMatrix<T>::empty() const { return m_size == 0; }
template<typename T> T* pineMatrix<T>::data() { return m_data.get(); }
template<typename T> const T* pineMatrix<T>::data() const { return m_data.get(); }

template<typename T>
std::size_t pineMatrix<T>::index(std::size_t row, std::size_t column) const
{
    if (row >= m_rows || column >= m_columns) {
        throw std::out_of_range("Matrix index out of range.");
    }
    return row * m_columns + column;
}

template<typename T> T& pineMatrix<T>::at(std::size_t row, std::size_t column) { return m_data[index(row, column)]; }
template<typename T> T& pineMatrix<T>::operator()(std::size_t row, std::size_t column) { return at(row, column); }
template<typename T> const T& pineMatrix<T>::at(std::size_t row, std::size_t column) const { return m_data[index(row, column)]; }
template<typename T> const T& pineMatrix<T>::operator()(std::size_t row, std::size_t column) const { return at(row, column); }

template<typename T>
void pineMatrix<T>::resize(std::size_t rows, std::size_t columns)
{
    if (rows == 0 || columns == 0) {
        clear();
        return;
    }

    std::size_t newSize = rows * columns;
    std::unique_ptr<T[]> newData = std::make_unique<T[]>(newSize);

    std::size_t copyCount = std::min(m_size, newSize);
    for (std::size_t i = 0; i < copyCount; ++i) {
        newData[i] = m_data[i];
    }

    m_rows = rows;
    m_columns = columns;
    m_size = newSize;
    m_data = std::move(newData);
}

template<typename T>
void pineMatrix<T>::clear()
{
    m_rows = 0;
    m_columns = 0;
    m_size = 0;
    m_data.reset();
}

template<typename T>
void pineMatrix<T>::fill(const T& value)
{
    if (m_data) {
        std::fill_n(m_data.get(), m_size, value);
    }
}

template<typename T>
pineMatrix<T> pineMatrix<T>::transpose() const
{
    if (empty()) {
        return pineMatrix<T>();
    }

    pineMatrix<T> result(m_columns, m_rows);
    const T* src = m_data.get();
    T* dst = result.m_data.get();

    for (std::size_t row = 0; row < m_rows; ++row) {
        const std::size_t srcOffset = row * m_columns;
        for (std::size_t column = 0; column < m_columns; ++column) {
            dst[column * m_rows + row] = src[srcOffset + column];
        }
    }
    return result;
}

template<typename T>
pineMatrix<T> pineMatrix<T>::matmul(const pineMatrix& other) const
{
    if (m_columns != other.m_rows) {
        throw std::invalid_argument("Matrix dimensions do not align for multiplication.");
    }

    pineMatrix<T> result(m_rows, other.m_columns, T{});
    for (std::size_t row = 0; row < m_rows; ++row) {
        for (std::size_t column = 0; column < other.m_columns; ++column) {
            T sum = T{};
            for (std::size_t inner = 0; inner < m_columns; ++inner) {
                sum += m_data[row * m_columns + inner] * other.m_data[inner * other.m_columns + column];
            }
            result.m_data[row * other.m_columns + column] = sum;
        }
    }
    return result;
}

template<typename T>
pineMatrix<T> pineMatrix<T>::operator+(const pineMatrix& other) const
{
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::invalid_argument("Matrix dimensions must match for addition.");
    }

    pineMatrix<T> result(m_rows, m_columns);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] + rhs[i];
    }
    return result;
}

template<typename T>
pineMatrix<T> pineMatrix<T>::operator-(const pineMatrix& other) const
{
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction.");
    }

    pineMatrix<T> result(m_rows, m_columns);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] - rhs[i];
    }
    return result;
}

template<typename T>
pineMatrix<T> pineMatrix<T>::operator*(const pineMatrix& other) const
{
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::invalid_argument("Matrix dimensions must match for element-wise multiplication.");
    }

    pineMatrix<T> result(m_rows, m_columns);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] * rhs[i];
    }
    return result;
}

template<typename T>
pineMatrix<T> pineMatrix<T>::operator/(const pineMatrix& other) const
{
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::invalid_argument("Matrix dimensions must match for division.");
    }

    pineMatrix<T> result(m_rows, m_columns);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        if (rhs[i] == T{}) {
            throw std::domain_error("Division by zero in matrix element.");
        }
        out[i] = lhs[i] / rhs[i];
    }
    return result;
}

template<typename T>
pineMatrix<T>& pineMatrix<T>::operator+=(const pineMatrix& other)
{
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::invalid_argument("Matrix dimensions must match for addition.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] += rhs[i];
    }
    return *this;
}

template<typename T>
pineMatrix<T>& pineMatrix<T>::operator-=(const pineMatrix& other)
{
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::invalid_argument("Matrix dimensions must match for subtraction.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] -= rhs[i];
    }
    return *this;
}

template<typename T>
pineMatrix<T>& pineMatrix<T>::operator*=(const pineMatrix& other)
{
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::invalid_argument("Matrix dimensions must match for multiplication.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] *= rhs[i];
    }
    return *this;
}

template<typename T>
pineMatrix<T>& pineMatrix<T>::operator/=(const pineMatrix& other)
{
    if (m_rows != other.m_rows || m_columns != other.m_columns) {
        throw std::invalid_argument("Matrix dimensions must match for division.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        if (rhs[i] == T{}) {
            throw std::domain_error("Division by zero in matrix element.");
        }
        data[i] /= rhs[i];
    }
    return *this;
}

} // namespace pine
