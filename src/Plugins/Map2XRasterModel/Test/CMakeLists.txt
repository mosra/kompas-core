enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Map2XRasterArchiveTestConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Map2XRasterArchiveTestConfigure.h)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Map2XRasterModelTestConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/Map2XRasterModelTestConfigure.h)

map2x_add_test(Map2XRasterArchiveTest Map2XRasterArchiveTest.h Map2XRasterArchiveTest.cpp Map2XCore Map2XRasterModelTestLib)
map2x_add_test(Map2XRasterModelTest Map2XRasterModelTest.h Map2XRasterModelTest.cpp Map2XCore Map2XRasterModelTestLib)
map2x_add_test(Map2XMultiRasterModelTest Map2XMultiRasterModelTest.h Map2XMultiRasterModelTest.cpp Map2XCore Map2XRasterModelTestLib)
