add_definitions(-Wall -pedantic)

add_subdirectory(PluginManager)
add_subdirectory(Plugins)
add_subdirectory(Utility)

include_directories(${Map2XCore_SOURCE_DIR}/src ${Map2XCore_BINARY_DIR}/src)

set(Map2X_Core_SRCS
    TileArchiveReader.cpp
    Wgs84Coords.cpp
    AbstractTileModel.cpp
    AbstractMercatorTileModel.cpp
    Plugins/registerStatic.cpp
)

add_library(Map2XCore SHARED ${Map2X_Core_SRCS})
target_link_libraries(Map2XCore Map2XUtility Map2XPluginManager ${Map2XCore_Plugins})

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
