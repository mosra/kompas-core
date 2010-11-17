set(Map2XCore_Plugins_Map2XRasterModel_SRCS
    Map2XRasterModel.cpp
    Map2XRasterArchiveReader.cpp
    Map2XRasterArchiveMaker.cpp
)

map2x_add_static_plugin(Map2XCore_Plugins
    Map2XRasterModel Map2XRasterModel.conf ${Map2XCore_Plugins_Map2XRasterModel_SRCS})

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
