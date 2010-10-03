include_directories(${Map2XCore_SOURCE_DIR}/src)

add_subdirectory(OpenStreetMapTileModel)
add_subdirectory(MercatorProjection)

# Propagate plugin list variable to parent scope
set(Map2XCore_Plugins ${Map2XCore_Plugins} PARENT_SCOPE)

if(BUILD_TESTS)
    enable_testing()
endif()
