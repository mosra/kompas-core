add_definitions(-Wall -pedantic)

add_subdirectory(PluginManager)
add_subdirectory(Utility)

set(Map2X_Core_SRCS
    Wgs84Coords.cpp
)

add_library(Map2XCore SHARED ${Map2X_Core_SRCS})
target_link_libraries(Map2XCore Map2XUtility Map2XPluginManager)

if(BUILD_TESTS)
    add_subdirectory(Test)
endif()
