#ifndef Map2X_Core_AbstractMercatorTileModel_h
#define Map2X_Core_AbstractMercatorTileModel_h
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

/** @file
 * @brief Class Map2X::Core::AbstractMercatorTileModel
 */

#include "AbstractTileModel.h"

namespace Map2X { namespace Core {

/**
 * @brief Abstract class for maps with Mercator projection
 *
 * Coordinate calculation based on http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
 */
class AbstractMercatorTileModel: public AbstractTileModel {
    public:
        AbstractMercatorTileModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = 0):
            AbstractTileModel(manager, plugin) {}

        inline virtual int features() const { return ConvertableCoords; }
        RasterCoords fromWgs84(Zoom z, const Map2X::Core::Wgs84Coords& coords) const;
        Wgs84Coords toWgs84(Zoom z, const Map2X::Core::RasterCoords& coords) const;
};

}}

#endif
