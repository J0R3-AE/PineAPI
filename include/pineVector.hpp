#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>

namespace pine
{

/**
 * @class pineVector
 * @brief Dense vector container for arithmetic and element access.
 *
 * The pineVector type stores a contiguous sequence of values of type T and
 * supports resizing, indexing, and arithmetic operators.
 *
 * @tparam T The element type stored in the vector.
 */
template<typename T>
class pineVector
{
public:

    /**
     * @brief Default constructor.
     *
     * Creates an empty vector.
     */
    pineVector();

    /**
     * @brief Constructs a vector with a fixed size and initial value.
     * @param size Number of elements.
     * @param value Value assigned to all elements.
     */
    pineVector(std::size_t size, const T& value);

    /**
     * @brief Constructs a vector from an initializer list.
     * @param values Initial values in order.
     */
    pineVector(std::initializer_list<T> values);

    /**
     * @brief Constructs a vector with a given size and default-initialized elements.
     * @param size Number of elements.
     */
    explicit pineVector(std::size_t size);

    /**
     * @brief Copy constructor.
     * @param other Vector to copy from.
     */
    pineVector(const pineVector& other);

    /**
     * @brief Destructor.
     */
    ~pineVector();

    /**
     * @brief Returns the current number of elements.
     * @return Vector size.
     */
    std::size_t size() const;

    /**
     * @brief Checks whether the vector is empty.
     * @return True if size is zero, otherwise false.
     */
    bool empty() const;

    /**
     * @brief Returns a pointer to the underlying data buffer.
     * @return Mutable pointer to element storage.
     */
    T* data();

    /**
     * @brief Returns a const pointer to the underlying data buffer.
     * @return Read-only pointer to element storage.
     */
    const T* data() const;

    /**
     * @brief Bounds-checked access to an element.
     * @param index Element index.
     * @return Reference to the indexed element.
     * @throw std::out_of_range If index is out of range.
     */
    T& at(std::size_t index);

    /**
     * @brief Accesses an element by index.
     * @param index Element index.
     * @return Reference to the indexed element.
     */
    T& operator[](std::size_t index);

    /**
     * @brief Bounds-checked read-only access to an element.
     * @param index Element index.
     * @return Const reference to the indexed element.
     * @throw std::out_of_range If index is out of range.
     */
    const T& at(std::size_t index) const;

    /**
     * @brief Read-only access to an element by index.
     * @param index Element index.
     * @return Const reference to the indexed element.
     */
    const T& operator[](std::size_t index) const;

    /**
     * @brief Resizes the vector and preserves existing elements up to the new size.
     * @param size New vector size.
     */
    void resize(std::size_t size);

    /**
     * @brief Removes all elements and clears storage.
     */
    void clear();

    /**
     * @brief Fills the vector with a single value.
     * @param value Value to assign to all elements.
     */
    void fill(const T& value);

    /**
     * @brief Adds two vectors element-wise.
     * @param other Vector to add.
     * @return Resulting vector.
     * @throw std::invalid_argument If vector sizes differ.
     */
    pineVector operator+(const pineVector& other) const;

    /**
     * @brief Subtracts two vectors element-wise.
     * @param other Vector to subtract.
     * @return Resulting vector.
     * @throw std::invalid_argument If vector sizes differ.
     */
    pineVector operator-(const pineVector& other) const;

    /**
     * @brief Multiplies two vectors element-wise.
     * @param other Vector to multiply by.
     * @return Resulting vector.
     * @throw std::invalid_argument If vector sizes differ.
     */
    pineVector operator*(const pineVector& other) const;

    /**
     * @brief Divides two vectors element-wise.
     * @param other Vector containing divisors.
     * @return Resulting vector.
     * @throw std::invalid_argument If vector sizes differ.
     * @throw std::domain_error If any divisor is zero.
     */
    pineVector operator/(const pineVector& other) const;

    /**
     * @brief Adds another vector in place.
     * @param other Vector to add.
     * @return Reference to this vector.
     * @throw std::invalid_argument If vector sizes differ.
     */
    pineVector& operator+=(const pineVector& other);

    /**
     * @brief Subtracts another vector in place.
     * @param other Vector to subtract.
     * @return Reference to this vector.
     * @throw std::invalid_argument If vector sizes differ.
     */
    pineVector& operator-=(const pineVector& other);

    /**
     * @brief Multiplies another vector in place.
     * @param other Vector to multiply by.
     * @return Reference to this vector.
     * @throw std::invalid_argument If vector sizes differ.
     */
    pineVector& operator*=(const pineVector& other);

    /**
     * @brief Divides this vector by another in place.
     * @param other Vector containing divisors.
     * @return Reference to this vector.
     * @throw std::invalid_argument If vector sizes differ.
     * @throw std::domain_error If any divisor is zero.
     */
    pineVector& operator/=(const pineVector& other);

    /**
     * @brief Copy assignment operator.
     * @param other Vector to assign from.
     * @return Reference to this vector.
     */
    pineVector& operator=(const pineVector& other);

private:
    std::size_t m_size;     ///< Number of stored elements.
    std::unique_ptr<T[]> m_data; ///< Internal storage buffer.
};

} // namespace pine

#include "pineVector.tpp"
