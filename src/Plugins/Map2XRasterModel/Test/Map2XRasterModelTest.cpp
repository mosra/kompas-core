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

#include "Map2XRasterModelTest.h"

#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "Map2XRasterModelTestConfigure.h"

QTEST_APPLESS_MAIN(Map2X::Plugins::Test::Map2XRasterModelTest)

using namespace std;
using namespace Map2X::Utility;
using namespace Map2X::Core;

namespace Map2X { namespace Plugins { namespace Test {

void Map2XRasterModelTest::metadata() {
    QVERIFY(model.addPackage(Directory::join(RASTERMODEL_TEST_DIR, "small/map.conf")) == 0);

    QVERIFY(model.attribute(AbstractRasterModel::Name, 0) == "Map2X testing package");
    QVERIFY(model.attribute(AbstractRasterModel::Description, 0) == "Buggy and sparse");
    QVERIFY(model.attribute(AbstractRasterModel::Packager, 0) == "mosra");

    QVERIFY(model.area() == TileArea(6, 7, 2, 2));
    QVERIFY(model.zoomStep() == 2);
    QVERIFY(model.tileSize() == TileSize(256, 256));

    vector<Zoom> z;
    z.push_back(2);
    z.push_back(3);
    QVERIFY(model.zoomLevels() == z);

    vector<string> layers;
    layers.push_back("base");
    QVERIFY(model.layers() == layers);

    vector<string> overlays;
    overlays.push_back("relief");
    QVERIFY(model.overlays() == overlays);
}

void Map2XRasterModelTest::tiles() {
    QVERIFY(model.tileFromPackage("base", 2, TileCoords(6, 8)) == "3");
    QVERIFY(model.tileFromPackage("relief", 2, TileCoords(7, 7)) == "p");
    QVERIFY(model.tileFromPackage("base", 3, TileCoords(14, 16)) == "a");
    QVERIFY(model.tileFromPackage("base", 3, TileCoords(13, 15)) == "6");
}

}}}
