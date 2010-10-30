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

#include "AbsoluteAreaTest.h"

#include <QtTest/QTest>

#include "basictypes.h"
#include "Utility/ConfigurationGroup.h"

typedef Map2X::Core::AbsoluteArea<unsigned int> UAbsoluteArea;

QTEST_APPLESS_MAIN(Map2X::Core::Test::AbsoluteAreaTest)
Q_DECLARE_METATYPE(UAbsoluteArea)

using namespace std;
using namespace Map2X::Core;
using namespace Map2X::Utility;

namespace Map2X { namespace Core { namespace Test {

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

}}}
