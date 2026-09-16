#pragma once

#include <cstddef>
#include <initializer_list>
#include <memory>

namespace pine
{

/**
 * @class pineTensor
 * @brief Dense tensor container with row, column, and depth indexing.
 *
 * A pineTensor stores a 3D block of values with contiguous memory and supports
 * element-wise arithmetic and direct indexed access.
 *
 * @tparam T The element type stored in the tensor.
 */
template<typename T>
class pineTensor
{
public:

    /**
     * @brief Default constructor.
     *
     * Creates an empty tensor.
     */
    pineTensor();

    /**
     * @brief Constructs a tensor with explicit dimensions.
     * @param rows Number of rows.
     * @param columns Number of columns.
     * @param depth Number of depth slices.
     */
    pineTensor(std::size_t rows, std::size_t columns, std::size_t depth);

    /**
     * @brief Constructs a tensor with explicit dimensions and a fill value.
     * @param rows Number of rows.
     * @param columns Number of columns.
     * @param depth Number of depth slices.
     * @param value Value assigned to all elements.
     */
    pineTensor(std::size_t rows, std::size_t columns, std::size_t depth, const T& value);

    /**
     * @brief Constructs a tensor from a nested initializer list.
     * @param values Tensor contents in row/column/depth order.
     */
    pineTensor(std::initializer_list<std::initializer_list<std::initializer_list<T>>> values);

    /**
     * @brief Copy constructor.
     * @param other Tensor to copy from.
     */
    pineTensor(const pineTensor& other);

    /**
     * @brief Destructor.
     */
    ~pineTensor();

    /**
     * @brief Returns the number of rows.
     * @return Row count.
     */
    std::size_t rows() const;

    /**
     * @brief Returns the number of columns.
     * @return Column count.
     */
    std::size_t columns() const;

    /**
     * @brief Returns the depth size.
     * @return Depth count.
     */
    std::size_t depth() const;

    /**
     * @brief Returns the total number of elements.
     * @return Element count.
     */
    std::size_t size() const;

    /**
     * @brief Checks whether the tensor is empty.
     * @return True if it has no elements.
     */
    bool empty() const;

    /**
     * @brief Returns a pointer to the underlying data buffer.
     * @return Mutable pointer to tensor storage.
     */
    T* data();

    /**
     * @brief Returns a const pointer to the underlying data buffer.
     * @return Read-only pointer to tensor storage.
     */
    const T* data() const;

    /**
     * @brief Bounds-checked access to a tensor element.
     * @param row Row index.
     * @param column Column index.
     * @param depthIndex Depth index.
     * @return Reference to the selected element.
     * @throw std::out_of_range If any index is out of range.
     */
    T& at(std::size_t row, std::size_t column, std::size_t depthIndex);

    /**
     * @brief Accesses a tensor element with index operators.
     * @param row Row index.
     * @param column Column index.
     * @param depthIndex Depth index.
     * @return Reference to the selected element.
     */
    T& operator()(std::size_t row, std::size_t column, std::size_t depthIndex);

    /**
     * @brief Bounds-checked read-only access to a tensor element.
     * @param row Row index.
     * @param column Column index.
     * @param depthIndex Depth index.
     * @return Const reference to the selected element.
     * @throw std::out_of_range If any index is out of range.
     */
    const T& at(std::size_t row, std::size_t column, std::size_t depthIndex) const;

    /**
     * @brief Read-only access to a tensor element.
     * @param row Row index.
     * @param column Column index.
     * @param depthIndex Depth index.
     * @return Const reference to the selected element.
     */
    const T& operator()(std::size_t row, std::size_t column, std::size_t depthIndex) const;

    /**
     * @brief Resizes the tensor to a new shape.
     * @param rows New row count.
     * @param columns New column count.
     * @param depth New depth count.
     */
    void resize(std::size_t rows, std::size_t columns, std::size_t depth);

    /**
     * @brief Clears the tensor and releases its storage.
     */
    void clear();

    /**
     * @brief Fills every element with the same value.
     * @param value Value used for the fill operation.
     */
    void fill(const T& value);

    /**
     * @brief Adds two tensors element-wise.
     * @param other Tensor to add.
     * @return Resulting tensor.
     * @throw std::invalid_argument If shapes differ.
     */
    pineTensor operator+(const pineTensor& other) const;

    /**
     * @brief Subtracts two tensors element-wise.
     * @param other Tensor to subtract.
     * @return Resulting tensor.
     * @throw std::invalid_argument If shapes differ.
     */
    pineTensor operator-(const pineTensor& other) const;

    /**
     * @brief Multiplies two tensors element-wise.
     * @param other Tensor to multiply by.
     * @return Resulting tensor.
     * @throw std::invalid_argument If shapes differ.
     */
    pineTensor operator*(const pineTensor& other) const;

    /**
     * @brief Divides two tensors element-wise.
     * @param other Tensor containing divisors.
     * @return Resulting tensor.
     * @throw std::invalid_argument If shapes differ.
     * @throw std::domain_error If any divisor is zero.
     */
    pineTensor operator/(const pineTensor& other) const;

    /**
     * @brief Adds another tensor in place.
     * @param other Tensor to add.
     * @return Reference to this tensor.
     * @throw std::invalid_argument If shapes differ.
     */
    pineTensor& operator+=(const pineTensor& other);

    /**
     * @brief Subtracts another tensor in place.
     * @param other Tensor to subtract.
     * @return Reference to this tensor.
     * @throw std::invalid_argument If shapes differ.
     */
    pineTensor& operator-=(const pineTensor& other);

    /**
     * @brief Multiplies another tensor in place.
     * @param other Tensor to multiply by.
     * @return Reference to this tensor.
     * @throw std::invalid_argument If shapes differ.
     */
    pineTensor& operator*=(const pineTensor& other);

    /**
     * @brief Divides this tensor by another in place.
     * @param other Tensor containing divisors.
     * @return Reference to this tensor.
     * @throw std::invalid_argument If shapes differ.
     * @throw std::domain_error If any divisor is zero.
     */
    pineTensor& operator/=(const pineTensor& other);

    /**
     * @brief Copy assignment operator.
     * @param other Tensor to assign from.
     * @return Reference to this tensor.
     */
    pineTensor& operator=(const pineTensor& other);

private:
    std::size_t m_rows;      ///< Number of rows.
    std::size_t m_columns;   ///< Number of columns.
    std::size_t m_depth;     ///< Number of depth slices.
    std::size_t m_size;      ///< Total number of elements.
    std::unique_ptr<T[]> m_data; ///< Internal storage buffer.

    /**
     * @brief Computes the flat index for a row, column, and depth value.
     * @param row Row index.
     * @param column Column index.
     * @param depthIndex Depth index.
     * @return Flat offset into the storage array.
     */
    std::size_t index(std::size_t row, std::size_t column, std::size_t depthIndex) const;
};

} // namespace pine

#include "pineTensor.tpp"
