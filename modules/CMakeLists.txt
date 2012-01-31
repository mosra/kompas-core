if(NOT WIN32)
    install(FILES
        FindKompasCore.cmake
        DESTINATION ${KOMPAS_CMAKE_MODULE_INSTALL_DIR})
endif()
