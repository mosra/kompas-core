/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

    This file is part of Kompas.

    Kompas is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Kompas is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "Wgs84CoordsTest.h"

#include <QtTest/QTest>

#include "Wgs84Coords.h"

QTEST_APPLESS_MAIN(Kompas::Core::Test::Wgs84CoordsTest)
Q_DECLARE_METATYPE(Kompas::Core::Wgs84Coords)
Q_DECLARE_METATYPE(std::string)

using namespace std;

namespace Kompas { namespace Core { namespace Test {

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

void Wgs84CoordsTest::toString_data() {
    QTest::addColumn<Wgs84Coords>("coords");
    QTest::addColumn<int>("precision");
    QTest::addColumn<bool>("skipTrailingZeros");
    QTest::addColumn<QString>("output");

    QTest::newRow("precision0") << Wgs84Coords(0.00014166667, 0.00013611111) << 0 << false << "0°0'1\"N 0°0'0\"E";
    QTest::newRow("precision-1") << Wgs84Coords(0.0085, 0.0081666667) << -1 << false << "0°1'N 0°0'E";
    QTest::newRow("precision-2") << Wgs84Coords(0.51, 0.49) << -2 << false << "1°N 0°E";

    QTest::newRow("skipTrailingZeros") << Wgs84Coords(0, 0.75) << 5 << true << "0°N 0°45'E";
    QTest::newRow("skipTrailingZeros2") << Wgs84Coords(0.5, 0) << 5 << true << "0°30'N 0°E";
}

void Wgs84CoordsTest::toString() {
    QFETCH(Wgs84Coords, coords);
    QFETCH(int, precision);
    QFETCH(bool, skipTrailingZeros);
    QFETCH(QString, output);

    QCOMPARE(QString::fromStdString(coords.toString(precision, skipTrailingZeros)), output);
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

    string format =
        "Latitude: \n, Longtitude: \n\n deg. \n min. \n sec. \nNorth\nSouth\nEast\nWest";

    QCOMPARE(QString::fromStdString(c.toString(4, false, format)),
             QString("Latitude: 49 deg. 9 min. 33.1672 sec. North, Longtitude: 15 deg. 12 min. 4.7740 sec. East"));

    /* Zero and negative precision */
    QCOMPARE(QString::fromStdString(c.toString(0)),
             QString("49°9'33\"N 15°12'5\"E"));
    QCOMPARE(QString::fromStdString(c.toString(-1)),
             QString("49°10'N 15°12'E"));
    QCOMPARE(QString::fromStdString(c.toString(-2)),
             QString("49°N 15°E"));
}

void Wgs84CoordsTest::compare() {
    /* Two similar */
    QVERIFY(Wgs84Coords(15.7652, -120.2542) == Wgs84Coords(15.7652, -120.2542));

    /* Two another */
    QVERIFY(Wgs84Coords(23.23, 5.5) != Wgs84Coords(10.005, 11.0));

    /* Boundaries */
    QVERIFY(Wgs84Coords(0, 180) == Wgs84Coords(0, -180));

    /* Two invalid */
    QVERIFY(Wgs84Coords() == Wgs84Coords());

    /* Invalid and valid with the same value */
    QVERIFY(Wgs84Coords() != Wgs84Coords(0, 0));

    /* Tolerance */
    QVERIFY(Wgs84Coords(15.6999999901, 136.0000000099) == Wgs84Coords(15.7, 136.0));
    QVERIFY(Wgs84Coords(15.6999999901, 136.0000000099) != Wgs84Coords(15.70000001, 135.99999999));
}

void Wgs84CoordsTest::fromString_data() {
    QTest::addColumn<string>("input");
    QTest::addColumn<Wgs84Coords>("output");
    QTest::addColumn<string>("format");

    /* Valid */
    QTest::newRow("fullPrecision")
        << string("49°9'33.167\"N 15°12'4.774\"E")
        << Wgs84Coords(49.159213056, 15.201326111) << string();
    QTest::newRow("onlyNumbers")
        << string("49 9 33.167 S 15 12 4.774 W")
        << Wgs84Coords(-49.159213056, -15.201326111) << string();
    QTest::newRow("doubleValue")
        << string("49.1592131 N 15°12'4.774\"W")
        << Wgs84Coords(49.1592131, -15.201326111) << string();
    QTest::newRow("smallPrecision")
        << string("49 S 15 E")
        << Wgs84Coords(-49.0, 15.0) << string();
    QTest::newRow("configurationValue")
        << string("49.1592131 15.2013261")
        << Wgs84Coords(49.1592131, 15.2013261) << string();

    /* Period hell */
    QTest::newRow("periodHell")
        << string("... .1337. 49 0 .0102 N 25. .033 E")
        << Wgs84Coords(49.000002833, 25.00055) << string();

    /* Different format */
    QTest::newRow("czFormat")
        << string("49°9'33.167\" j.š. 15°12'4.774\" z.d.")
        << Wgs84Coords(-49.159213056, -15.201326111)
        << string("\n.š. \n.d.\n°\n'\n\"\ns\nj\nv\nz");

    /* Shortened format */
    QTest::newRow("shortened")
        << string("49°N 15°12'4.774\"E")
        << Wgs84Coords(49, 15.201326111) << string();
    QTest::newRow("shortened2")
        << string("49°9'33.167\"N 15°12'E")
        << Wgs84Coords(49.159213056, 15.2) << string();

    /* Invalid */
    QTest::newRow("noNumbers")
        << string("N E")
        << Wgs84Coords() << string();
    QTest::newRow("wrongNSEW")
        << string("49°9'33.167\"S 15°12'4.774\"V")
        << Wgs84Coords() << string();
    QTest::newRow("switchedNSEW")
        << string("15°12'4.774\"E 49°9'33.167\"N")
        << Wgs84Coords() << string();
}

void Wgs84CoordsTest::fromString() {
    QFETCH(string, input);
    QFETCH(Wgs84Coords, output);
    QFETCH(string, format);

    if(format.empty()) format = Wgs84Coords::format;

    Wgs84Coords actual(input, format);
    if(!actual.isValid()) {
        QVERIFY(actual.latitude() == 0);
        QVERIFY(actual.longtitude() == 0);
    }

    QVERIFY(actual == output);
}

void Wgs84CoordsTest::decimalToDms() {
    /* Test only one value, as underlying algorithm is tested extensively in toString() */
    QCOMPARE(QString::fromStdString(Wgs84Coords::decimalToDms(49.159213056)), QString("49°9'33.167\""));
}

}}}
