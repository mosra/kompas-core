set(KompasCore_Plugins_KompasRasterModel_SRCS
    KompasRasterModel.cpp
    KompasRasterArchiveReader.cpp
    KompasRasterArchiveMaker.cpp
)

corrade_add_static_plugin(KompasCore_Plugins
    KompasRasterModel KompasRasterModel.conf ${KompasCore_Plugins_KompasRasterModel_SRCS})

if(WIN32)
    set_target_properties(KompasRasterModel PROPERTIES COMPILE_FLAGS -DCORE_EXPORTING)
    target_link_libraries(KompasRasterModel ${CORRADE_UTILITY_LIBRARY} ${CORRADE_PLUGINMANAGER_LIBRARY})
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
