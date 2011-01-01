/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "KompasRasterModelTest.h"

#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "testConfigure.h"

QTEST_APPLESS_MAIN(Kompas::Plugins::Test::KompasRasterModelTest)

using namespace std;
using namespace Kompas::Utility;
using namespace Kompas::Core;

namespace Kompas { namespace Plugins { namespace Test {

void KompasRasterModelTest::metadata() {
    QVERIFY(model.addPackage(Directory::join(RASTERMODEL_TEST_DIR, "small/map.conf")) == 0);

    /* Opening multiple packages in generic model is not possible */
    QVERIFY(model.addPackage(Directory::join(RASTERMODEL_TEST_DIR, "small/map.conf")) == -1);

    QVERIFY(model.packageAttribute(0, AbstractRasterModel::Filename) == Directory::join(RASTERMODEL_TEST_DIR, "small/map.conf"));
    QVERIFY(model.packageAttribute(0, AbstractRasterModel::Name) == "Kompas testing package");
    QVERIFY(model.packageAttribute(0, AbstractRasterModel::Description) == "Buggy and sparse");
    QVERIFY(model.packageAttribute(0, AbstractRasterModel::Packager) == "mosra");

    QVERIFY(model.area() == TileArea(6, 7, 2, 2));
    QVERIFY(model.tileSize() == TileSize(256, 256));

    set<Zoom> z;
    z.insert(2);
    z.insert(3);
    QVERIFY(model.zoomLevels() == z);

    vector<string> layers;
    layers.push_back("base");
    QVERIFY(model.layers() == layers);

    vector<string> overlays;
    overlays.push_back("relief");
    QVERIFY(model.overlays() == overlays);
}

void KompasRasterModelTest::tiles() {
    QVERIFY(model.tileFromPackage("base", 2, TileCoords(6, 8)) == "3");
    QVERIFY(model.tileFromPackage("relief", 2, TileCoords(7, 7)) == "p");
    QVERIFY(model.tileFromPackage("base", 3, TileCoords(14, 16)) == "a");
    QVERIFY(model.tileFromPackage("base", 3, TileCoords(13, 15)) == "6");
}

void KompasRasterModelTest::create() {
    QDir dir;
    dir.remove(RASTERMODEL_WRITE_TEST_DIR);

    vector<Zoom> zoomLevels;
    zoomLevels.push_back(3);
    zoomLevels.push_back(2);

    vector<string> layers;
    layers.push_back("base");

    vector<string> overlays;
    overlays.push_back("relief");

    QVERIFY(model.initializePackage(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "map.conf"), TileSize(256, 256), zoomLevels, TileArea(6, 7, 2, 2), layers, overlays));
    QVERIFY(model.setPackageAttribute(AbstractRasterModel::Name, "Kompas testing package"));
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

    QFile base2(QString::fromStdString(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "base/2.kps")));
    QFile base2Expected(QString::fromStdString(Directory::join(RASTERMODEL_TEST_DIR, "small/base/2.kps")));
    base2.open(QFile::ReadOnly);
    base2Expected.open(QFile::ReadOnly);
    QVERIFY(base2.readAll() == base2Expected.readAll());

    QFile base3(QString::fromStdString(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "base/3.kps")));
    QFile base3Expected(QString::fromStdString(Directory::join(RASTERMODEL_TEST_DIR, "small/base/3.kps")));
    base3.open(QFile::ReadOnly);
    base3Expected.open(QFile::ReadOnly);
    QVERIFY(base3.readAll() == base3Expected.readAll());

    QFile relief2(QString::fromStdString(Directory::join(RASTERMODEL_WRITE_TEST_DIR, "relief/2.kps")));
    QFile relief2Expected(QString::fromStdString(Directory::join(RASTERMODEL_TEST_DIR, "small/relief/2.kps")));
    relief2.open(QFile::ReadOnly);
    relief2Expected.open(QFile::ReadOnly);
    QVERIFY(relief2.readAll() == relief2Expected.readAll());
}

void KompasRasterModelTest::recognizeFile_data() {
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
        << "map.conf" << "version=3\nmodel=KompasRasterModel"
        << (int) AbstractRasterModel::FullySupported;
    QTest::newRow("subclassedModel")
        << "map.conf" << "version=3\nmodel=OpenStreetMapRasterModel"
        << (int) AbstractRasterModel::PartiallySupported;
}

void KompasRasterModelTest::recognizeFile() {
    QFETCH(QString, filename);
    QFETCH(QString, file);
    QFETCH(int, state);

    istringstream i(file.toStdString());

    QVERIFY(model.recognizeFile(filename.toStdString(), i) == state);
}

}}}
