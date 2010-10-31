map2x_add_static_plugin(Map2XCore_Plugins
    Map2XRasterModel Map2XRasterModel.conf Map2XRasterModel.cpp Map2XRasterArchiveReader.cpp)

if(BUILD_TESTS)
    enable_testing()
    add_library(Map2XRasterModelTestLib Map2XRasterModel.cpp Map2XRasterArchiveReader.cpp)
    add_subdirectory(Test)
endif()
