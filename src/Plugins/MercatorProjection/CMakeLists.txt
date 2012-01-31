corrade_add_static_plugin(KompasCore_Plugins MercatorProjection
    MercatorProjection.conf MercatorProjection.cpp)

if(WIN32)
    set_target_properties(MercatorProjection PROPERTIES COMPILE_FLAGS -DCORE_EXPORTING)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
