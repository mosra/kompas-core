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

#include "MercatorProjection.h"

#include <cmath>

#include "constants.h"

using namespace std;
using namespace Kompas::Core;

PLUGIN_REGISTER(MercatorProjection, Kompas::Plugins::MercatorProjection,
                "cz.mosra.Kompas.Core.AbstractProjection/0.2")

namespace Kompas { namespace Plugins {

MercatorProjection::MercatorProjection(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& plugin): AbstractProjection(manager, plugin), stretch(Coords<double>(1, 1)), shift(Coords<double>(0, 0)) {
    _seams.reserve(3);
    _seams.push_back(LatLonCoords(60, 180));
    _seams.push_back(LatLonCoords(0, 180));
    _seams.push_back(LatLonCoords(-60, 180));
}

Coords<double> MercatorProjection::fromLatLon(const LatLonCoords& coords) const {
    /* Transform longitude range from -180° - +180° to 0 - 1 and shift origin
        to left top corner */
    double x = (1 + coords.longitude()/180)/2;

    /* Convert latitude to radians */
    double latitude = coords.latitude()*PI/180;

    /* Reproject latitude to Mercator projection, transform to range 0 - 1 */
    double y = (1 - log(tan(latitude) + 1/cos(latitude))/PI)/2;

    /* Apply shift and stretch */
    x = x*stretch.x + shift.x;
    y = y*stretch.y + shift.y;

    /* Transform coordinates to world map size */
    return Coords<double>(x, y);
}

LatLonCoords MercatorProjection::toLatLon(const Coords<double>& coords) const {
    Coords<double> _coords = coords;

    /* Remove shift and stretch */
    _coords.x = _coords.x/stretch.x - shift.x;
    _coords.y = _coords.y/stretch.y - shift.y;

    /* Convert latitude range */
    _coords.y = (1 - 2*_coords.y)*PI;

    /* Reproject Mercator projection to latitude, transform to -90 - 90° */
    double latitude = atan(0.5 * (exp(_coords.y) - exp(-_coords.y)))*180/PI;

    /* Convert longitude range from 0 - 1 to  -180° - 180° */
    double longitude = (2*_coords.x - 1)*180;

    return LatLonCoords(latitude, longitude);
}

vector<LatLonCoords> MercatorProjection::seams() const {
    if(stretch == Coords<double>(1, 1) && shift == Coords<double>(0, 0))
        return _seams;
    else return AbstractProjection::seams();
}

}}
