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

#include "AbstractRasterModel.h"

using namespace std;

namespace Map2X { namespace Core {

int AbstractRasterModel::addPackage(const string& packageDir) {
    /** @todo Implement! */
    return -1;
}

bool AbstractRasterModel::setOnline(bool enabled) {
    if(!enabled || !(features() & LoadableFromUrl))
        _online = false;
    else
        _online = true;

    return _online;
}

bool AbstractRasterModel::setCache(const string& cacheDir) {
    /** @todo Implement! */
    return false;
}

string AbstractRasterModel::tileData(const string& layer, Zoom z, const TileCoords& coords) {
    /** @todo Implement! */
    return "";
}

}}
