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

#include "AbstractCache.h"

#include "Utility/Endianness.h"

using namespace std;
using namespace Kompas::Utility;

namespace Kompas { namespace Core {

string AbstractCache::key(const string& model, const string& layer, Zoom z, TileCoords coords) const {
    z = Endianness::littleEndian<Zoom>(z);
    coords.x = Endianness::littleEndian<unsigned int>(coords.x);
    coords.y = Endianness::littleEndian<unsigned int>(coords.y);

    return RasterTile +
        string(reinterpret_cast<const char*>(&z), sizeof(Zoom)) +
        string(reinterpret_cast<const char*>(&coords), sizeof(TileCoords)) +
        model +
        layer;
}

}}
