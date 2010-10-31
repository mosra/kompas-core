#ifndef Map2X_Plugins_OpenStreetMapRasterModel_h
#define Map2X_Plugins_OpenStreetMapRasterModel_h
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
 * @brief Class Map2X::Plugins::OpenStreetMapRasterModel
 */

#include "Map2XRasterModel/Map2XRasterModel.h"
#include "MercatorProjection/MercatorProjection.h"

namespace Map2X { namespace Plugins {

/**
 * @brief OpenStreetMap raster model
 *
 * Based on: http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
 */
class OpenStreetMapRasterModel: public Map2XRasterModel {
    public:
        inline OpenStreetMapRasterModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            Map2XRasterModel(manager, plugin) {}

        inline virtual int features() const {
            return Map2XRasterModel::features()|LoadableFromUrl|NonConvertableFormat;
        }
        inline virtual const Core::AbstractProjection* projection() const
            { return &_projection; }
        inline virtual Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline virtual std::string copyright() const
            { return "© OpenStreetMap and contributors, CC-BY-SA."; }

        virtual std::vector<Core::Zoom> zoomLevels() const;
        virtual Core::TileArea area() const;
        virtual std::vector<std::string> layers() const;

        virtual std::string tileUrl(const std::string& layer, Core::Zoom z, const Map2X::Core::TileCoords& coords) const;

    private:
        MercatorProjection _projection;
};

}}

#endif
