find_package(Qt4 REQUIRED)

enable_testing()
include(Map2XMacros)

include_directories(${Map2XCore_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})

map2x_add_test(OpenStreetMapTileModelTest OpenStreetMapTileModelTest.h OpenStreetMapTileModelTest.cpp Map2XCore)
