#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>

namespace pine
{

template<typename T>
class pineVector
{
public:

    // Constructors
    pineVector();
    pineVector(std::size_t size, const T& value);
    pineVector(std::initializer_list<T> values);

    explicit pineVector(std::size_t size);
    
    pineVector(const pineVector& other);
    ~pineVector();

    std::size_t size() const;
    bool empty() const;

    // Data access
    T* data();
    const T* data() const;

    // Element access
    T& at(std::size_t index);
    T& operator[](std::size_t index);
    const T& at(std::size_t index) const;
    const T& operator[](std::size_t index) const;

    // Memory management
    void resize(std::size_t size);
    void clear();

    // Fill the vector with a specific value
    void fill(const T& value);

    // Arithmetic operations
    pineVector operator+(const pineVector& other) const;
    pineVector operator-(const pineVector& other) const;
    pineVector operator*(const pineVector& other) const;
    pineVector operator/(const pineVector& other) const;

    // In-place arithmetic operations
    pineVector& operator+=(const pineVector& other);
    pineVector& operator-=(const pineVector& other);
    pineVector& operator*=(const pineVector& other);
    pineVector& operator/=(const pineVector& other);

    // Copy assignment operator
    pineVector& operator=(const pineVector& other);

private:
    std::size_t m_size;
    std::unique_ptr<T[]> m_data;
};

} // namespace pine

#include "pineVector.tpp"
