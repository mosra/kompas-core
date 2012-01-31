set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -pedantic")

include_directories(${CORRADE_INCLUDE_DIR} ${KOMPAS_CORE_INCLUDE_DIR} ${KOMPAS_PLUGINS_INCLUDE_DIR})

if(BUILD_TESTS)
    # Don't display long long warnings for Qt
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-long-long")
    include_directories(${QT_INCLUDE_DIR})
endif()

add_subdirectory(Plugins)

set(Kompas_Core_SRCS
    LatLonCoords.cpp
    AbstractCache.cpp
    AbstractCelestialBody.cpp
    AbstractRasterModel.cpp
    Plugins/registerStatic.cpp
)

add_library(KompasCore SHARED ${Kompas_Core_SRCS})
target_link_libraries(KompasCore ${CORRADE_UTILITY_LIBRARY} ${CORRADE_PLUGINMANAGER_LIBRARY} ${KompasCore_Plugins})
set_target_properties(KompasCore PROPERTIES VERSION ${KOMPAS_CORE_LIBRARY_VERSION} SOVERSION ${KOMPAS_CORE_LIBRARY_SOVERSION})

if(WIN32)
    set_target_properties(KompasCore PROPERTIES COMPILE_FLAGS -DCORE_EXPORTING)
endif()

install(TARGETS KompasCore DESTINATION ${KOMPAS_LIBRARY_INSTALL_DIR})

# Include also runtime libs for Win32
if(WIN32)
    corrade_bundle_dlls(${KOMPAS_LIBRARY_INSTALL_DIR}
        libCorradeUtility.dll
        libCorradePluginManager.dll
        libgcc_s_sjlj-1.dll
        libstdc++-6.dll
        mingwm10.dll
        PATHS ${DLL_SEARCH_PATH}
    )
endif()

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(Test)
endif()
