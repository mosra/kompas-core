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

#include "AbstractRasterModelTest.h"

#include <QtTest/QTest>

#include "AbstractRasterModel.h"

typedef Kompas::Core::Coords<unsigned int> UCoords;

QTEST_APPLESS_MAIN(Kompas::Core::Test::AbstractRasterModelTest)
Q_DECLARE_METATYPE(UCoords)

namespace Kompas { namespace Core { namespace Test {

void AbstractRasterModelTest::tilesInArea_data() {
    QTest::addColumn<UCoords>("tileArea");
    QTest::addColumn<UCoords>("expected");

    QTest::newRow("empty/minimal") << UCoords(0, 1) << UCoords(0, 1);
    QTest::newRow("minimal/empty") << UCoords(1, 0) << UCoords(1, 0);
    QTest::newRow("exact+1/2") << UCoords(257,130) << UCoords(2, 3);
    QTest::newRow("exact+2/1") << UCoords(258,129) << UCoords(3, 2);
    QTest::newRow("more+2") << UCoords(5*256+2, 3*128+2) << UCoords(7, 5);
}

void AbstractRasterModelTest::tilesInArea() {
    QFETCH(UCoords, tileArea);
    QFETCH(UCoords, expected);
    QVERIFY(model.tilesInArea(tileArea) == expected);
}

}}}
