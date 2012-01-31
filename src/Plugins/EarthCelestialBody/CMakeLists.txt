corrade_add_static_plugin(KompasCore_Plugins EarthCelestialBody
    EarthCelestialBody.conf EarthCelestialBody.cpp)

if(WIN32)
    set_target_properties(EarthCelestialBody PROPERTIES COMPILE_FLAGS -DCORE_EXPORTING)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
