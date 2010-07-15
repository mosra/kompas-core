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

#include "AbstractTileModel.h"

namespace Map2X { namespace Core {

std::string AbstractTileModel::attribute(AbstractTileModel::Attribute type, int package) {
    /** @todo Implement! */
    return "";
}

int AbstractTileModel::addPackage(std::string packageDir) {
    /** @todo Implement! */
    return -1;
}

bool AbstractTileModel::setOnline(bool enabled) {
    if(!enabled) enabledOnline = false;
    else {
        if(!(features() & LoadableFromUrl)) enabledOnline = false;
        else enabledOnline = true;
    }

    return enabledOnline;
}

bool AbstractTileModel::setCache(std::string cacheDir) {
    /** @todo Implement! */
    return false;
}

std::string AbstractTileModel::tileData(std::string layer, Zoom z, const Map2X::Core::TileCoords& coords) const {
    /** @todo Implement! */
    return "";
}

}}
