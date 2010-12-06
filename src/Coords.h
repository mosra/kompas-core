#ifndef Kompas_Core_Coords_h
#define Kompas_Core_Coords_h
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
 * @brief Class Kompas::Core::Coords
 */

#include "Utility/ConfigurationGroup.h"
#include "Utility/Debug.h"

namespace Kompas { namespace Core {

/** @brief Coordinates */
template <class PositionType> struct Coords {
    PositionType x,     /**< @brief X value */
        y;              /**< @brief Y value */

    /** @brief Implicit constructor */
    inline Coords(): x(0), y(0) {}

    /**
     * @brief Constructor
     * @param _x    X value
     * @param _y    Y value
     */
    inline Coords(PositionType _x, PositionType _y):
        x(_x), y(_y) {}

    /** @brief Equality operator */
    inline bool operator==(const Coords<PositionType>& other) const {
        return other.x == x && other.y == y;
    }

    /** @brief Non-equality operator */
    inline bool operator!=(const Coords<PositionType>& other) const {
        return !operator==(other);
    }
};

}

namespace Utility {

/** @copydoc Utility::ConfigurationValue */
template<class PositionType> struct ConfigurationValue<Core::Coords<PositionType> > {
    /**
     * @copydoc Utility::ConfigurationValue::toString()
     * Returns two values separated with space.
     */
    static std::string toString(const Core::Coords<PositionType>& value, int flags = 0) {
        return
            ConfigurationValue<PositionType>::toString(value.x, flags) + ' ' +
            ConfigurationValue<PositionType>::toString(value.y, flags);
    }

    /**
     * @copydoc Utility::ConfigurationValue::fromString()
     * Expects two values separated with space. Missing values are set to zero.
     */
    static Core::Coords<PositionType> fromString(const std::string& stringValue, int flags = 0) {
        std::istringstream i(stringValue);
        std::string x, y;

        Core::Coords<PositionType> coords;
        (i >> x) && (coords.x = ConfigurationValue<PositionType>::fromString(x, flags));
        (i >> y) && (coords.y = ConfigurationValue<PositionType>::fromString(y, flags));

        return coords;
    }
};

template<class PositionType> Debug& operator<<(Debug debug, const Core::Coords<PositionType>& value) {
    std::ostringstream o;
    o << "Coords(" << value.x << ", " << value.y << ")";

    return debug << o.str();
}

}}

#endif
