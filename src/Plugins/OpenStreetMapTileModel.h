#ifndef Map2X_Plugins_OpenStreetMapTileModel_h
#define Map2X_Plugins_OpenStreetMapTileModel_h
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
 * @brief Class Map2X::Plugins::OpenStreetMapTileModel
 */

#include "AbstractMercatorTileModel.h"

namespace Map2X { namespace Core { namespace Plugins {

/** @brief OpenStreetMap tile model */
class OpenStreetMapTileModel: public Core::AbstractMercatorTileModel {
    public:
        inline virtual int features() const
            { return AbstractMercatorTileModel::features()|LoadableFromUrl; }
        inline virtual Core::TileSize tileSize() const
            { return Core::TileSize(256,256); }
        inline virtual std::string copyright() const
            { return "© OpenStreetMap and contributors, CC-BY-SA."; }

        virtual std::vector<Core::Zoom> zoomLevels() const;
        virtual Core::TileArea area() const;
        virtual std::vector<std::string> layers() const;
        virtual std::vector<std::string> overlays() const;

        virtual std::string tileUrl(std::string layer, Core::Zoom z, const Map2X::Core::TileCoords& coords) const;
};

}}}

PLUGIN_REGISTER_STATIC(OpenStreetMap, Map2X::Core::Plugins::OpenStreetMapTileModel,
                       "cz.mosra.Map2X.Core.AbstractTileModel/0.1");
PLUGIN_SET_NAME("OpenStreetMap");
PLUGIN_FINISH

#endif
