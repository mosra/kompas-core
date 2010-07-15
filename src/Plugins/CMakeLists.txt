include_directories(${Map2XCore_SOURCE_DIR}/src)

set(Map2X_Core_Plugins_SRCS
    OpenStreetMapTileModel.cpp
)

add_library(Map2XCorePlugins STATIC ${Map2X_Core_Plugins_SRCS})
