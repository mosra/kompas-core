/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>
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

#include "EarthCelestialBodyTest.h"

#include <cmath>
#include <QtTest/QTest>

#include "EarthCelestialBody/EarthCelestialBody.h"

QTEST_APPLESS_MAIN(Kompas::Core::Test::EarthCelestialBodyTest)
Q_DECLARE_METATYPE(Kompas::Core::LatLonCoords)

using namespace Kompas::Plugins;
using namespace std;

namespace Kompas { namespace Core { namespace Test {

void EarthCelestialBodyTest::distance_data() {
    QTest::addColumn<LatLonCoords>("a");
    QTest::addColumn<LatLonCoords>("b");
    QTest::addColumn<double>("distance");

    LatLonCoords greenwich(0.0,0.0);
    LatLonCoords prague(50.08333, 14.46667);
    LatLonCoords ny(40.7142691, -74.0059729);
    LatLonCoords sydney(-33.88333, 151.2167);

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

void EarthCelestialBodyTest::distance() {
    QFETCH(LatLonCoords, a);
    QFETCH(LatLonCoords, b);
    QFETCH(double, distance);

    EarthCelestialBody body;

    double distance1 = body.distance(a, b);
    double distance2 = body.distance(b, a);

    QVERIFY(abs(distance - distance1) < 0.001);
    QVERIFY(abs(distance - distance2) < 0.001);
}

}}}
