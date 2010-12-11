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

#include "KompasRasterArchiveReader.h"

#include "Utility/Endianness.h"
#include "Utility/Debug.h"

using namespace std;
using namespace Kompas::Utility;

namespace Kompas { namespace Plugins {

KompasRasterArchiveReader::KompasRasterArchiveReader(const string& _file): _version(0), _total(0), _begin(0), _end(0), _isValid(false) {
    file.open(_file.c_str(), fstream::binary);
    if(!file.is_open()) {
        Error() << "Cannot open Kompas archive file" << _file;
        return;
    }

    /* Four bytes - enough for all integers even for signature MAP with null
        terminator at the end */
    char* buffer = new char[4];

    /* Check file signature (\0 is automagically appended at the end) */
    file.get(buffer, 4);
    if(string(buffer) != "MAP") {
        Error() << "Unknown Kompas archive signature" << buffer << "in" << _file;
        return;
    }

    /* Check file version (only version 2 is currently supported) */
    file.read(buffer, 1);
    _version = buffer[0];

    if(_version != 2 && _version != 3) {
        Error() << "Unsupported Kompas archive version" << _version << "in" << _file;
        return;
    }

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

    /* Check whether begin < end <= total */
    if(_begin >= _end || _end > _total) {
        Error() << "Kompas archive range mismatch, total:" << _total << "begin:" << _begin << "end:" << _end << "in" << _file;
        return;
    }

    /* Version 2 has positions array after header */
    if(_version == 2)
        positions = 16;

    /* Version 3 has it at the end of the file */
    else if(_version == 3) {
        /* Beginning of positions array is saved in last 4 bytes of the file */
        file.seekg(-4, ios::end);
        file.read(buffer, 4);
        positions = Endianness::littleEndian(*reinterpret_cast<unsigned int*>(buffer));

        if(positions+(_end-_begin+1)*4 != file.tellg()) {
            Error() << "Kompas archive tile positions array has unexpected size, expected" << (_end-_begin+1)*4 << "found" << static_cast<unsigned int>(file.tellg()) - positions << "in" << _file;
            return;
        }
    }

    _isValid = true;
}

KompasRasterArchiveReader::~KompasRasterArchiveReader() {
    if(file.is_open()) file.close();
}

string KompasRasterArchiveReader::get(unsigned int tileNumber) {
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

    /* Prepare buffer for tile size */
    delete[] buffer;
    buffer = new char[size];

    /* Seek to tile position and get the data */
    file.seekg(position);
    file.read(buffer, size);

    string ret(buffer, size);
    delete[] buffer;
    return ret;

    return "";
}

}}
