add_definitions(-Wall -pedantic)

add_subdirectory(PluginManager)
add_subdirectory(Utility)

include_directories(${Map2XCore_SOURCE_DIR}/src ${Map2XCore_BINARY_DIR}/src)

set(Map2X_Core_SRCS
    TileArchiveReader.cpp
    Wgs84Coords.cpp
)

add_library(Map2XCore SHARED ${Map2X_Core_SRCS})
target_link_libraries(Map2XCore Map2XUtility Map2XPluginManager)

if(BUILD_TESTS)
    add_subdirectory(Test)
endif()
