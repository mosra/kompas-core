#ifndef Kompas_Core_AbstractProjection_h
#define Kompas_Core_AbstractProjection_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

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
#include "Wgs84Coords.h"

namespace Kompas { namespace Core {

/**
 * @brief Abstract class for different map projections
 *
 * Provides converting to and from WGS84 coordinates.
 */
class AbstractProjection: public PluginManager::Plugin {
    PLUGIN_INTERFACE("cz.mosra.Kompas.Core.AbstractProjection/0.1")

    public:
        /** @copydoc PluginManager::Plugin::Plugin */
        inline AbstractProjection(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            Plugin(manager, plugin) {}

        /**
         * @brief Get raster map coordinates from WGS84 coordinates
         * @param coords    WGS84 coordinates
         * @return Raster map coordinates
         */
        virtual Coords<double> fromWgs84(const Wgs84Coords& coords) const = 0;

        /**
         * @brief Get WGS84 coordinates from raster map coordinates
         * @param coords    Raster map coordinates
         * @return WGS84 coordinates
         */
        virtual Wgs84Coords toWgs84(const Coords<double>& coords) const = 0;
};

}}

#endif
