add_definitions(-Wall -pedantic)

include_directories(${KOMPAS_CORE_INCLUDE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})

add_subdirectory(PluginManager)
add_subdirectory(Plugins)
add_subdirectory(Utility)

set(Kompas_Core_SRCS
    Wgs84Coords.cpp
    AbstractRasterModel.cpp
    Plugins/registerStatic.cpp
)

add_library(KompasCore SHARED ${Kompas_Core_SRCS})
target_link_libraries(KompasCore KompasUtility KompasPluginManager ${KompasCore_Plugins})

if(WIN32)
    set_target_properties(KompasCore PROPERTIES COMPILE_FLAGS -DCORE_EXPORTING)
endif()

install(TARGETS KompasCore DESTINATION ${KOMPAS_LIBRARY_INSTALL_DIR})

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
