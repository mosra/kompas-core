kompas_add_static_plugin(KompasCore_Plugins
    OpenStreetMapRasterModel OpenStreetMapRasterModel.conf OpenStreetMapRasterModel.cpp)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
