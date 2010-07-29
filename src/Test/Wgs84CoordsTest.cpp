/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

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
Q_DECLARE_METATYPE(Map2X::Core::Wgs84Coords)

namespace Map2X { namespace Core { namespace Test {

void Wgs84CoordsTest::construct_data() {
    QTest::addColumn<double>("lat");
    QTest::addColumn<double>("lon");
    QTest::addColumn<bool>("expectedValid");
    QTest::addColumn<QString>("toString");

    QTest::newRow("invalidPositive") << 90.000001 << 180.00001 << false << "0°0'0.000\"N 0°0'0.000\"E";
    QTest::newRow("invalidNegative") << -90.000001 << -180.00001 << false << "0°0'0.000\"N 0°0'0.000\"E";
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

void Wgs84CoordsTest::distance_data() {
    QTest::addColumn<Wgs84Coords>("a");
    QTest::addColumn<Wgs84Coords>("b");
    QTest::addColumn<double>("distance");

    Wgs84Coords greenwich(0.0,0.0);
    Wgs84Coords prague(50.08333, 14.46667);
    Wgs84Coords ny(40.7142691, -74.0059729);
    Wgs84Coords sydney(-33.88333, 151.2167);

    QTest::newRow("Greenwich-Greenwich")
        << greenwich << greenwich << 0.0;
    QTest::newRow("Greenwich-NY")
        << greenwich << ny << 8667450.978;
    QTest::newRow("Greenwich-Sydney")
        << greenwich << sydney << 15209514.325;
    QTest::newRow("Greenwich-Prague")
        << greenwich << prague << 5718439.631;
    QTest::newRow("NY-Sydney")
        << ny << sydney << 15988059.978; // online is 977
    QTest::newRow("Prague-NY")
        << prague << ny << 6591583.293;
    QTest::newRow("Prague-Sydney")
        << prague << sydney << 16079006.106; // online is 107
}

void Wgs84CoordsTest::distance() {
    QFETCH(Wgs84Coords, a);
    QFETCH(Wgs84Coords, b);
    QFETCH(double, distance);

    double distance1 = Wgs84Coords::distance(a, b, 0.001);
    double distance2 = Wgs84Coords::distance(b, a, 0.001);

    QCOMPARE(distance, distance1);
    QCOMPARE(distance, distance2);
}

void Wgs84CoordsTest::stringFormat() {
    Wgs84Coords c(49.1592131, 15.2013261);

    std::string format =
        "Latitude: \n, Longtitude: \n\n deg. \n min. \n sec. \nNorth\nSouth\nEast\nWest";

    QCOMPARE(QString::fromStdString(c.toString(4, format)),
             QString("Latitude: 49 deg. 9 min. 33.1672 sec. North, Longtitude: 15 deg. 12 min. 4.7740 sec. East"));

    /* Zero and negative precision */
    QCOMPARE(QString::fromStdString(c.toString(0)),
             QString("49°9'33\"N 15°12'5\"E"));
    QCOMPARE(QString::fromStdString(c.toString(-1)),
             QString("49°9'N 15°12'E"));
    QCOMPARE(QString::fromStdString(c.toString(-2)),
             QString("49°N 15°E"));
}

void Wgs84CoordsTest::compare() {
    /* Two similar */
    QVERIFY(Wgs84Coords(15.7652, -120.2542) == Wgs84Coords(15.7652, -120.2542));

    /* Two another */
    QVERIFY(Wgs84Coords(23.23, 5.5) != Wgs84Coords(10.005, 11.0));

    /* Two invalid */
    QVERIFY(Wgs84Coords() == Wgs84Coords());

    /* Invalid and valid with the same value */
    QVERIFY(Wgs84Coords() != Wgs84Coords(0, 0));

    /* Tolerance */
    QVERIFY(Wgs84Coords(15.6999999901, 136.0000000099) == Wgs84Coords(15.7, 136.0));
    QVERIFY(Wgs84Coords(15.6999999901, 136.0000000099) != Wgs84Coords(15.70000001, 135.99999999));
}

}}}
