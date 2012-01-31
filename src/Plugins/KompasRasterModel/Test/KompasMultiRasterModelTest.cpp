/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "KompasMultiRasterModelTest.h"

#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "testConfigure.h"

QTEST_APPLESS_MAIN(Kompas::Plugins::Test::KompasMultiRasterModelTest)

using namespace std;
using namespace Corrade::Utility;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins { namespace Test {

void KompasMultiRasterModelTest::initialization() {
    /* Offline map parameters should be updated even when the online is enabled */
    model.setOnline(true);

    QVERIFY(model.addPackage(Directory::join(RASTERMODEL_TEST_DIR, "small/map.conf")) == 0);

    /* With lowest zoom level < current */
    QVERIFY(model.addPackage(Directory::join(RASTERMODEL_TEST_DIR, "big/map.conf")) == 1);

    /* With lowest zoom level > current */
    QVERIFY(model.addPackage(Directory::join(RASTERMODEL_TEST_DIR, "map.conf.smallZoom")) == 2);

    /* Different tileSize */
    QVERIFY(model.addPackage(Directory::join(RASTERMODEL_TEST_DIR, "map.conf.different1")) == -1);

    model.setOnline(false);
}

void KompasMultiRasterModelTest::expansion() {
    QVERIFY(model.area() == TileArea(2, 1, 3, 4));

    set<Zoom> z;
    z.insert(1);
    z.insert(2);
    z.insert(3);
    z.insert(4);
    QVERIFY(model.zoomLevels() == z);

    vector<string> layers;
    layers.push_back("base");
    layers.push_back("photo");
    QVERIFY(model.layers() == layers);

    vector<string> overlays;
    overlays.push_back("relief");
    overlays.push_back("cycle");
    QVERIFY(model.overlays() == overlays);
}

void KompasMultiRasterModelTest::get() {
    /* Tile only in first package */
    QVERIFY(model.tileFromPackage("base", 2, TileCoords(6, 8)) == "3");

    /* Tile only in second package */
    QVERIFY(model.tileFromPackage("base", 2, TileCoords(5, 6)) == "p");

    /* Tile in tileArea, but in no package */
    QVERIFY(model.tileFromPackage("base", 2, TileCoords(5, 8)) == "");

    /* Tile in both packages (first package has precedency) */
    QVERIFY(model.tileFromPackage("base", 2, TileCoords(7, 7)) == "2");
}

}}}
