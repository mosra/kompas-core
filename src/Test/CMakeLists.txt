find_package(Qt4 REQUIRED)

enable_testing()

include_directories(${CMAKE_SOURCE_DIR}/src)

set(Map2X_Core_Wgs84CoordsTest_SRCS
    Wgs84CoordsTest.cpp
)

qt4_wrap_cpp(Map2X_Core_Wgs84CoordsTest_MOC
    Wgs84CoordsTest.h
)

add_executable(Wgs84CoordsTest ${Map2X_Core_Wgs84CoordsTest_SRCS} ${Map2X_Core_Wgs84CoordsTest_MOC})
target_link_libraries(Wgs84CoordsTest Map2XCore ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})

add_test(Wgs84CoordsTest Wgs84CoordsTest)
