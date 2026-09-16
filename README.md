# PineAPI

PineAPI is a lightweight C++ template-based math library for vectors, matrices, and tensors.

It is designed to be easy to include in other C++ projects and to work as a header-driven library with minimal setup.

## Features

- Generic vector operations
- Matrix arithmetic and multiplication
- Tensor arithmetic
- Header-only style template implementation
- Simple build with `make`
- CMake package configuration included

## Project structure

- `include/` — public headers
- `source/` — template implementations
- `test.cpp` — smoke tests and validation
- `Makefile` — local build helper
- `CMakeLists.txt` — packaging support

## Build

```bash
make
./test_app
```

## Include in another project

If you are compiling from another C++ project, add the library include path:

```bash
g++ -std=c++17 -I/path/to/PineAPI/include your_program.cpp -o your_program
```

Then include:

```cpp
#include "pineMatrix.hpp"
#include "pineVector.hpp"
#include "pineTensor.hpp"
```

## Example

```cpp
#include "pineMatrix.hpp"
#include "pineVector.hpp"

int main() {
    pine::pineVector<int> v({1, 2, 3});
    auto w = v + v;

    pine::pineMatrix<float> a({
        {1.0f, 2.0f},
        {3.0f, 4.0f}
    });

    pine::pineMatrix<float> b({
        {5.0f, 6.0f},
        {7.0f, 8.0f}
    });

    auto c = a.matmul(b);
    return 0;
}
```

## License

This project is licensed under the MIT License.

The MIT license allows commercial use, modification, distribution, and private use, while requiring that the original copyright and license notice remain included in copies or substantial portions of the software.

If you want a stronger requirement like "must credit us in marketing material or public docs," that would require a custom license rather than a standard open-source license.

## Recommended licensing choice

For your goal of open-source + attribution, the best standard option is:

- MIT License

It keeps the project open and usable commercially, while still requiring attribution and preserving your copyright.

## Contributing

Contributions are welcome. Please keep changes focused, test them with the provided build, and preserve the library’s minimal dependency footprint.
