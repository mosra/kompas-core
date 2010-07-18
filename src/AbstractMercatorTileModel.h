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
        inline virtual int features() const { return ConvertableCoords; }
        RasterCoords fromWgs84(Zoom z, const Map2X::Core::Wgs84Coords& coords) const;
        Wgs84Coords toWgs84(Zoom z, const Map2X::Core::RasterCoords& coords) const;

    protected:
        /**
         * @brief Coordinate shifting
         *
         * By default, in zoom 0 the map is only one tile containing whole world
         * from -180°W to 180°E and -85.05113°S to 85.05113°N. If the map
         * doesn't comply with these values, reimplement this function to move
         * coordinate origin from left top corner. Value is portion of size of
         * the map at zoom 0, varying from -1 to 1. If the value is positive, it
         * means WGS84 coordinate origin will be moved move right / down on the
         * map, negative moves it left / up.
         */
        inline virtual Coords<double> coordinateShift() const { return Coords<double>(0, 0); }

        /**
         * @brief Coordinate stretching
         *
         * If the map at zoom 0 is not precisely containing the whole world in
         * one tile as described in coordinateShift(), reimplement this function
         * to stretch the coordinates on the map. Values greater than one will
         * enlarge WGS84 coordinates out of the map bounds, so the map will
         * occupy only portion of whole range, values lower than one will make
         * the coordinate system smaller than the map.
         */
        inline virtual Coords<double> coordinateStretch() const { return Coords<double>(1, 1); }
};

}}

#endif
