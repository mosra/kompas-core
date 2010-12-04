enable_testing()

kompas_add_test(Wgs84CoordsTest Wgs84CoordsTest.h Wgs84CoordsTest.cpp KompasCore)
kompas_add_test(CoordsTest CoordsTest.h CoordsTest.cpp KompasCore)
kompas_add_test(AreaTest AreaTest.h AreaTest.cpp KompasCore)
kompas_add_test(AbsoluteAreaTest AbsoluteAreaTest.h AbsoluteAreaTest.cpp KompasCore)
