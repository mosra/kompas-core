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
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "Map2XRasterModel/Map2XRasterArchiveReader.h"
#include "Map2XRasterModel/Map2XRasterArchiveMaker.h"

#include "Map2XRasterArchiveTestConfigure.h"

QTEST_APPLESS_MAIN(Map2X::Plugins::Test::Map2XRasterArchiveTest)

using namespace std;
using namespace Map2X::Utility;

namespace Map2X { namespace Plugins { namespace Test {

Map2XRasterArchiveTest::Map2XRasterArchiveTest(QObject* parent): QObject(parent) {
    QDir dir;
    dir.mkpath(RASTERARCHIVE_WRITE_TEST_DIR);
}

void Map2XRasterArchiveTest::reader2() {
    Map2XRasterArchiveReader r(Directory::join(RASTERARCHIVE_TEST_DIR, "version2.map"));

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

    /* Tile with null byte */
    QVERIFY(r.get(7) == "7" + string("\0", 1) + "77");
}

void Map2XRasterArchiveTest::reader3() {
    Map2XRasterArchiveReader r(Directory::join(RASTERARCHIVE_TEST_DIR, "version3.map"));

    QVERIFY(r.isValid());
    QVERIFY(r.version() == 3);
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

    /* Tile with null byte */
    QVERIFY(r.get(7) == "7" + string("\0", 1) + "77");
}

void Map2XRasterArchiveTest::maker2() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make2.map")));

    Map2XRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make2"), 2, 10);
    QVERIFY(m.finish() == Map2XRasterArchiveMaker::VersionError);

    QVERIFY(!QFile::exists(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make2.map"))));
}

void Map2XRasterArchiveTest::maker() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make.map")));

    Map2XRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make"), 3, 3);
    QCOMPARE(m.currentFileNumber(), -1);
    QCOMPARE(m.currentFileSize(), 0u);

    QVERIFY(m.append("1111") == Map2XRasterArchiveMaker::NextFile);
    QCOMPARE(m.currentFileNumber(), 0);
    QCOMPARE(m.currentFileSize(), 28u);

    QVERIFY(m.append("") == Map2XRasterArchiveMaker::Ok);
    QVERIFY(m.append("3333") == Map2XRasterArchiveMaker::Ok);
    QVERIFY(m.finish() == Map2XRasterArchiveMaker::Ok);
    QCOMPARE(m.currentFileNumber(), 0);
    QCOMPARE(m.currentFileSize(), 0u);
    QCOMPARE(m.currentFileTileCount(), 0u);

    QFile f(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make.map")));
    f.open(QFile::ReadOnly);
    QCOMPARE(f.readAll(), QByteArray(
        "MAP\x03"           "\x03\x00\x00\x00"  "\x00\x00\x00\x00"
        "\x03\x00\x00\x00"  "1111"              "3333"
        "\x10\x00\x00\x00"  "\x14\x00\x00\x00"  "\x14\x00\x00\x00"
        "\x18\x00\x00\x00", 40));
}

void Map2XRasterArchiveTest::makerEmpty() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeEmpty.map")));

    Map2XRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeEmpty"), 3, 10);
    QVERIFY(m.finish() == Map2XRasterArchiveMaker::TotalMismatch);

    QVERIFY(!QFile::exists(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeEmpty.map"))));
}

void Map2XRasterArchiveTest::makerUnderrun() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeUnderrun.map")));

    Map2XRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeUnderrun"), 3, 10);
    QVERIFY(m.append("1111") == Map2XRasterArchiveMaker::NextFile);
    QVERIFY(m.finish() == Map2XRasterArchiveMaker::TotalMismatch);

    /* Try to add after finishing */
    QVERIFY(m.append("2222") == Map2XRasterArchiveMaker::WriteError);
    QVERIFY(m.finish() == Map2XRasterArchiveMaker::WriteError);

    QFile f(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeUnderrun.map")));
    f.open(QFile::ReadOnly);
    QCOMPARE(f.readAll(), QByteArray(
        "MAP\x03"           "\x0a\x00\x00\x00"  "\x00\x00\x00\x00"
        "\x01\x00\x00\x00"  "1111"              "\x10\x00\x00\x00"
        "\x14\x00\x00\x00", 28));
}

void Map2XRasterArchiveTest::makerOverflow() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeOverflow.map")));

    Map2XRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeOverflow"), 3, 1);
    QVERIFY(m.append("1111") == Map2XRasterArchiveMaker::NextFile);
    QVERIFY(m.append("2222") == Map2XRasterArchiveMaker::TotalMismatch);
    QVERIFY(m.finish() == Map2XRasterArchiveMaker::Ok);

    QFile f(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeOverflow.map")));
    f.open(QFile::ReadOnly);
    QCOMPARE(f.readAll(), QByteArray(
        "MAP\x03"           "\x01\x00\x00\x00"  "\x00\x00\x00\x00"
        "\x01\x00\x00\x00"  "1111"              "\x10\x00\x00\x00"
        "\x14\x00\x00\x00", 28));
}

void Map2XRasterArchiveTest::makerSizeLimit() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit.map")));
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit-1.map")));

    Map2XRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit"), 3, 3, 36);
    QVERIFY(m.append("1111") == Map2XRasterArchiveMaker::NextFile);
    QVERIFY(m.append("2222") == Map2XRasterArchiveMaker::Ok);
    QVERIFY(m.append("3333") == Map2XRasterArchiveMaker::NextFile);
    QVERIFY(m.finish() == Map2XRasterArchiveMaker::Ok);

    QFile f(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit.map")));
    f.open(QFile::ReadOnly);
    QCOMPARE(f.readAll(), QByteArray(
        "MAP\x03"           "\x03\x00\x00\x00"  "\x00\x00\x00\x00"
        "\x02\x00\x00\x00"  "1111"              "2222"
        "\x10\x00\x00\x00"  "\x14\x00\x00\x00"  "\x18\x00\x00\x00", 36));

    QFile f1(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit-1.map")));
    f1.open(QFile::ReadOnly);
    QCOMPARE(f1.readAll(), QByteArray(
        "MAP\x03"           "\x03\x00\x00\x00"  "\x02\x00\x00\x00"
        "\x03\x00\x00\x00"  "3333"              "\x10\x00\x00\x00"
        "\x14\x00\x00\x00", 28));
}

}}}
