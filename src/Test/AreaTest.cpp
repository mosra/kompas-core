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

#include "AreaTest.h"

#include <QtTest/QTest>

#include "Area.h"
#include "Utility/ConfigurationGroup.h"

typedef Kompas::Core::Area<unsigned int, unsigned int> UArea;

QTEST_APPLESS_MAIN(Kompas::Core::Test::AreaTest)
Q_DECLARE_METATYPE(UArea)

using namespace std;
using namespace Kompas::Core;
using namespace Kompas::Utility;

namespace Kompas { namespace Core { namespace Test {

void AreaTest::operators() {
    QVERIFY(UArea(1,2,3,4) == UArea(1,2,3,4));

    QVERIFY(UArea(1,2,3,4)*2 == UArea(2,4,6,8));
    QVERIFY(UArea(2,4,6,8)/2 == UArea(1,2,3,4));

    QVERIFY(UArea(1,2,3,4)*Coords<unsigned int>(2,5) == UArea(2,10,6,20));
    QVERIFY(UArea(2,10,6,20)/Coords<unsigned int>(2,5) == UArea(1,2,3,4));
}

void AreaTest::fromString_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<UArea>("expected");
    QTest::addColumn<int>("flags");

    QTest::newRow("empty") << "" << UArea() << 0;
    QTest::newRow("half") << "20 30" << UArea(20, 30, 0, 0) << 0;
    QTest::newRow("garbage") << "ghsk5246dddaiu8663jh" << UArea() << 0;
    QTest::newRow("halfGarbage") << "20 ab cd 30" << UArea(20, 0, 0, 30) << 0;
    QTest::newRow("regular") << "1 2 3 4" << UArea(1, 2, 3, 4) << 0;
    QTest::newRow("hex") << "de ad be ef" << UArea(0xDE, 0xAD, 0xBE, 0xEF) << (int) ConfigurationGroup::Hex;
}

void AreaTest::fromString() {
    QFETCH(QString, input);
    QFETCH(UArea, expected);
    QFETCH(int, flags);

    UArea actual = ConfigurationValue<UArea>::fromString(input.toStdString(), flags);

    QVERIFY(actual == expected);
}

void AreaTest::toString_data() {
    QTest::addColumn<UArea>("input");
    QTest::addColumn<QString>("expected");
    QTest::addColumn<int>("flags");

    QTest::newRow("regular") << UArea(1, 2, 3, 4) << "1 2 3 4" << 0;
    QTest::newRow("hex") << UArea(0xDE, 0xAD, 0xBE, 0xEF) << "de ad be ef" << (int) ConfigurationGroup::Hex;
}

void AreaTest::toString() {
    QFETCH(UArea, input);
    QFETCH(QString, expected);
    QFETCH(int, flags);

    string actual = ConfigurationValue<UArea>::toString(input, flags);

    QCOMPARE(QString::fromStdString(actual), expected);
}

}}}
