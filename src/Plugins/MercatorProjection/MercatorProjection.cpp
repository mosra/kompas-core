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

#include "MercatorProjection.h"

#include <cmath>

using namespace Map2X::Core;

PLUGIN_REGISTER_STATIC(MercatorProjection,
    Map2X::Plugins::MercatorProjection,
    "cz.mosra.Map2X.Core.AbstractProjection/0.1")

namespace Map2X { namespace Plugins {

Coords<double> MercatorProjection::fromWgs84(const Wgs84Coords& coords) const {
    /* Transform longtitude range from -180° - +180° to 0 - 1 and shift origin
        to left top corner */
    double x = (1 + coords.longtitude()/180)/2;

    /* Convert latitude to radians */
    double y = coords.latitude()*PI/180;

    /* Reproject latitude to Mercator projection */
    y = log(tan(y) + 1/cos(y));

    /* Transform latitude range, shift origin */
    y = (1 - y/PI)/2;

    /* Apply shift and stretch */
    x = (x+_coordinateShift.x)*_coordinateStretch.x;
    y = (y+_coordinateShift.y)*_coordinateStretch.y;

    /* Transform coordinates to world map size */
    return Coords<double>(x, y);
}

Wgs84Coords MercatorProjection::toWgs84(const Coords<double>& coords) const {
    /* Substract shift and stretch */
    double lon = coords.x/_coordinateStretch.x - _coordinateShift.x;
    double lat = coords.y/_coordinateStretch.y - _coordinateShift.y;

    /* Reset latitude range, unshift origin */
    lat = (1 - 2*lat)*PI;

    /* Reproject mercator projection to latitude in degrees */
    lat = 180/PI * atan(0.5 * (exp(lat) - exp(-lat)));

    /* Shift origin to center, transform 0 - 1 range to  -180° - +180° */
    lon = (2*lon - 1)*180;

    return Wgs84Coords(lat, lon);
}

}}
