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

#include "Map2XMultiRasterModelTest.h"

#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "testConfigure.h"

QTEST_APPLESS_MAIN(Map2X::Plugins::Test::Map2XMultiRasterModelTest)

using namespace std;
using namespace Map2X::Core;
using namespace Map2X::Utility;

namespace Map2X { namespace Plugins { namespace Test {

void Map2XMultiRasterModelTest::initialization() {
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

void Map2XMultiRasterModelTest::expansion() {
    QVERIFY(model.area() == TileArea(2, 1, 3, 4));

    vector<Zoom> z;
    z.push_back(1);
    z.push_back(2);
    z.push_back(3);
    z.push_back(4);
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

void Map2XMultiRasterModelTest::get() {
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
