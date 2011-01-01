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

#include "AbsoluteAreaTest.h"

#include <QtTest/QTest>

#include "AbsoluteArea.h"
#include "Utility/ConfigurationGroup.h"

typedef Kompas::Core::AbsoluteArea<unsigned int> UAbsoluteArea;

QTEST_APPLESS_MAIN(Kompas::Core::Test::AbsoluteAreaTest)
Q_DECLARE_METATYPE(UAbsoluteArea)

using namespace std;
using namespace Kompas::Core;
using namespace Kompas::Utility;

namespace Kompas { namespace Core { namespace Test {

void AbsoluteAreaTest::operators() {
    QVERIFY(UAbsoluteArea(1,2,3,4) == UAbsoluteArea(1,2,3,4));

    QVERIFY(UAbsoluteArea(1,2,3,4)*2 == UAbsoluteArea(2,4,6,8));
    QVERIFY(UAbsoluteArea(2,4,6,8)/2 == UAbsoluteArea(1,2,3,4));

    QVERIFY(UAbsoluteArea(1,2,3,4)*Coords<unsigned int>(2,5) == UAbsoluteArea(2,10,6,20));
    QVERIFY(UAbsoluteArea(2,10,6,20)/Coords<unsigned int>(2,5) == UAbsoluteArea(1,2,3,4));
}

void AbsoluteAreaTest::fromString_data() {
    QTest::addColumn<QString>("input");
    QTest::addColumn<UAbsoluteArea>("expected");
    QTest::addColumn<int>("flags");

    QTest::newRow("empty") << "" << UAbsoluteArea() << 0;
    QTest::newRow("half") << "20 30" << UAbsoluteArea(20, 30, 0, 0) << 0;
    QTest::newRow("garbage") << "ghsk5246dddaiu8663jh" << UAbsoluteArea() << 0;
    QTest::newRow("halfGarbage") << "20 ab cd 30" << UAbsoluteArea(20, 0, 0, 30) << 0;
    QTest::newRow("regular") << "1 2 3 4" << UAbsoluteArea(1, 2, 3, 4) << 0;
    QTest::newRow("hex") << "de ad be ef" << UAbsoluteArea(0xDE, 0xAD, 0xBE, 0xEF) << (int) ConfigurationGroup::Hex;
}

void AbsoluteAreaTest::fromString() {
    QFETCH(QString, input);
    QFETCH(UAbsoluteArea, expected);
    QFETCH(int, flags);

    UAbsoluteArea actual = ConfigurationValue<UAbsoluteArea>::fromString(input.toStdString(), flags);

    QVERIFY(actual == expected);
}

void AbsoluteAreaTest::toString_data() {
    QTest::addColumn<UAbsoluteArea>("input");
    QTest::addColumn<QString>("expected");
    QTest::addColumn<int>("flags");

    QTest::newRow("regular") << UAbsoluteArea(1, 2, 3, 4) << "1 2 3 4" << 0;
    QTest::newRow("hex") << UAbsoluteArea(0xDE, 0xAD, 0xBE, 0xEF) << "de ad be ef" << (int) ConfigurationGroup::Hex;
}

void AbsoluteAreaTest::toString() {
    QFETCH(UAbsoluteArea, input);
    QFETCH(QString, expected);
    QFETCH(int, flags);

    string actual = ConfigurationValue<UAbsoluteArea>::toString(input, flags);

    QCOMPARE(QString::fromStdString(actual), expected);
}

void AbsoluteAreaTest::debug() {
    ostringstream o;
    Debug(&o) << UAbsoluteArea(1, 2, 3, 4);
    QCOMPARE(QString::fromStdString(o.str()), QString("AbsoluteArea(1, 2, 3, 4)\n"));
}

}}}
