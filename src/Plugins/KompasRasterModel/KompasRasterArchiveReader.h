#ifndef Kompas_Plugins_KompasRasterArchiveReader_h
#define Kompas_Plugins_KompasRasterArchiveReader_h
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

/** @file
 * @brief Class Kompas::Plugins::KompasRasterArchiveReader
 */

#include <fstream>

namespace Kompas { namespace Plugins {

/**
 * @brief Reader for tile archives
 *
 * Supports tile archive version 2 and 3. See also @ref KompasRasterArchive.
 * @todo Support for files > 4GB
 * @todo Creating from istream
 */
class KompasRasterArchiveReader {
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
         * with KompasRasterArchiveReader::isValid().
         */
        KompasRasterArchiveReader(const std::string& _file);

        /**
         * @brief Destructor
         *
         * Closes the archive file.
         */
        ~KompasRasterArchiveReader();

        /**
         * @brief Get tile from archive
         * @param tileNumber    Absolute tile number. Absolute means that the
         *      number is not relative to number of first tile in actual archive.
         *
         * Checks whether tile with that number exists in actual archive, if
         * yes, returns its data.
         */
        std::string get(unsigned int tileNumber);

    private:
        int _version;
        unsigned int _total,
            _begin,
            _end,
            positions;
        bool _isValid;
        std::ifstream file;

        unsigned int (*endianator)(unsigned int);
};

}}

#endif
