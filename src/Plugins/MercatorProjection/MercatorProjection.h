#ifndef Kompas_Plugins_MercatorProjection_h
#define Kompas_Plugins_MercatorProjection_h
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

/** @file
 * @brief Class Kompas::Plugins::MercatorProjection
 */

#include "AbstractProjection.h"

namespace Kompas { namespace Plugins {

/**
@brief Mercator projection

Customizable shift and stretch.
@section MercatorProjection_fromLatLon Converting from lat/lon coordinates
X coordinate is latitude, converted from range @f$ [ - \pi ; \pi ] @f$ to
@f$ [ 0 ; 1 ] @f$. Y coordinate is reprojected from longitude and converted to
range @f$ [ 0; 1 ] @f$ like this:
@f[
    y = \left(
        1 - \frac{\ln ( {\tan latitude} + {1 \over {\cos latitude}} )}{\pi}
    \right) \cdot {1 \over 2}
@f]
Last step is to apply shift and stretch.
@section MercatorProjection_toLatLon Converting to lat/lon coordinates
First is removed shift and stretch from both coordinates. Latitude is X
coordinate, converted from range @f$ [ 0 ; 1 ] @f$ to @f$ [ - \pi ; \pi ] @f$.
Longtitude is reprojected to Y coordinate and converted to range
@f$ [ - {\pi \over 2}; {\pi \over 2} ] @f$ like this:
@f[
    longitude = \arctan \left(
        {1 \over 2} \cdot (e^y - e^{-y})
    \right)
@f]
Coordinate calculation based on http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
 */
class CORE_EXPORT MercatorProjection: public Core::AbstractProjection {
    public:
        /** @copydoc Core::AbstractProjection::AbstractProjection */
        MercatorProjection(Corrade::PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = "");

        Core::Coords<double> fromLatLon(const Core::LatLonCoords& coords) const;
        Core::LatLonCoords toLatLon(const Core::Coords<double>& coords) const;
        std::vector<Core::LatLonCoords> seams() const;

        /**
         * @brief Set map stretch
         *
         * By default the map occupies whole area, so stretch is set to (1, 1).
         * If the map is smaller, set it to lower values.
         */
        inline void setStretch(const Core::Coords<double>& _stretch)
            { stretch = _stretch; }

        /**
         * @brief Set map shift
         *
         * By default the map touches left and top map border, so shift is set
         * to (0, 0). The values are in range 0 - 1 (portion of map size in
         * given direction).
         */
        inline void setShift(const Core::Coords<double>& _shift)
            { shift = _shift; }

    private:
        std::vector<Core::LatLonCoords> _seams;
        Core::Coords<double> stretch, shift;
};

}}

#endif
