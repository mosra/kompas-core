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

#include "OpenStreetMapTileModel.h"

#include <sstream>

using namespace std;
using namespace Map2X::Core;

PLUGIN_REGISTER_STATIC(OpenStreetMapTileModel,
                       Map2X::Plugins::OpenStreetMapTileModel,
                       "cz.mosra.Map2X.Core.AbstractTileModel/0.1")

namespace Map2X { namespace Plugins {

vector<Zoom> OpenStreetMapTileModel::zoomLevels() const {
    /** @todo Implement for packages */
    vector<Zoom> z;
    for(Zoom i = 0; i != 19; ++i)
        z.push_back(i);
    return z;
}

TileArea OpenStreetMapTileModel::area() const {
    /** @todo Implement for packages */
    /* For zoom 0 */
    return TileArea(0, 0, 1, 1);
}

vector<string> OpenStreetMapTileModel::layers() const {
    vector<string> l;
    l.push_back("Mapnik");
    l.push_back("Osmarender");
    l.push_back("Cycle");
    return l;
}

vector<string> OpenStreetMapTileModel::overlays() const {
    return vector<string>();
}

string OpenStreetMapTileModel::tileUrl(const std::string& layer, Zoom z, const TileCoords& coords) const {
    if(z > 18) return "";

    ostringstream url;
    if(layer == "Mapnik")
        url << "http://tile.openstreetmap.org/";
    else if(layer == "Osmarender")
        url << "http://tah.openstreetmap.org/Tiles/tile/";
    else if(layer == "Cycle")
        url << "http://andy.sandbox.cloudmade.com/tiles/cycle/";
    else return "";

    url << z << "/" << coords.x << "/" << coords.y << ".png";

    return url.str();
}

}}
