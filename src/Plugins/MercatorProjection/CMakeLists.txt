map2x_add_static_plugin(Map2XCore_Plugins MercatorProjection
    MercatorProjection.conf MercatorProjection.cpp)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
