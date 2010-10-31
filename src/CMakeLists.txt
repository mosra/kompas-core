add_definitions(-Wall -pedantic)

include_directories(${MAP2X_CORE_INCLUDE_DIR} ${MAP2X_PLUGINS_INCLUDE_DIR})

add_subdirectory(PluginManager)
add_subdirectory(Plugins)
add_subdirectory(Utility)

set(Map2X_Core_SRCS
    Wgs84Coords.cpp
    AbstractRasterModel.cpp
    Plugins/registerStatic.cpp
)

add_library(Map2XCore SHARED ${Map2X_Core_SRCS})
target_link_libraries(Map2XCore Map2XUtility Map2XPluginManager ${Map2XCore_Plugins})

install(TARGETS Map2XCore DESTINATION ${MAP2X_LIBRARY_INSTALL_DIR})

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
