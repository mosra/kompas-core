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

#include "AbstractRasterModel.h"
#include "AbstractCache.h"

using namespace std;

namespace Kompas { namespace Core {

bool AbstractRasterModel::setOnline(bool enabled) {
    if(!enabled || !(features() & LoadableFromUrl))
        _online = false;
    else
        _online = true;

    return _online;
}

string AbstractRasterModel::tileFromCache(const string& layer, Zoom z, const TileCoords& coords) {
    /* No cache set */
    if(!_cache) return "";
    return _cache->rasterTile(plugin(), layer, z, coords);
}

bool AbstractRasterModel::tileToCache(const std::string& layer, Zoom z, const Kompas::Core::TileCoords& coords, const std::string& data) {
    /* No cache set */
    if(!_cache) return false;
    return _cache->setRasterTile(plugin(), layer, z, coords, data);
}

}}
