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

#include "LatLonCoordsTest.h"

#include <QtTest/QTest>

#include "LatLonCoords.h"

QTEST_APPLESS_MAIN(Kompas::Core::Test::LatLonCoordsTest)
Q_DECLARE_METATYPE(Kompas::Core::LatLonCoords)
Q_DECLARE_METATYPE(std::string)

using namespace std;
using namespace Corrade::Utility;

namespace Kompas { namespace Core { namespace Test {

void LatLonCoordsTest::construct_data() {
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

void LatLonCoordsTest::construct() {
    QFETCH(double, lat);
    QFETCH(double, lon);
    QFETCH(bool, expectedValid);
    QFETCH(QString, toString);

    LatLonCoords c(lat, lon);
    QVERIFY(c.isValid() == expectedValid);

    if(expectedValid) {
        QVERIFY(c.latitude() == lat);
        QVERIFY(c.longitude() == lon);
    } else {
        QVERIFY(c.latitude() == 0);
        QVERIFY(c.longitude() == 0);
    }

    QCOMPARE(QString::fromStdString(c.toString()), toString);
}

void LatLonCoordsTest::toString_data() {
    QTest::addColumn<LatLonCoords>("coords");
    QTest::addColumn<int>("precision");
    QTest::addColumn<bool>("skipTrailingZeros");
    QTest::addColumn<QString>("output");

    QTest::newRow("precision0") << LatLonCoords(0.00014166667, 0.00013611111) << 0 << false << "0°0'1\"N 0°0'0\"E";
    QTest::newRow("precision-1") << LatLonCoords(0.0085, 0.0081666667) << -1 << false << "0°1'N 0°0'E";
    QTest::newRow("precision-2") << LatLonCoords(0.51, 0.49) << -2 << false << "1°N 0°E";

    QTest::newRow("skipTrailingZeros") << LatLonCoords(0, 0.75) << 5 << true << "0°N 0°45'E";
    QTest::newRow("skipTrailingZeros2") << LatLonCoords(0.5, 0) << 5 << true << "0°30'N 0°E";
}

void LatLonCoordsTest::toString() {
    QFETCH(LatLonCoords, coords);
    QFETCH(int, precision);
    QFETCH(bool, skipTrailingZeros);
    QFETCH(QString, output);

    QCOMPARE(QString::fromStdString(coords.toString(precision, skipTrailingZeros)), output);
}

void LatLonCoordsTest::stringFormat() {
    LatLonCoords c(49.1592131, 15.2013261);

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

void LatLonCoordsTest::compare() {
    /* Two similar */
    QVERIFY(LatLonCoords(15.7652, -120.2542) == LatLonCoords(15.7652, -120.2542));

    /* Two another */
    QVERIFY(LatLonCoords(23.23, 5.5) != LatLonCoords(10.005, 11.0));

    /* Boundaries */
    QVERIFY(LatLonCoords(0, 180) == LatLonCoords(0, -180));

    /* Two invalid */
    QVERIFY(LatLonCoords() == LatLonCoords());

    /* Invalid and valid with the same value */
    QVERIFY(LatLonCoords() != LatLonCoords(0, 0));

    /* Tolerance */
    QVERIFY(LatLonCoords(15.6999999901, 136.0000000099) == LatLonCoords(15.7, 136.0));
    QVERIFY(LatLonCoords(15.6999999901, 136.0000000099) != LatLonCoords(15.70000001, 135.99999999));
}

void LatLonCoordsTest::fromString_data() {
    QTest::addColumn<string>("input");
    QTest::addColumn<LatLonCoords>("output");
    QTest::addColumn<string>("format");

    /* Valid */
    QTest::newRow("fullPrecision")
        << string("49°9'33.167\"N 15°12'4.774\"E")
        << LatLonCoords(49.159213056, 15.201326111) << string();
    QTest::newRow("onlyNumbers")
        << string("49 9 33.167 S 15 12 4.774 W")
        << LatLonCoords(-49.159213056, -15.201326111) << string();
    QTest::newRow("doubleValue")
        << string("49.1592131 N 15°12'4.774\"W")
        << LatLonCoords(49.1592131, -15.201326111) << string();
    QTest::newRow("smallPrecision")
        << string("49 S 15 E")
        << LatLonCoords(-49.0, 15.0) << string();
    QTest::newRow("configurationValue")
        << string("49.1592131 15.2013261")
        << LatLonCoords(49.1592131, 15.2013261) << string();

    /* Period hell */
    QTest::newRow("periodHell")
        << string("... .1337. 49 0 .0102 N 25. .033 E")
        << LatLonCoords(49.000002833, 25.00055) << string();

    /* Different format */
    QTest::newRow("czFormat")
        << string("49°9'33.167\" j.š. 15°12'4.774\" z.d.")
        << LatLonCoords(-49.159213056, -15.201326111)
        << string("\n.š. \n.d.\n°\n'\n\"\ns\nj\nv\nz");

    /* Shortened format */
    QTest::newRow("shortened")
        << string("49°N 15°12'4.774\"E")
        << LatLonCoords(49, 15.201326111) << string();
    QTest::newRow("shortened2")
        << string("49°9'33.167\"N 15°12'E")
        << LatLonCoords(49.159213056, 15.2) << string();

    /* Invalid */
    QTest::newRow("noNumbers")
        << string("N E")
        << LatLonCoords() << string();
    QTest::newRow("wrongNSEW")
        << string("49°9'33.167\"S 15°12'4.774\"V")
        << LatLonCoords() << string();
    QTest::newRow("switchedNSEW")
        << string("15°12'4.774\"E 49°9'33.167\"N")
        << LatLonCoords() << string();
}

void LatLonCoordsTest::fromString() {
    QFETCH(string, input);
    QFETCH(LatLonCoords, output);
    QFETCH(string, format);

    if(format.empty()) format = LatLonCoords::format;

    LatLonCoords actual(input, format);
    if(!actual.isValid()) {
        QVERIFY(actual.latitude() == 0);
        QVERIFY(actual.longitude() == 0);
    }

    QVERIFY(actual == output);
}

void LatLonCoordsTest::pointOnSphere_data() {
    QTest::addColumn<LatLonCoords>("coords");

    QTest::newRow("prague") << LatLonCoords(50.08333, 14.46667);
    QTest::newRow("ny") << LatLonCoords(40.7142691, -74.0059729);
    QTest::newRow("sydney") << LatLonCoords(-33.88333, 151.2167);
}

void LatLonCoordsTest::pointOnSphere() {
    QFETCH(LatLonCoords, coords);
    double x, y, z;
    coords.toPointOnSphere(&x, &y, &z);
    QVERIFY(LatLonCoords::fromPointOnSphere(x, y, z) == coords);
}

void LatLonCoordsTest::debug() {
    ostringstream o;
    Debug(&o) << LatLonCoords(49.159, -15.201);
    QCOMPARE(QString::fromStdString(o.str()), QString("LatLonCoords(49.159, -15.201)\n"));
}

void LatLonCoordsTest::decimalToDms() {
    /* Test only one value, as underlying algorithm is tested extensively in toString() */
    QCOMPARE(QString::fromStdString(LatLonCoords::decimalToDms(49.159213056)), QString("49°9'33.167\""));
}

}}}
