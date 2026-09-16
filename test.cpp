#include <cassert>
#include <iostream>
#include <stdexcept>
#include "pineMatrix.hpp"
#include "pineVector.hpp"
#include "pineTensor.hpp"

int main() {
    // valid multiplication: 4x2 * 2x4 -> 4x4
    pine::pineMatrix<float> a({
        {1.0f, 2.0f},
        {3.0f, 4.0f},
        {9.0f, 10.0f},
        {11.0f, 12.0f}
    });

    pine::pineMatrix<float> b({
        {5.0f, 6.0f, 13.0f, 15.0f},
        {7.0f, 8.0f, 14.0f, 16.0f}
    });

    auto c = a.matmul(b);
    assert(c.rows() == 4);
    assert(c.columns() == 4);
    assert(c(0, 0) == 19.0f);
    assert(c(0, 1) == 22.0f);
    assert(c(3, 3) == 357.0f);

    std::cout << "Matrix product:\n";
    for (std::size_t i = 0; i < c.rows(); ++i) {
        for (std::size_t j = 0; j < c.columns(); ++j) {
            std::cout << c(i, j) << " ";
        }
        std::cout << "\n";
    }

    // transpose shape and values
    auto t = a.transpose();
    assert(t.rows() == 2);
    assert(t.columns() == 4);
    assert(t(0, 0) == 1.0f);
    assert(t(1, 3) == 12.0f);

    std::cout << "Transpose:\n";
    for (std::size_t i = 0; i < t.rows(); ++i) {
        for (std::size_t j = 0; j < t.columns(); ++j) {
            std::cout << t(i, j) << " ";
        }
        std::cout << "\n";
    }

    // invalid multiplication dimensions must throw: 2x3 * 2x2 is invalid because 3 != 2
    pine::pineMatrix<float> bad_left({
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f}
    });

    pine::pineMatrix<float> bad_right({
        {1.0f, 2.0f},
        {3.0f, 4.0f}
    });

    bool threw = false;
    try {
        (void)bad_left.matmul(bad_right);
    } catch (const std::invalid_argument&) {
        threw = true;
    }
    assert(threw);

    // vector smoke tests
    pine::pineVector<int> v({1, 2, 3, 4});
    assert(v.size() == 4);
    assert(v[0] == 1);
    assert(v[3] == 4);
    assert((v + v)[2] == 6);

    std::cout << "Vector:\n";
    for (std::size_t i = 0; i < v.size(); ++i) {
        std::cout << v[i] << " ";
    }
    std::cout << "\n";

    // tensor smoke tests
    pine::pineTensor<float> tensor({
        {
            {1.0f, 2.0f},
            {3.0f, 4.0f}
        },
        {
            {5.0f, 6.0f},
            {7.0f, 8.0f}
        }
    });
    assert(tensor.rows() == 2);
    assert(tensor.columns() == 2);
    assert(tensor.depth() == 2);
    assert(tensor(1, 1, 0) == 7.0f);

    pine::pineTensor<float> tensor2({
        {
            {10.0f, 20.0f},
            {30.0f, 40.0f}
        },
        {
            {50.0f, 60.0f},
            {70.0f, 80.0f}
        }
    });
    auto tensorSum = tensor + tensor2;
    assert(tensorSum(0, 0, 1) == 22.0f);
    assert(tensorSum(1, 1, 1) == 88.0f);

    std::cout << "Tensor sum:\n";
    for (std::size_t i = 0; i < tensorSum.rows(); ++i) {
        for (std::size_t j = 0; j < tensorSum.columns(); ++j) {
            for (std::size_t k = 0; k < tensorSum.depth(); ++k) {
                std::cout << tensorSum(i, j, k) << " ";
            }
            std::cout << "| ";
        }
        std::cout << "\n";
    }

    std::cout << "All matrix, vector, and tensor tests passed.\n";
    return 0;
}