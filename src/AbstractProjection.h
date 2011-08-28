#ifndef Kompas_Core_AbstractProjection_h
#define Kompas_Core_AbstractProjection_h
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
 * @brief Class Kompas::Core::AbstractProjection
 */

#include "PluginManager/Plugin.h"
#include "Coords.h"
#include "LatLonCoords.h"

namespace Kompas { namespace Core {

/**
 * @brief Abstract class for different map projections
 *
 * Provides converting to and from lat/lon coordinates.
 */
class AbstractProjection: public PluginManager::Plugin {
    PLUGIN_INTERFACE("cz.mosra.Kompas.Core.AbstractProjection/0.2")

    public:
        /** @copydoc PluginManager::Plugin::Plugin */
        inline AbstractProjection(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            Plugin(manager, plugin) {}

        /**
         * @brief Get raster map coordinates from lat/lon coordinates
         * @param coords    Lat/lon coordinates
         * @return Raster map coordinates
         */
        virtual Coords<double> fromLatLon(const LatLonCoords& coords) const = 0;

        /**
         * @brief Get lat/lon coordinates from raster map coordinates
         * @param coords    Raster map coordinates
         * @return Lat/lon coordinates
         */
        virtual LatLonCoords toLatLon(const Coords<double>& coords) const = 0;

        /**
         * @brief List of map seams
         *
         * Because the map is projected from sphere to surface, it must be cut
         * somewhere to make it flat. Line of cut is called seam, on Earth maps
         * it is mostly between North America and Asia or around hemispheres on
         * stereographic maps. Each three @ref LatLonCoords define one seam.
         * This data are then used for seamless rendering in e.g. 3D map view or
         * when rendering vector maps with given projection.
         */
        virtual std::vector<LatLonCoords> seams() const { return std::vector<LatLonCoords>(); }
};

}}

#endif
