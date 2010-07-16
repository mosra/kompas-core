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

#include "OpenStreetMapTileModelTest.h"

#include <QtTest/QTest>
#include <QtCore/QDebug>

#include "Plugins/OpenStreetMapTileModel.h"
#include "Wgs84Coords.h"

QTEST_APPLESS_MAIN(Map2X::Core::Plugins::Test::OpenStreetMapTileModelTest)

namespace Map2X { namespace Core { namespace Plugins { namespace Test {

void OpenStreetMapTileModelTest::coords_data() {
    QTest::addColumn<double>("lat");
    QTest::addColumn<double>("lon");
    QTest::addColumn<Zoom>("z");

    QTest::newRow("Grenwich") << 0. << 0. << (Zoom) 1;
    QTest::newRow("Grenwich zoom 10") << 0. << 0. << (Zoom) 10;
    QTest::newRow("Prague") << 50.08333 << 14.46667 << (Zoom) 1;
    QTest::newRow("Prague zoom 25") << 50.08333 << 14.46667 << (Zoom) 25;
    QTest::newRow("New York") << 40.7142691 << -74.0059729 << (Zoom) 1;
    QTest::newRow("New York zoom 3") << 40.7142691 << -74.0059729 << (Zoom) 3;
    QTest::newRow("Sydney") << 	-33.88333 << 151.2167 << (Zoom) 3;
    QTest::newRow("Sydney zoom 2") << 	-33.88333 << 151.2167 << (Zoom) 3;
}

void OpenStreetMapTileModelTest::coords() {
    QFETCH(double, lat);
    QFETCH(double, lon);
    QFETCH(Zoom, z);

    Wgs84Coords c1(lat, lon);
    Wgs84Coords c2 = TileModel.toWgs84(z, TileModel.fromWgs84(z, c1));

	qDebug() << c1.latitude() << c1.longtitude();
	qDebug() << c2.latitude() << c2.longtitude();
    QCOMPARE(c1, c2);
}

}}}}
