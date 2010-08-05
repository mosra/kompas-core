include_directories(${Map2XCore_SOURCE_DIR}/src)

set(Map2X_Core_Plugins_OpenStreetMapTileModel_SRCS
    OpenStreetMapTileModel.cpp
)

add_library(Map2XCorePluginsOpenStreetMapTileModel STATIC ${Map2X_Core_Plugins_OpenStreetMapTileModel_SRCS})

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
