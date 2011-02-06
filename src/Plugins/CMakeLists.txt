add_subdirectory(EarthCelestialBody)
add_subdirectory(KompasRasterModel)
add_subdirectory(OpenStreetMapRasterModel)
add_subdirectory(MercatorProjection)

# Propagate plugin list variable to parent scope
set(KompasCore_Plugins ${KompasCore_Plugins} PARENT_SCOPE)

if(BUILD_TESTS)
    enable_testing()
endif()
