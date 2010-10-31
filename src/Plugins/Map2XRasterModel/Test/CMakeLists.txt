enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Map2XRasterArchiveTestConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Map2XRasterArchiveTestConfigure.h)

map2x_add_test(Map2XRasterArchiveTest Map2XRasterArchiveTest.h Map2XRasterArchiveTest.cpp Map2XCore Map2XRasterModelTestLib)
