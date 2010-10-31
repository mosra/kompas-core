/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Map2X.

    Map2X is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Map2X is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "Map2XRasterArchiveTest.h"

#include <string>
#include <QtTest/QTest>

#include "Map2XRasterModel/Map2XRasterArchiveReader.h"

#include "Map2XRasterArchiveTestConfigure.h"

QTEST_APPLESS_MAIN(Map2X::Plugins::Test::Map2XRasterArchiveTest)

using namespace std;

namespace Map2X { namespace Plugins { namespace Test {

void Map2XRasterArchiveTest::reader() {
    Map2XRasterArchiveReader r(RASTERARCHIVE_TEST_DIR + string("version2.map"));

    QVERIFY(r.isValid());
    QVERIFY(r.version() == 2);
    QVERIFY(r.total() == (unsigned int) 16);
    QVERIFY(r.begin() == (unsigned int) 5);
    QVERIFY(r.end() == (unsigned int) 8);

    /* Tiles out of range */
    QVERIFY(r.get(4) == "");
    QVERIFY(r.get(8) == "");

    /* Normal tiles */
    QVERIFY(r.get(5) == "5555");

    /* Empty tile */
    QVERIFY(r.get(6) == "");

    /* Last tile */
    QVERIFY(r.get(7) == "7777");
}

}}}
