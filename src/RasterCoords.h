#ifndef Map2X_Core_RasterCoords_h
#define Map2X_Core_RasterCoords_h
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
 * @brief Class Map2X::Core::RasterCoords
 */

namespace Map2X { namespace Core {

/**
 * @brief Coordinates on tile maps
 *
 * Stores coordinates of point on tile map. @c X and @c Y is tile position
 * relative to left top corner, @c moveX and @c moveY are coordinates of pixel
 * on the tile. The class doesn't do any checks whether the coordinates are
 * in valid range, it's up to class which creates and uses them.
 */
class RasterCoords {
    public:
        /** @brief X coordinate of the tile */
        inline unsigned int x() const { return _x; }

        /** @brief Y coordinate of the tile */
        inline unsigned int y() const { return _y; }

        /** @brief X coordinate of pixel on the tile */
        inline unsigned int moveX() const { return _moveX; }

        /** @brief Y coordinate of pixel on the tile */
        inline unsigned int moveY() const { return _moveY; }

        /** @brief Whether the coordinates are valid */
        inline bool isValid() const { return _isValid; }

        /**
         * @brief Implicit constructor
         *
         * Creates invalid coordinates.
         */
        inline RasterCoords():
            _x(0), _y(0), _moveX(0), _moveY(0), _isValid(false) {}

        /**
         * @brief Constructor
         * @param __x       X coordinate of the tile
         * @param __y       Y coordinate of the tile
         * @param __moveX   X coordinate of pixel on the tile
         * @param __moveY   Y coordinate of pixel on the tile
         *
         * Creates valid coordinates (always).
         */
        inline RasterCoords(unsigned int __x, unsigned int __y, unsigned int __moveX, unsigned int __moveY):
            _x(__x), _y(__y), _moveX(__moveX), _moveY(__moveY), _isValid(true) {}

    private:
        unsigned int _x, _y, _moveX, _moveY;
        bool _isValid;
};

}}

#endif
