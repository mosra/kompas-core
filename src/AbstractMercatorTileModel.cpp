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

#include "AbstractMercatorTileModel.h"

#include <cmath>

namespace Map2X { namespace Core {

RasterCoords AbstractMercatorTileModel::fromWgs84(Zoom z, const Map2X::Core::Wgs84Coords& coords) const {
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
    x = (x+coordinateShift().x)*coordinateStretch().x;
    y = (y+coordinateShift().y)*coordinateStretch().y;

    /* Apply coordinates to map area size at given zoom */
    x *= pow(zoomStep(), z);
    y *= pow(zoomStep(), z);

    return RasterCoords(
        static_cast<int>(x),
        static_cast<int>(y),
        (x-floor(x))*tileSize().x,
        (y-floor(y))*tileSize().y
    );
}

Wgs84Coords AbstractMercatorTileModel::toWgs84(Zoom z, const Map2X::Core::RasterCoords& coords) const {
    /* Add pixel coordinates to Tile coordinates */
    double lon = coords.x() + coords.moveX()/tileSize().x;
    double lat = coords.y() + coords.moveY()/tileSize().y;

    /* Transform coordinates to zoom 1 */
    lon /= pow(zoomStep(), z);
    lat /= pow(zoomStep(), z);

    /* Substract shift and stretch */
    lon = lon/coordinateStretch().x - coordinateShift().x;
    lat = lat/coordinateStretch().y - coordinateShift().y;

    /* Reset latitude range, unshift origin */
    lat = (1 - 2*lat)*PI;

    /* Reproject mercator projection to latitude in degrees */
    lat = 180/PI * atan(0.5 * (exp(lat) - exp(-lat)));

    /* Shift origin to center, transform 0 - 1 range to  -180° - +180° */
    lon = (2*lon - 1)*180;

    return Wgs84Coords(lat, lon);
}

}}
