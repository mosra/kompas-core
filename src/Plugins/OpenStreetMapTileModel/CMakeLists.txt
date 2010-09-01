include_directories(${Map2XCore_SOURCE_DIR}/src)

set(Map2X_Core_Plugins_OpenStreetMapTileModel_SRCS
    OpenStreetMapTileModel.cpp
)

map2x_add_static_plugin(Map2XCore_Plugins
    OpenStreetMap OpenStreetMapTileModel.conf OpenStreetMapTileModel.cpp)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
