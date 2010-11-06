enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/testConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/testConfigure.h)

map2x_add_test(Map2XRasterArchiveTest Map2XRasterArchiveTest.h Map2XRasterArchiveTest.cpp Map2XCore Map2XRasterModelTestLib)
map2x_add_test(Map2XRasterModelTest Map2XRasterModelTest.h Map2XRasterModelTest.cpp Map2XCore Map2XRasterModelTestLib)
map2x_add_test(Map2XMultiRasterModelTest Map2XMultiRasterModelTest.h Map2XMultiRasterModelTest.cpp Map2XCore Map2XRasterModelTestLib)
