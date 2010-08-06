#ifndef Map2X_Core_Test_DummyMercatorTileModel_h
#define Map2X_Core_Test_DummyMercatorTileModel_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

    This file is part of Map2X.

    Map2X is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License version 3
    only, as published by the Free Software Foundation.

    Map2X is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU Lesser General Public License version 3 for more details.
*/

#include "AbstractMercatorTileModel.h"

namespace Map2X { namespace Core { namespace Test {

class DummyMercatorTileModel: public AbstractMercatorTileModel {
    public:
        DummyMercatorTileModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            AbstractMercatorTileModel(manager, plugin) {}

        inline virtual TileArea area() const { return TileArea(0, 0, 1, 1); }
        inline virtual std::string copyright() const { return ""; }
        inline virtual std::vector<std::string> layers() const { return std::vector<std::string>(); }
        inline virtual std::vector< std::string > overlays() const { return std::vector<std::string>(); }
        inline virtual TileSize tileSize() const { return TileSize(256, 256); }
        inline virtual std::vector<Zoom> zoomLevels() const { return std::vector<Zoom>(); }
};

}}}

#endif
