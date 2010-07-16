find_package(Qt4 REQUIRED)

enable_testing()

include_directories(${Map2XCore_SOURCE_DIR}/src ${CMAKE_CURRENT_BINARY_DIR})

qt4_wrap_cpp(Map2X_Core_Plugins_OpenStreetMapTileModelTest_MOC OpenStreetMapTileModelTest.h)

add_executable(OpenStreetMapTileModelTest OpenStreetMapTileModelTest.cpp ${Map2X_Core_Plugins_OpenStreetMapTileModelTest_MOC})

target_link_libraries(OpenStreetMapTileModelTest Map2XCore ${QT_QTCORE_LIBRARY} ${QT_QTTEST_LIBRARY})

add_test(OpenStreetMapTileModelTest OpenStreetMapTileModelTest)
