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

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "testConfigure.h"

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

void Map2XRasterModelTest::create() {
    QDir dir;
    dir.remove(RASTERMODEL_WRITE_TEST_DIR);

    vector<Zoom> zoomLevels;
    zoomLevels.push_back(3);
    zoomLevels.push_back(2);

    vector<string> layers;
    layers.push_back("base");

    vector<string> overlays;
    overlays.push_back("relief");

    QVERIFY(model.initializePackage(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "map.conf"), TileSize(256, 256), zoomLevels, 2, TileArea(6, 7, 2, 2), layers, overlays));
    QVERIFY(model.setPackageAttribute(AbstractRasterModel::Name, "Map2X testing package"));
    QVERIFY(model.setPackageAttribute(AbstractRasterModel::Description, "Buggy and sparse"));
    QVERIFY(model.setPackageAttribute(AbstractRasterModel::Packager, "mosra"));

    QVERIFY(model.tileToPackage("base", 2, TileCoords(6, 7), "1"));
    QVERIFY(model.tileToPackage("relief", 2, TileCoords(6, 7), "o"));
    QVERIFY(model.tileToPackage("base", 2, TileCoords(7, 7), "2"));
    QVERIFY(model.tileToPackage("relief", 2, TileCoords(7, 7), "p"));
    QVERIFY(model.tileToPackage("base", 2, TileCoords(6, 8), "3"));
    QVERIFY(model.tileToPackage("relief", 2, TileCoords(6, 8), "q"));
    QVERIFY(model.tileToPackage("base", 2, TileCoords(7, 8), "4"));
    QVERIFY(model.tileToPackage("relief", 2, TileCoords(7, 8), "r"));

    QVERIFY(model.tileToPackage("base", 3, TileCoords(12, 14), "1"));
    QVERIFY(model.tileToPackage("base", 3, TileCoords(13, 14), "2"));
    QVERIFY(model.tileToPackage("base", 3, TileCoords(14, 14), "3"));
    QVERIFY(model.tileToPackage("base", 3, TileCoords(15, 14), "4"));
    QVERIFY(model.tileToPackage("base", 3, TileCoords(12, 15), "5"));
    QVERIFY(model.tileToPackage("base", 3, TileCoords(13, 15), "6"));

    /* Tile out of order */
    QVERIFY(!model.tileToPackage("base", 3, TileCoords(14, 16), "a"));

    QVERIFY(model.finalizePackage());

    /* Compare created files with expected */
    QFile conf(QString::fromStdString(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "map.conf")));
    QFile confExpected(QString::fromStdString(Directory::join(RASTERMODEL_TEST_DIR, "map.conf.expected")));
    conf.open(QFile::ReadOnly);
    confExpected.open(QFile::ReadOnly);
    QVERIFY(conf.readAll() == confExpected.readAll());

    QFile base2(QString::fromStdString(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "base/2.map")));
    QFile base2Expected(QString::fromStdString(Directory::join(RASTERMODEL_TEST_DIR, "base/2.map")));
    base2.open(QFile::ReadOnly);
    base2Expected.open(QFile::ReadOnly);
    QVERIFY(base2.readAll() == base2Expected.readAll());

    QFile base3(QString::fromStdString(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "base/3.map")));
    QFile base3Expected(QString::fromStdString(Directory::join(RASTERMODEL_TEST_DIR, "base/3.map")));
    base3.open(QFile::ReadOnly);
    base3Expected.open(QFile::ReadOnly);
    QVERIFY(base3.readAll() == base3Expected.readAll());

    QFile relief2(QString::fromStdString(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "relief/2.map")));
    QFile relief2Expected(QString::fromStdString(Directory::join(RASTERMODEL_TEST_DIR, "relief/2.map")));
    relief2.open(QFile::ReadOnly);
    relief2Expected.open(QFile::ReadOnly);
    QVERIFY(relief2.readAll() == relief2Expected.readAll());
}

void Map2XRasterModelTest::recognizeFile_data() {
    QTest::addColumn<QString>("filename");
    QTest::addColumn<QString>("file");
    QTest::addColumn<int>("state");

    QTest::newRow("badFilename")
        << "map.shp" << ""
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("badVersion")
        << "map.conf" << "version=2"
        << (int) AbstractRasterModel::NotSupported;
    QTest::newRow("genericModel")
        << "map.conf" << "version=3\nmodel=Map2XRasterModel"
        << (int) AbstractRasterModel::FullySupported;
    QTest::newRow("subclassedModel")
        << "map.conf" << "version=3\nmodel=OpenStreetMapRasterModel"
        << (int) AbstractRasterModel::PartiallySupported;
}

void Map2XRasterModelTest::recognizeFile() {
    QFETCH(QString, filename);
    QFETCH(QString, file);
    QFETCH(int, state);

    istringstream i(file.toStdString());

    QVERIFY(model.recognizeFile(filename.toStdString(), i) == state);
}

}}}
