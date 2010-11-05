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

#include "Map2XRasterArchiveReader.h"

#include "Utility/Endianness.h"

using namespace std;
using Map2X::Utility::Endianness;

namespace Map2X { namespace Plugins {

Map2XRasterArchiveReader::Map2XRasterArchiveReader(const string& _file): _version(0), _total(0), _begin(0), _end(0), _isValid(false) {
    file.open(_file.c_str(), fstream::binary);
    if(!file.is_open()) return;

    /* Four bytes - enough for all integers even for signature MAP with null
        terminator at the end */
    char* buffer = new char[4];

    /* Check file signature */
    file.get(buffer, 4);
    if(string(buffer) != "MAP") return;

    /* Check file version (only version 2 is currently supported) */
    file.read(buffer, 1);
    _version = buffer[0];

    if(_version != 2 && _version != 3) return;

    /* Version 2 is in big endian, version 3 in little endian */
    if(_version == 2)
        endianator = Endianness::bigEndian<unsigned int>;
    else if(_version == 3)
        endianator = Endianness::littleEndian<unsigned int>;

    /* Total count of tiles */
    file.read(buffer, 4);
    _total = endianator(*reinterpret_cast<unsigned int*>(buffer));

    /* Beginning tile */
    file.read(buffer, 4);
    _begin = endianator(*reinterpret_cast<unsigned int*>(buffer));

    /* (One item after) ending tile */
    file.read(buffer, 4);
    _end = endianator(*reinterpret_cast<unsigned int*>(buffer));

    /* Version 2 has positions array after header */
    if(_version == 2)
        positions = 16;

    /* Version 3 has it at the end of the file */
    else if(_version == 3) {
        /* Beginning of positions array is saved in last 4 bytes of the file */
        file.seekg(-4, ios::end);
        file.read(buffer, 4);
        positions = Endianness::littleEndian(*reinterpret_cast<unsigned int*>(buffer));
    }

    _isValid = true;

    /** @todo Pedantic validity checks: begin < end <= total, whether EOF
        position leads really to EOF etc? */
}

Map2XRasterArchiveReader::~Map2XRasterArchiveReader() {
    if(file.is_open()) file.close();
}

string Map2XRasterArchiveReader::get(unsigned int tileNumber) {
    /* If the archive is invalid, file is not ready or tileNumber is out of bounds, return empty data */
    if(!isValid() || !file.good() || tileNumber < begin() || tileNumber >= end()) return "";

    char* buffer = new char[4];
    unsigned int position, size;

    /* Seek to information about position of the tile. Tile number is passed
        as absolute, so we must make it relative to this file. */
    file.seekg(positions+4*(tileNumber-begin()));

    /* Position of tile data */
    file.read(buffer, 4);
    position = endianator(*reinterpret_cast<unsigned int*>(buffer));

    /* Size of tile data */
    file.read(buffer, 4);
    size = endianator(*reinterpret_cast<unsigned int*>(buffer))-position;

    /* Prepare buffer for tile size (including null terminator) */
    delete[] buffer;
    buffer = new char[size+1];
    buffer[size] = '\0';

    /* Seek to tile position and get the data */
    file.seekg(position);
    file.read(buffer, size);

    string ret(buffer);
    delete[] buffer;
    return ret;

    return "";
}

}}
