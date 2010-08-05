add_subdirectory(OpenStreetMapTileModel)

set(Map2XCore_Plugins
    Map2XCorePluginsOpenStreetMapTileModel
    PARENT_SCOPE)

if(BUILD_TESTS)
    enable_testing()
endif()
