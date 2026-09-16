cmake_minimum_required(VERSION 3.16)
project(PineAPI VERSION 0.1.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(PineAPI INTERFACE)

target_include_directories(PineAPI
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_compile_features(PineAPI INTERFACE cxx_std_17)

option(PINEAPI_BUILD_TESTS "Build PineAPI sample test program" ON)

if(PINEAPI_BUILD_TESTS)
    add_executable(test_app test.cpp)
    target_link_libraries(test_app PRIVATE PineAPI)
    target_include_directories(test_app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
endif()

install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install(TARGETS PineAPI EXPORT PineAPITargets)

install(EXPORT PineAPITargets
    FILE PineAPITargets.cmake
    NAMESPACE PineAPI::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PineAPI
)

configure_package_config_file(
    ${CMAKE_CURRENT_SOURCE_DIR}/cmake/PineAPIConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/PineAPIConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PineAPI
)

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/PineAPIConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/PineAPIConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/PineAPIConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/PineAPI
)
