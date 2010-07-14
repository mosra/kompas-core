#ifndef Map2X_Core_TileArchiveReader_h
#define Map2X_Core_TileArchiveReader_h
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
 * @brief Class Map2X::Core::TileArchiveReader
 */

#include <fstream>

namespace Map2X { namespace Core {

/**
 * @brief Reader for tile archives
 *
 * Supports tile archive version 2. See also @ref TileArchiveFormat.
 * @todo Archive version 3
 * @todo Support for files > 4GB
 * @todo Creating from istream
 */
class TileArchiveReader {
    private:
        int _version;
        unsigned int _total,
            _begin,
            _end;
        bool _isValid;
        std::ifstream file;

    public:
        /**
         * @brief Whether the loaded archive is valid
         *
         * The archive is valid when has right signature and supported version.
         */
        inline bool isValid() const { return _isValid; }

        /** @brief Archive version */
        inline int version() const { return _version; }

        /** @brief Total count of tiles in all archives */
        inline unsigned int total() const { return _total; }

        /** @brief First tile in current archive */
        inline unsigned int begin() const { return _begin; }

        /** @brief (One tile after) last tile in current archive */
        inline unsigned int end() const { return _end; }

        /**
         * @brief Constructor
         * @param _file         Archive file
         *
         * Opens the archive file, checks file signature and version and gets
         * tile counts for the file. Success of this operation can be verified
         * with TileArchiveReader::isValid().
         */
        TileArchiveReader(const std::string& _file);

        /**
         * @brief Destructor
         *
         * Closes the archive file.
         */
        ~TileArchiveReader();

        /**
         * @brief Get tile from archive
         * @param tileNumber    Absolute tile number. Absolute means that the
         *      number is not relative to number of first tile in actual archive.
         *
         * Checks whether tile with that number exists in actual archive, if
         * yes, returns its data.
         */
        std::string get(unsigned int tileNumber);
};

}}

#endif
