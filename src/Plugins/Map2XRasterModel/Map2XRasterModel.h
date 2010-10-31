#ifndef Map2X_Plugins_Map2XRasterModel_h
#define Map2X_Plugins_Map2XRasterModel_h
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
 * @brief Class Map2X::Plugins::Map2XRasterModel
 */

#include "AbstractRasterModel.h"

namespace Map2X { namespace Plugins {

/**
 * @brief Map2X raster model
 *
 * Built-in format for storing offline maps.
 * @todo Document subclassing
 */
class Map2XRasterModel: public Core::AbstractRasterModel {
    public:
        inline Map2XRasterModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            AbstractRasterModel(manager, plugin), _zoomStep(2) {}

        inline virtual int features() const { return MultiplePackages|WriteableFormat|SequentialFormat; }
        inline virtual Core::TileSize tileSize() const { return _tileSize; }

        inline virtual double zoomStep() const { return _zoomStep; }
        inline virtual std::vector<Core::Zoom> zoomLevels() const { return _zoomLevels; }
        inline virtual Core::TileArea area() const { return _area; }
        inline virtual std::vector<std::string> layers() const { return _layers; }
        inline virtual std::vector<std::string> overlays() const { return _overlays; }

        virtual int addPackage(const std::string& filename);
        virtual std::string tileFromPackage(const std::string& layer, Core::Zoom z, const Map2X::Core::TileCoords& coords);

    protected:
        Core::TileSize _tileSize;
        double _zoomStep;
        std::vector<Core::Zoom> _zoomLevels;
        Core::TileArea _area;
        std::vector<std::string> _layers, _overlays;
};

}}

#endif
