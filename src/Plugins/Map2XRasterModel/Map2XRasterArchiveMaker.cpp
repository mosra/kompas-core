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

#include "Map2XRasterArchiveMaker.h"

#include "Utility/Endianness.h"

using namespace std;
using namespace Map2X::Utility;

namespace Map2X { namespace Plugins {

Map2XRasterArchiveMaker::State Map2XRasterArchiveMaker::append(const std::string& data) {
    if(version != 3) return VersionError;
    if(currentEnd == total) return TotalMismatch;

    /* Package is already finished, return WriteError */
    if(currentNumber != -1 && file.tellp() == -1) return WriteError;

    unsigned int buffer;
    State state = Ok;

    /* Open first file or next file, if size limit has been reached */
    if(currentNumber == -1 || static_cast<unsigned int>(file.tellp()) + data.size() + (currentEnd-currentBegin+1)*4 >= sizeLimit) {
        if(currentNumber++ != -1) finishCurrentFile();

        /* Open next file */
        ostringstream filename;
        filename << filePrefix;
        if(currentNumber != 0) filename << "-" << currentNumber;
        filename << ".map";
        file.open(filename.str().c_str());

        if(!file.good()) return FileError;

        /* Write header */
        file.write("MAP", 3);
        file.write("\3", 1);
        buffer = Endianness::littleEndian(total);
        file.write(reinterpret_cast<const char*>(&buffer), 4);
        buffer = Endianness::littleEndian(currentBegin);
        file.write(reinterpret_cast<const char*>(&buffer), 4);
        file.write("\0\0\0\0", 4); /* Placeholder for number of (one item after) the last tile in the file */

        if(!file.good()) return WriteError;

        state = NextFile;
    }

    /* Add position of current tile to positions array */
    buffer = Endianness::littleEndian(static_cast<unsigned int>(file.tellp()));
    positions.write(reinterpret_cast<const char*>(&buffer), 4);

    /* Write tile data */
    file.write(data.c_str(), data.size());
    currentEnd++;

    if(!file.good()) return WriteError;

    return state;
}

bool Map2XRasterArchiveMaker::finishCurrentFile() {
    unsigned int buffer = Endianness::littleEndian(static_cast<unsigned int>(file.tellp()));

    /* Add position after last tile to positions array */
    positions.write(reinterpret_cast<const char*>(&buffer), 4);

    /* Write positions array at the end of file */
    file.write(positions.str().c_str(), static_cast<unsigned int>(positions.tellp()));

    /* Write number of (one item after) the last tile in the file */
    file.seekp(12);
    buffer = Endianness::littleEndian(currentEnd);
    file.write(reinterpret_cast<const char*>(&buffer), 4);

    bool ok = true;
    if(!file.good()) ok = false;

    /* Close file and clear positions array */
    file.close();
    positions.str("");

    /* Set current begin to end of now closed file */
    currentBegin = currentEnd;

    return ok;
}

Map2XRasterArchiveMaker::State Map2XRasterArchiveMaker::finish() {
    if(version != 3) return VersionError;

    /* Package is already finished, return WriteError */
    if(currentNumber != -1 && file.tellp() == -1) return WriteError;

    State state = Ok;
    if(total != currentEnd) state = TotalMismatch;
    if(currentNumber != -1 && !finishCurrentFile())
        state = WriteError;

    return state;
}

unsigned int Map2XRasterArchiveMaker::currentFileSize() {
    /* No file currently opened */
    if(file.tellp() == -1) return 0;

    return static_cast<unsigned int>(file.tellp()) + static_cast<unsigned int>(positions.tellp()) + 4;
}

}}
