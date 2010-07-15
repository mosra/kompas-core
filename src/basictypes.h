#ifndef Map2X_Core_basictypes_h
#define Map2X_Core_basictypes_h
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
 * @brief Basic template types
 */

namespace Map2X { namespace Core {

/** @brief Pi */
#define PI 3.1415926535

/** @brief Coordinates */
template <class PositionType> struct Coords {
    PositionType x,     /**< @brief X value */
        y;              /**< @brief Y value */

    /**
     * @brief Constructor
     * @param _x    X value
     * @param _y    Y value
     */
    inline Coords(PositionType _x, PositionType _y):
        x(_x), y(_y) {}
};

/** @brief %Area with defined position and size */
template <class PositionType, class SizeType> struct Area {
    PositionType x,     /**< @brief X position */
        y;              /**< @brief Y position */
    SizeType w,         /**< @brief Width */
        h;              /**< @brief Height */

    /**
     * @brief Constructor
     * @param _x    X position
     * @param _y    Y position
     * @param _w    Width
     * @param _h    Height
     */
    inline Area(PositionType _x, PositionType _y, SizeType _w, SizeType _h):
        x(_x), y(_y), w(_w), h(_h) {}
};

/** @brief Absolutely defined area */
template <class PositionType> struct AbsoluteArea {
    PositionType x1,    /**< @brief Beginning X position */
        y1,             /**< @brief Beginning Y position */
        x2,             /**< @brief Ending X position */
        y2;             /**< @brief Ending Y position */

    /**
     * @brief Constructor
     * @param _x1   Beginning X position
     * @param _y1   Beginning Y position
     * @param _x2   Ending X position
     * @param _y2   Ending Y position
     */
    inline AbsoluteArea(PositionType _x1, PositionType _y1, PositionType _x2, PositionType _y2):
        x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}
};

}}

#endif
