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

#include "Wgs84CoordsTest.h"

#include <QtTest/QTest>

#include "Wgs84Coords.h"

QTEST_APPLESS_MAIN(Map2X::Core::Test::Wgs84CoordsTest)

namespace Map2X { namespace Core { namespace Test {

void Wgs84CoordsTest::construct_data() {
    QTest::addColumn<double>("lat");
    QTest::addColumn<double>("lon");
    QTest::addColumn<bool>("expectedValid");
    QTest::addColumn<QString>("toString");

    QTest::newRow("invalidPositive") << 90.000001 << 180.00001 << false << "";
    QTest::newRow("invalidNegative") << -90.000001 << -180.00001 << false << "";
    QTest::newRow("toString") << 49.1592131 << 15.2013261 << true << "49°9'33.167\"N 15°12'4.774\"E";
    QTest::newRow("negativeLat") << -10.0 << 100.0 << true << "10°0'0.000\"S 100°0'0.000\"E";
    QTest::newRow("negativeLon") << 15.0 << -150.0 << true << "15°0'0.000\"N 150°0'0.000\"W";
}

void Wgs84CoordsTest::construct() {
    QFETCH(double, lat);
    QFETCH(double, lon);
    QFETCH(bool, expectedValid);
    QFETCH(QString, toString);

    Wgs84Coords c(lat, lon);
    QVERIFY(c.isValid() == expectedValid);

    if(expectedValid) {
        QVERIFY(c.latitude() == lat);
        QVERIFY(c.longtitude() == lon);
    } else {
        QVERIFY(c.latitude() == 0);
        QVERIFY(c.longtitude() == 0);
    }

    QCOMPARE(QString::fromStdString(c.toString()), toString);
}

void Wgs84CoordsTest::stringFormat() {
    Wgs84Coords c(49.1592131, 15.2013261);

    std::string format =
        "Latitude: \n, Longtitude: \n\n deg. \n min. \n sec. \nNorth\nSouth\nEast\nWest";

    QCOMPARE(QString::fromStdString(c.toString(4, format)),
             QString("Latitude: 49 deg. 9 min. 33.1672 sec. North, Longtitude: 15 deg. 12 min. 4.7740 sec. East"));
}

void Wgs84CoordsTest::compare() {
    /* Two similar */
    QVERIFY(Wgs84Coords(15.7652, -120.2542) == Wgs84Coords(15.7652, -120.2542));

    /* Two another */
    QVERIFY(Wgs84Coords(23.23, 5.5) != Wgs84Coords(10.005, 11.0));

    /* Invalid and valid with the same value */
    QVERIFY(Wgs84Coords() != Wgs84Coords(0, 0));
}

}}}
