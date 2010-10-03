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

#include "MercatorProjectionTest.h"

#include <cmath>
#include <QtTest/QTest>
#include <QtCore/QDebug>

#include "Wgs84Coords.h"

Q_DECLARE_METATYPE(Map2X::Core::Wgs84Coords)
QTEST_APPLESS_MAIN(Map2X::Plugins::Test::MercatorProjectionTest)

using namespace Map2X::Core;

namespace Map2X { namespace Plugins { namespace Test {

void MercatorProjectionTest::coords_data() {
    QTest::addColumn<Wgs84Coords>("coords");

    QTest::newRow("Greenwich")
        << Wgs84Coords(0.0, 0.0);
    QTest::newRow("Prague")
        << Wgs84Coords(50.08333, 14.46667);
    QTest::newRow("New York")
        << Wgs84Coords(40.7142691, -74.0059729);
    QTest::newRow("Sydney")
        << Wgs84Coords(-33.88333, 151.2167);
}

void MercatorProjectionTest::coords() {
    QFETCH(Wgs84Coords, coords);
    QVERIFY(projection.toWgs84(projection.fromWgs84(coords)) == coords);
}

}}}
