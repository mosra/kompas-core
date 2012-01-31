#ifndef Kompas_Plugins_OpenStreetMapRasterModel_h
#define Kompas_Plugins_OpenStreetMapRasterModel_h
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
 * @brief Class Kompas::Plugins::OpenStreetMapRasterModel
 */

#include "KompasRasterModel/KompasRasterModel.h"
#include "MercatorProjection/MercatorProjection.h"

namespace Kompas { namespace Plugins {

/**
 * @brief OpenStreetMap raster model
 *
 * Based on: http://wiki.openstreetmap.org/wiki/Slippy_map_tilenames
 */
class CORE_EXPORT OpenStreetMapRasterModel: public KompasRasterModel {
    public:
        /** @copydoc Plugins::KompasRasterModel::KompasRasterModel */
        OpenStreetMapRasterModel(Corrade::PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = "");

        inline int features() const {
            return KompasRasterModel::features()|MultipleFileFormat|LoadableFromUrl|NonConvertableFormat|ConvertableCoords;
        }
        inline const Core::AbstractProjection* projection() const
            { return &_projection; }
        inline std::string celestialBody() const
            { return "EarthCelestialBody"; }
        inline Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline std::string copyright() const
            { return "© OpenStreetMap and contributors, CC-BY-SA."; }

        inline std::set<Core::Zoom> zoomLevels() const {
            return online() ? zoomLevelsOnline : KompasRasterModel::zoomLevels();
        }
        inline Core::TileArea area() const {
            return online() ? areaOnline : KompasRasterModel::area();
        }
        std::vector<std::string> layers() const {
            return online() ? layersOnline : KompasRasterModel::layers();
        }

        std::string tileUrl(const std::string& layer, Core::Zoom z, const Kompas::Core::TileCoords& coords) const;

    private:
        MercatorProjection _projection;

        std::set<Core::Zoom> zoomLevelsOnline;
        Core::TileArea areaOnline;
        std::vector<std::string> layersOnline;
};

}}

#endif
