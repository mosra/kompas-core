find_package(Qt4 REQUIRED)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/TileArchiveTestConfigure.h.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/TileArchiveTestConfigure.h)

qt4_wrap_cpp(Map2X_Core_Wgs84CoordsTest_MOC Wgs84CoordsTest.h)
qt4_wrap_cpp(Map2X_Core_TileArchiveTest_MOC TileArchiveTest.h)

add_executable(Wgs84CoordsTest Wgs84CoordsTest.cpp ${Map2X_Core_Wgs84CoordsTest_MOC})
add_executable(TileArchiveTest TileArchiveTest.cpp ${Map2X_Core_TileArchiveTest_MOC})

target_link_libraries(Wgs84CoordsTest Map2XCore ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})
target_link_libraries(TileArchiveTest Map2XCore ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})

add_test(Wgs84CoordsTest Wgs84CoordsTest)
add_test(TileArchiveTest TileArchiveTest)
