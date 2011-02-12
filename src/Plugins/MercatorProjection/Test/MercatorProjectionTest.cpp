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

#include "MercatorProjectionTest.h"

#include <QtTest/QTest>
#include <QtCore/QDebug>

#include "LatLonCoords.h"

Q_DECLARE_METATYPE(Kompas::Core::LatLonCoords)
QTEST_APPLESS_MAIN(Kompas::Plugins::Test::MercatorProjectionTest)

using namespace Kompas::Core;

namespace Kompas { namespace Plugins { namespace Test {

void MercatorProjectionTest::coords_data() {
    QTest::addColumn<LatLonCoords>("coords");

    QTest::newRow("Greenwich")
        << LatLonCoords(0.0, 0.0);
    QTest::newRow("Prague")
        << LatLonCoords(50.08333, 14.46667);
    QTest::newRow("New York")
        << LatLonCoords(40.7142691, -74.0059729);
    QTest::newRow("Sydney")
        << LatLonCoords(-33.88333, 151.2167);
}

void MercatorProjectionTest::coords() {
    QFETCH(LatLonCoords, coords);
    QVERIFY(projection.toLatLon(projection.fromLatLon(coords)) == coords);
}

}}}
