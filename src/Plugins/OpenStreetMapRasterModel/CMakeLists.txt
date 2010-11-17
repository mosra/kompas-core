kompas_add_static_plugin(KompasCore_Plugins
    OpenStreetMapRasterModel OpenStreetMapRasterModel.conf OpenStreetMapRasterModel.cpp)

if(WIN32)
    set_target_properties(OpenStreetMapRasterModel PROPERTIES COMPILE_FLAGS -DCORE_EXPORTING)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
