find_package(Qt4 REQUIRED)

enable_testing()
include(Map2XMacros)

include_directories(${Map2XCore_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TileArchiveTestConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/TileArchiveTestConfigure.h)

map2x_add_test(Wgs84CoordsTest Wgs84CoordsTest.h Wgs84CoordsTest.cpp Map2XCore)
map2x_add_test(TileArchiveTest TileArchiveTest.h TileArchiveTest.cpp Map2XCore)
map2x_add_test(AbstractMercatorTileModelTest
    AbstractMercatorTileModelTest.h AbstractMercatorTileModelTest.cpp Map2XCore)
