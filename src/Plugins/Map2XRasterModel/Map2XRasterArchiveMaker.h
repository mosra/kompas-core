#ifndef Map2X_Plugins_Map2XRasterArchiveMaker_h
#define Map2X_Plugins_Map2XRasterArchiveMaker_h
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
 * @brief Class Map2X::Plugins::Map2XRasterArchiveMaker
 */

#include <fstream>
#include <sstream>

namespace Map2X { namespace Plugins {

/**
 * @brief Class for creating %Map2X raster archives
 */
class Map2XRasterArchiveMaker {
    public:
        /**
         * @brief Saving state
         * @see append(), finish()
         */
        enum State {
            /**
             * Unsupported archive version.
             */
            VersionError = -4,

            /**
             * File cannot be created or opened for writing.
             */
            FileError = -3,

            /**
             * Write error (maximum system filesize limit exceeded, full disk
             * etc.)
             */
            WriteError = -2,

            /**
             * Count of all writed tiles and total count of tiles (specified in
             * constructor) doesn't match. Returned when trying to append()
             * another tile to archive while total count was already reached or
             * when calling finish() with less added tiles than total count.
             * This error doesn't exactly mean the package will be left in
             * unusable state - append() will simply ignore tiles after reaching
             * total count and finish() creates package which will look like
             * last archive part is missing.
             */
            TotalMismatch = -1,

            /**
             * Everything went OK.
             */
            Ok = 0,

            /**
             * Filesize limit (specified in constructor) was reached and next
             * archive part was opened for current tile.
             */
            NextFile = 1
        };

        /**
         * @brief Constructor
         * @param _filePrefix   File prefix (with path). For example, if
         *      file prefix is set to @c package/base/17, archives will be saved
         *      to @c package/base/17.map, @c package/base/17-1.map etc.
         * @param _version      Archive version (currently version 3 is supported)
         * @param _total        Total count of all tiles in all archive parts
         * @param _sizeLimit    Size limit of the archive (default is 2 GB).
         */
        inline Map2XRasterArchiveMaker(const std::string& _filePrefix, unsigned int _version, unsigned int _total, unsigned int _sizeLimit = 0x7FFFFFFF): version(_version), total(_total), sizeLimit(_sizeLimit), currentBegin(0), currentEnd(0), currentNumber(-1), filePrefix(_filePrefix) {}

        /**
         * @brief Destructor
         *
         * Calls finish().
         */
        ~Map2XRasterArchiveMaker() { finish(); }

        /**
         * @brief Append tile to archive
         * @param data          Tile data
         * @return Saving state
         */
        State append(const std::string& data);

        /**
         * @brief Finish package
         * @return Saving state
         *
         * Writes remaining data to files and finishes the package.
         */
        State finish();

        /**
         * @brief Number of current file
         * @return -1 if no file is currently opened (no tile was added yet), 0
         *      for the first file and so on.
         */
        inline int currentFileNumber() const { return currentNumber; }

        /**
         * @brief Size of current file
         * @return Size of current file as if it has full
         *      positions header appended or 0 if no file is currently opened.
         */
        unsigned int currentFileSize();

        /** @brief Count of tiles in current file */
        inline unsigned int currentFileTileCount() const {
            return currentEnd - currentBegin;
        }

        /** @brief Count of all added tiles */
        inline unsigned int tileCount() const {
            return currentEnd;
        }

    private:
        unsigned int version,
            total,
            sizeLimit,
            currentBegin,
            currentEnd;
        int currentNumber;
        std::string filePrefix;

        std::ofstream file;
        std::ostringstream positions;

        bool finishCurrentFile();
};

}}

#endif
