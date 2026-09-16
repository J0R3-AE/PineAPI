#pragma once

#include <algorithm>
#include <stdexcept>
#include <utility>

namespace pine
{

template<typename T>
pineVector<T>::pineVector() : m_size(0), m_data(nullptr)
{
}

template<typename T>
pineVector<T>::pineVector(std::size_t size) : m_size(size), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
    }
}

template<typename T>
pineVector<T>::pineVector(std::size_t size, const T& value) : m_size(size), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
        fill(value);
    }
}

template<typename T>
pineVector<T>::pineVector(std::initializer_list<T> values) : m_size(values.size()), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
        std::size_t index = 0;
        for (const T& value : values) {
            m_data[index++] = value;
        }
    }
}

template<typename T>
pineVector<T>::pineVector(const pineVector& other) : m_size(other.m_size), m_data(nullptr)
{
    if (m_size > 0) {
        m_data = std::make_unique<T[]>(m_size);
        std::copy(other.m_data.get(), other.m_data.get() + m_size, m_data.get());
    }
}

template<typename T>
pineVector<T>& pineVector<T>::operator=(const pineVector& other)
{
    if (this != &other) {
        pineVector copy(other);
        std::swap(m_size, copy.m_size);
        std::swap(m_data, copy.m_data);
    }
    return *this;
}

template<typename T>
pineVector<T>::~pineVector() = default;

template<typename T>
std::size_t pineVector<T>::size() const
{
    return m_size;
}

template<typename T>
bool pineVector<T>::empty() const
{
    return m_size == 0;
}

template<typename T>
T* pineVector<T>::data()
{
    return m_data.get();
}

template<typename T>
const T* pineVector<T>::data() const
{
    return m_data.get();
}

template<typename T>
T& pineVector<T>::at(std::size_t index)
{
    if (index >= m_size) {
        throw std::out_of_range("Vector index out of range.");
    }
    return m_data[index];
}

template<typename T>
const T& pineVector<T>::at(std::size_t index) const
{
    if (index >= m_size) {
        throw std::out_of_range("Vector index out of range.");
    }
    return m_data[index];
}

template<typename T>
T& pineVector<T>::operator[](std::size_t index)
{
    return at(index);
}

template<typename T>
const T& pineVector<T>::operator[](std::size_t index) const
{
    return at(index);
}

template<typename T>
void pineVector<T>::resize(std::size_t size)
{
    std::unique_ptr<T[]> newData = nullptr;
    if (size > 0) {
        newData = std::make_unique<T[]>(size);
        std::size_t copyCount = std::min(m_size, size);
        for (std::size_t i = 0; i < copyCount; ++i) {
            newData[i] = m_data[i];
        }
    }

    m_size = size;
    m_data = std::move(newData);
}

template<typename T>
void pineVector<T>::clear()
{
    m_size = 0;
    m_data.reset();
}

template<typename T>
void pineVector<T>::fill(const T& value)
{
    if (m_data) {
        std::fill_n(m_data.get(), m_size, value);
    }
}

template<typename T>
pineVector<T> pineVector<T>::operator+(const pineVector& other) const
{
    if (m_size != other.m_size) {
        throw std::invalid_argument("Vector sizes must match for addition.");
    }

    pineVector result(m_size);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] + rhs[i];
    }
    return result;
}

template<typename T>
pineVector<T> pineVector<T>::operator-(const pineVector& other) const
{
    if (m_size != other.m_size) {
        throw std::invalid_argument("Vector sizes must match for subtraction.");
    }

    pineVector result(m_size);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] - rhs[i];
    }
    return result;
}

template<typename T>
pineVector<T> pineVector<T>::operator*(const pineVector& other) const
{
    if (m_size != other.m_size) {
        throw std::invalid_argument("Vector sizes must match for element-wise multiplication.");
    }

    pineVector result(m_size);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        out[i] = lhs[i] * rhs[i];
    }
    return result;
}

template<typename T>
pineVector<T> pineVector<T>::operator/(const pineVector& other) const
{
    if (m_size != other.m_size) {
        throw std::invalid_argument("Vector sizes must match for division.");
    }

    pineVector result(m_size);
    const T* lhs = m_data.get();
    const T* rhs = other.m_data.get();
    T* out = result.m_data.get();

    for (std::size_t i = 0; i < m_size; ++i) {
        if (rhs[i] == T{}) {
            throw std::domain_error("Division by zero in vector element.");
        }
        out[i] = lhs[i] / rhs[i];
    }
    return result;
}

template<typename T>
pineVector<T>& pineVector<T>::operator+=(const pineVector& other)
{
    if (m_size != other.m_size) {
        throw std::invalid_argument("Vector sizes must match for addition.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] += rhs[i];
    }
    return *this;
}

template<typename T>
pineVector<T>& pineVector<T>::operator-=(const pineVector& other)
{
    if (m_size != other.m_size) {
        throw std::invalid_argument("Vector sizes must match for subtraction.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] -= rhs[i];
    }
    return *this;
}

template<typename T>
pineVector<T>& pineVector<T>::operator*=(const pineVector& other)
{
    if (m_size != other.m_size) {
        throw std::invalid_argument("Vector sizes must match for multiplication.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        data[i] *= rhs[i];
    }
    return *this;
}

template<typename T>
pineVector<T>& pineVector<T>::operator/=(const pineVector& other)
{
    if (m_size != other.m_size) {
        throw std::invalid_argument("Vector sizes must match for division.");
    }

    T* data = m_data.get();
    const T* rhs = other.m_data.get();
    for (std::size_t i = 0; i < m_size; ++i) {
        if (rhs[i] == T{}) {
            throw std::domain_error("Division by zero in vector element.");
        }
        data[i] /= rhs[i];
    }
    return *this;
}

} // namespace pine
