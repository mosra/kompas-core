#ifndef Kompas_Core_Area_h
#define Kompas_Core_Area_h
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

/** @file
 * @brief Class Kompas::Core::Area
 */

#include "Utility/ConfigurationGroup.h"
#include "Coords.h"

namespace Kompas { namespace Core {

/** @brief %Area with defined position and size */
template <class PositionType, class SizeType> struct Area {
    PositionType x,     /**< @brief X position */
        y;              /**< @brief Y position */
    SizeType w,         /**< @brief Width */
        h;              /**< @brief Height */

    /** @brief Implicit constructor */
    inline Area(): x(0), y(0), w(0), h(0) {}

    /**
     * @brief Constructor
     * @param _x    X position
     * @param _y    Y position
     * @param _w    Width
     * @param _h    Height
     */
    inline Area(PositionType _x, PositionType _y, SizeType _w, SizeType _h):
        x(_x), y(_y), w(_w), h(_h) {}

    /**
     * @brief Multiplication operator
     * @return All coordinates multiplied by given number
     */
    template<class T> Area<PositionType, SizeType> operator*(T multiplier) const {
        return Area<PositionType, SizeType>(
            x*multiplier,
            y*multiplier,
            w*multiplier,
            h*multiplier
        );
    }

    /**
     * @brief Multiplication operator
     * @return Horizontal coordinates multiplied by x coordinate, vertical
     *      with y coordinate.
     */
    template<class T> Area<PositionType, SizeType> operator*(const Coords<T>& multiplier) const {
        return Area<PositionType, SizeType>(
            x*multiplier.x,
            y*multiplier.y,
            w*multiplier.x,
            h*multiplier.y
        );
    }

    /**
     * @brief Division operator
     * @return All coordinates divided by given number
     */
    template<class T> Area<PositionType, SizeType> operator/(T divisor) const {
        return Area<PositionType, SizeType>(
            x/divisor,
            y/divisor,
            w/divisor,
            h/divisor
        );
    }

    /**
     * @brief Division operator
     * @return Horizontal coordinates divided by x coordinate, vertical
     *      with y coordinate.
     */
    template<class T> Area<PositionType, SizeType> operator/(const Coords<T>& divisor) const {
        return Area<PositionType, SizeType>(
            x/divisor.x,
            y/divisor.y,
            w/divisor.x,
            h/divisor.y
        );
    }

    /** @brief Equality operator */
    bool operator==(const Area<PositionType, SizeType>& other) const {
        return other.x == x &&
               other.y == y &&
               other.w == w &&
               other.h == h;
    }

    /** @brief Non-equality operator */
    inline bool operator!=(const Area<PositionType, SizeType>& other) const {
        return !operator==(other);
    }
};

}

namespace Utility {

/** @copydoc Utility::ConfigurationValue */
template<class PositionType, class SizeType> struct ConfigurationValue<Core::Area<PositionType, SizeType> > {
    /**
     * @copydoc Utility::ConfigurationValue::toString()
     * Returns four values separated with spaces.
     */
    static std::string toString(const Core::Area<PositionType, SizeType>& value, int flags = 0) {
        return
            ConfigurationValue<PositionType>::toString(value.x, flags) + ' ' +
            ConfigurationValue<PositionType>::toString(value.y, flags) + ' ' +
            ConfigurationValue<SizeType>::toString(value.w, flags) + ' ' +
            ConfigurationValue<SizeType>::toString(value.h, flags);
    }

    /**
     * @copydoc Utility::ConfigurationValue::fromString()
     * Expects four values separated with spaces. Missing values are set to
     * zero.
     */
    static Core::Area<PositionType, SizeType> fromString(const std::string& stringValue, int flags = 0) {
        std::istringstream i(stringValue);
        std::string x, y, w, h;

        Core::Area<PositionType, SizeType> area;
        (i >> x) && (area.x = ConfigurationValue<PositionType>::fromString(x, flags));
        (i >> y) && (area.y = ConfigurationValue<PositionType>::fromString(y, flags));
        (i >> w) && (area.w = ConfigurationValue<SizeType>::fromString(w, flags));
        (i >> h) && (area.h = ConfigurationValue<SizeType>::fromString(h, flags));

        return area;
    }
};

/** @copydoc operator<<(Debug, const T&) */
template<class PositionType, class SizeType> Debug& operator<<(Debug debug, const Core::Area<PositionType, SizeType>& value) {
    std::ostringstream o;
    o << "Area(" << value.x << ", " << value.y << ", " << value.w << ", " << value.h << ")";

    return debug << o.str();
}

}}

#endif
