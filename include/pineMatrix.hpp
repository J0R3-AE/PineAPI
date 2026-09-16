/**
 * @file pineMatrix.hpp
 * @brief Dense matrix container and core linear algebra operations.
 * @author J0R3-AE
 * @version 1.0
 * @copyright Copyright (c) 2026 J0R3-AE
 * @date 2026-09-16
 */

#pragma once

#include <cstddef>
#include <memory>
#include <initializer_list>

namespace pine
{

/**
 * @brief Represents the logical data type used by the matrix implementation.
 */
enum class pineDataType
{
    Float32,
    Float64,
    Int32,
    Int64,
    UInt32,
    UInt64
};

/**
 * @class pineMatrix
 * @brief Dense row-major matrix type with arithmetic and multiplication support.
 *
 * This class stores a 2D matrix in contiguous memory and supports common
 * operations such as element-wise arithmetic, resizing, transpose, and matrix
 * multiplication.
 *
 * @tparam T The element type stored within the matrix.
 */
template<typename T>
class pineMatrix
{
public:

    /**
     * @brief Default constructor.
     *
     * Creates an empty matrix.
     */
    pineMatrix();

    /**
     * @brief Constructs a matrix with explicit dimensions.
     * @param rows Number of rows.
     * @param columns Number of columns.
     */
    pineMatrix(std::size_t rows, std::size_t columns);

    /**
     * @brief Constructs a matrix with explicit dimensions and a fill value.
     * @param rows Number of rows.
     * @param columns Number of columns.
     * @param value Value used to initialize the matrix.
     */
    pineMatrix(std::size_t rows, std::size_t columns, const T& value);

    /**
     * @brief Constructs a matrix from a row-major initializer list.
     * @param rows Number of rows.
     * @param columns Number of columns.
     * @param values Flat values arranged row by row.
     */
    pineMatrix(std::size_t rows, std::size_t columns, std::initializer_list<T> values);

    /**
     * @brief Constructs a matrix from nested initializer lists.
     * @param values Matrix values in row-major form.
     */
    pineMatrix(std::initializer_list<std::initializer_list<T>> values);

    /**
     * @brief Copy constructor.
     * @param other Matrix to copy from.
     */
    pineMatrix(const pineMatrix& other);

    /**
     * @brief Destructor.
     */
    ~pineMatrix();

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
     * @brief Returns the total number of elements in the matrix.
     * @return Element count.
     */
    std::size_t size() const;

    /**
     * @brief Checks whether the matrix is empty.
     * @return True if the matrix contains no elements.
     */
    bool empty() const;

    /**
     * @brief Returns a pointer to the underlying buffer.
     * @return Mutable pointer to matrix storage.
     */
    T* data();

    /**
     * @brief Returns a const pointer to the underlying buffer.
     * @return Read-only pointer to matrix storage.
     */
    const T* data() const;

    /**
     * @brief Bounds-checked matrix element access.
     * @param row Row index.
     * @param column Column index.
     * @return Reference to the requested element.
     * @throw std::out_of_range If the index is outside of the matrix bounds.
     */
    T& at(std::size_t row, std::size_t column);

    /**
     * @brief Accesses a matrix element using the indexing operator.
     * @param row Row index.
     * @param column Column index.
     * @return Reference to the requested element.
     */
    T& operator()(std::size_t row, std::size_t column);

    /**
     * @brief Bounds-checked read-only matrix element access.
     * @param row Row index.
     * @param column Column index.
     * @return Const reference to the requested element.
     * @throw std::out_of_range If the index is outside of the matrix bounds.
     */
    const T& at(std::size_t row, std::size_t column) const;

    /**
     * @brief Read-only access to a matrix element using the indexing operator.
     * @param row Row index.
     * @param column Column index.
     * @return Const reference to the requested element.
     */
    const T& operator()(std::size_t row, std::size_t column) const;

    /**
     * @brief Resizes the matrix while preserving any overlapping data.
     * @param rows New row count.
     * @param columns New column count.
     */
    void resize(std::size_t rows, std::size_t columns);

    /**
     * @brief Clears the matrix and releases all stored data.
     */
    void clear();

    /**
     * @brief Fills the matrix with a single value.
     * @param value Value used for initialization.
     */
    void fill(const T& value);

    /**
     * @brief Produces the transpose of the matrix.
     * @return Transposed matrix.
     */
    pineMatrix transpose() const;

    /**
     * @brief Multiplies this matrix by another matrix.
     *
     * The number of columns in this matrix must match the number of rows in
     * the operand matrix.
     *
     * @param other Right-hand matrix.
     * @return Result of matrix multiplication.
     * @throw std::invalid_argument If the dimensions are incompatible.
     */
    pineMatrix matmul(const pineMatrix& other) const;

    /**
     * @brief Adds two matrices element-wise.
     * @param other Matrix to add.
     * @return Resulting matrix.
     * @throw std::invalid_argument If matrix dimensions differ.
     */
    pineMatrix operator+(const pineMatrix& other) const;

    /**
     * @brief Subtracts two matrices element-wise.
     * @param other Matrix to subtract.
     * @return Resulting matrix.
     * @throw std::invalid_argument If matrix dimensions differ.
     */
    pineMatrix operator-(const pineMatrix& other) const;

    /**
     * @brief Multiplies two matrices element-wise.
     * @param other Matrix to multiply by.
     * @return Resulting matrix.
     * @throw std::invalid_argument If matrix dimensions differ.
     */
    pineMatrix operator*(const pineMatrix& other) const;

    /**
     * @brief Divides two matrices element-wise.
     * @param other Matrix containing divisors.
     * @return Resulting matrix.
     * @throw std::invalid_argument If matrix dimensions differ.
     * @throw std::domain_error If any divisor is zero.
     */
    pineMatrix operator/(const pineMatrix& other) const;

    /**
     * @brief Adds another matrix in place.
     * @param other Matrix to add.
     * @return Reference to this matrix.
     * @throw std::invalid_argument If matrix dimensions differ.
     */
    pineMatrix& operator+=(const pineMatrix& other);

    /**
     * @brief Subtracts another matrix in place.
     * @param other Matrix to subtract.
     * @return Reference to this matrix.
     * @throw std::invalid_argument If matrix dimensions differ.
     */
    pineMatrix& operator-=(const pineMatrix& other);

    /**
     * @brief Multiplies another matrix in place.
     * @param other Matrix to multiply by.
     * @return Reference to this matrix.
     * @throw std::invalid_argument If matrix dimensions differ.
     */
    pineMatrix& operator*=(const pineMatrix& other);

    /**
     * @brief Divides this matrix by another in place.
     * @param other Matrix containing divisors.
     * @return Reference to this matrix.
     * @throw std::invalid_argument If matrix dimensions differ.
     * @throw std::domain_error If any divisor is zero.
     */
    pineMatrix& operator/=(const pineMatrix& other);

    /**
     * @brief Copy assignment operator.
     * @param other Matrix to assign from.
     * @return Reference to this matrix.
     */
    pineMatrix& operator=(const pineMatrix& other);

private:
    std::size_t m_rows;      ///< Number of rows.
    std::size_t m_columns;   ///< Number of columns.
    std::size_t m_size;      ///< Total number of elements.
    std::unique_ptr<T[]> m_data; ///< Underlying contiguous storage.

    /**
     * @brief Converts a matrix row and column into a flat index.
     * @param row Row index.
     * @param column Column index.
     * @return Flat storage index.
     */
    std::size_t index(std::size_t row, std::size_t column) const;
};

} // namespace pine

#include "pineMatrix.tpp"