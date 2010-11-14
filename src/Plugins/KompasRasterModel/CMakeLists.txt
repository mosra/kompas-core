set(KompasCore_Plugins_KompasRasterModel_SRCS
    KompasRasterModel.cpp
    KompasRasterArchiveReader.cpp
    KompasRasterArchiveMaker.cpp
)

kompas_add_static_plugin(KompasCore_Plugins
    KompasRasterModel KompasRasterModel.conf ${KompasCore_Plugins_KompasRasterModel_SRCS})

if(WIN32)
    target_link_libraries(KompasRasterModel KompasPluginManager)
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
