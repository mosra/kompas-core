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

#include "CoordsTest.h"

#include <QtTest/QTest>

#include "basictypes.h"
#include "Utility/ConfigurationGroup.h"

typedef Map2X::Core::Coords<unsigned int> UCoords;

QTEST_APPLESS_MAIN(Map2X::Core::Test::CoordsTest)
Q_DECLARE_METATYPE(UCoords)

using namespace std;
using namespace Map2X::Core;
using namespace Map2X::Utility;

namespace Map2X { namespace Core { namespace Test {

void CoordsTest::fromString_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<UCoords>("expected");
    QTest::addColumn<int>("flags");

    QTest::newRow("empty") << "" << UCoords() << 0;
    QTest::newRow("half") << "20" << UCoords(20, 0) << 0;
    QTest::newRow("garbage") << "ghsk5246dddaiu8663jh" << UCoords() << 0;
    QTest::newRow("halfGarbage") << "cd 30" << UCoords(0, 30) << 0;
    QTest::newRow("regular") << "1 2" << UCoords(1, 2) << 0;
    QTest::newRow("hex") << "d0 0d" << UCoords(0xD0, 0x0D) << (int) ConfigurationGroup::Hex;
}

void CoordsTest::fromString() {
    QFETCH(QString, input);
    QFETCH(UCoords, expected);
    QFETCH(int, flags);

    UCoords actual = ConfigurationValue<UCoords>::fromString(input.toStdString(), flags);

    QVERIFY(actual == expected);
}

void CoordsTest::toString_data() {
    QTest::addColumn<UCoords>("input");
    QTest::addColumn<QString>("expected");
    QTest::addColumn<int>("flags");

    QTest::newRow("regular") << UCoords(1, 2) << "1 2" << 0;
    QTest::newRow("hex") << UCoords(0xD0, 0x0D) << "d0 d" << (int) ConfigurationGroup::Hex;
}

void CoordsTest::toString() {
    QFETCH(UCoords, input);
    QFETCH(QString, expected);
    QFETCH(int, flags);

    string actual = ConfigurationValue<UCoords>::toString(input, flags);

    QCOMPARE(QString::fromStdString(actual), expected);
}

}}}
