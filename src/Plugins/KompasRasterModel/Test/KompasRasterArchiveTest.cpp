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

#include "KompasRasterArchiveTest.h"

#include <string>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtTest/QTest>

#include "Utility/Directory.h"
#include "KompasRasterModel/KompasRasterArchiveReader.h"
#include "KompasRasterModel/KompasRasterArchiveMaker.h"

#include "testConfigure.h"

QTEST_APPLESS_MAIN(Kompas::Plugins::Test::KompasRasterArchiveTest)

using namespace std;
using namespace Kompas::Utility;

namespace Kompas { namespace Plugins { namespace Test {

KompasRasterArchiveTest::KompasRasterArchiveTest(QObject* parent): QObject(parent) {
    QDir dir;
    dir.mkpath(RASTERARCHIVE_WRITE_TEST_DIR);
}

void KompasRasterArchiveTest::reader2() {
    KompasRasterArchiveReader r(Directory::join(RASTERARCHIVE_TEST_DIR, "version2.kps"));

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

void KompasRasterArchiveTest::reader3() {
    KompasRasterArchiveReader r(Directory::join(RASTERARCHIVE_TEST_DIR, "version3.kps"));

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

void KompasRasterArchiveTest::maker2() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make2.kps")));

    KompasRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make2"), 2, 10);
    QVERIFY(m.finish() == KompasRasterArchiveMaker::VersionError);

    QVERIFY(!QFile::exists(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make2.kps"))));
}

void KompasRasterArchiveTest::maker() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make.kps")));

    KompasRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make"), 3, 3);
    QCOMPARE(m.currentFileNumber(), -1);
    QCOMPARE(m.currentFileSize(), 0u);

    QVERIFY(m.append("1111") == KompasRasterArchiveMaker::NextFile);
    QCOMPARE(m.currentFileNumber(), 0);
    QCOMPARE(m.currentFileSize(), 28u);

    QVERIFY(m.append("") == KompasRasterArchiveMaker::Ok);
    QVERIFY(m.append("3333") == KompasRasterArchiveMaker::Ok);
    QVERIFY(m.finish() == KompasRasterArchiveMaker::Ok);
    QCOMPARE(m.currentFileNumber(), 0);
    QCOMPARE(m.currentFileSize(), 0u);
    QCOMPARE(m.currentFileTileCount(), 0u);

    QFile f(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "make.kps")));
    f.open(QFile::ReadOnly);
    QCOMPARE(f.readAll(), QByteArray(
        "MAP\x03"           "\x03\x00\x00\x00"  "\x00\x00\x00\x00"
        "\x03\x00\x00\x00"  "1111"              "3333"
        "\x10\x00\x00\x00"  "\x14\x00\x00\x00"  "\x14\x00\x00\x00"
        "\x18\x00\x00\x00", 40));
}

void KompasRasterArchiveTest::makerEmpty() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeEmpty.kps")));

    KompasRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeEmpty"), 3, 10);
    QVERIFY(m.finish() == KompasRasterArchiveMaker::TotalMismatch);

    QVERIFY(!QFile::exists(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeEmpty.kps"))));
}

void KompasRasterArchiveTest::makerUnderrun() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeUnderrun.kps")));

    KompasRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeUnderrun"), 3, 10);
    QVERIFY(m.append("1111") == KompasRasterArchiveMaker::NextFile);
    QVERIFY(m.finish() == KompasRasterArchiveMaker::TotalMismatch);

    /* Try to add after finishing */
    QVERIFY(m.append("2222") == KompasRasterArchiveMaker::WriteError);
    QVERIFY(m.finish() == KompasRasterArchiveMaker::WriteError);

    QFile f(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeUnderrun.kps")));
    f.open(QFile::ReadOnly);
    QCOMPARE(f.readAll(), QByteArray(
        "MAP\x03"           "\x0a\x00\x00\x00"  "\x00\x00\x00\x00"
        "\x01\x00\x00\x00"  "1111"              "\x10\x00\x00\x00"
        "\x14\x00\x00\x00", 28));
}

void KompasRasterArchiveTest::makerOverflow() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeOverflow.kps")));

    KompasRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeOverflow"), 3, 1);
    QVERIFY(m.append("1111") == KompasRasterArchiveMaker::NextFile);
    QVERIFY(m.append("2222") == KompasRasterArchiveMaker::TotalMismatch);
    QVERIFY(m.finish() == KompasRasterArchiveMaker::Ok);

    QFile f(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeOverflow.kps")));
    f.open(QFile::ReadOnly);
    QCOMPARE(f.readAll(), QByteArray(
        "MAP\x03"           "\x01\x00\x00\x00"  "\x00\x00\x00\x00"
        "\x01\x00\x00\x00"  "1111"              "\x10\x00\x00\x00"
        "\x14\x00\x00\x00", 28));
}

void KompasRasterArchiveTest::makerSizeLimit() {
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit.kps")));
    QFile::remove(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit-1.kps")));

    KompasRasterArchiveMaker m(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit"), 3, 3, 36);
    QVERIFY(m.append("1111") == KompasRasterArchiveMaker::NextFile);
    QVERIFY(m.append("2222") == KompasRasterArchiveMaker::Ok);
    QVERIFY(m.append("3333") == KompasRasterArchiveMaker::NextFile);
    QVERIFY(m.finish() == KompasRasterArchiveMaker::Ok);

    QFile f(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit.kps")));
    f.open(QFile::ReadOnly);
    QCOMPARE(f.readAll(), QByteArray(
        "MAP\x03"           "\x03\x00\x00\x00"  "\x00\x00\x00\x00"
        "\x02\x00\x00\x00"  "1111"              "2222"
        "\x10\x00\x00\x00"  "\x14\x00\x00\x00"  "\x18\x00\x00\x00", 36));

    QFile f1(QString::fromStdString(Directory::join(RASTERARCHIVE_WRITE_TEST_DIR, "makeSizeLimit-1.kps")));
    f1.open(QFile::ReadOnly);
    QCOMPARE(f1.readAll(), QByteArray(
        "MAP\x03"           "\x03\x00\x00\x00"  "\x02\x00\x00\x00"
        "\x03\x00\x00\x00"  "3333"              "\x10\x00\x00\x00"
        "\x14\x00\x00\x00", 28));
}

}}}
