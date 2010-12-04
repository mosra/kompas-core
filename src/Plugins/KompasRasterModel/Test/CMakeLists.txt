enable_testing()

include_directories(${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/testConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/testConfigure.h)

kompas_add_test(KompasRasterArchiveTest KompasRasterArchiveTest.h KompasRasterArchiveTest.cpp KompasCore)
kompas_add_test(KompasRasterModelTest KompasRasterModelTest.h KompasRasterModelTest.cpp KompasCore)
kompas_add_test(KompasMultiRasterModelTest KompasMultiRasterModelTest.h KompasMultiRasterModelTest.cpp KompasCore)
