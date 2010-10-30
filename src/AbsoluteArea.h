#ifndef Map2X_Core_AbsoluteArea_h
#define Map2X_Core_AbsoluteArea_h
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
 * @brief Class Map2X::Core::AbsoluteArea
 */

#include "Utility/ConfigurationGroup.h"

namespace Map2X { namespace Core {

/** @brief Absolutely defined area */
template <class PositionType> struct AbsoluteArea {
    PositionType x1,    /**< @brief Beginning X position */
        y1,             /**< @brief Beginning Y position */
        x2,             /**< @brief Ending X position */
        y2;             /**< @brief Ending Y position */

    /** @brief Implicit constructor */
    inline AbsoluteArea(): x1(0), y1(0), x2(0), y2(0) {}

    /**
     * @brief Constructor
     * @param _x1   Beginning X position
     * @param _y1   Beginning Y position
     * @param _x2   Ending X position
     * @param _y2   Ending Y position
     */
    inline AbsoluteArea(PositionType _x1, PositionType _y1, PositionType _x2, PositionType _y2):
        x1(_x1), y1(_y1), x2(_x2), y2(_y2) {}

    template<class T> AbsoluteArea<PositionType> operator*(T multiplier) const {
        return AbsoluteArea<PositionType>(
            x1*multiplier,
            y1*multiplier,
            x2*multiplier,
            y2*multiplier
        );
    }
    template<class T> AbsoluteArea<PositionType> operator/(T divisor) const {
        return AbsoluteArea<PositionType>(
            x1/divisor,
            y1/divisor,
            x2/divisor,
            y2/divisor
        );
    }
    bool operator==(const AbsoluteArea<PositionType>& other) {
        return other.x1 == x1 &&
               other.y1 == y1 &&
               other.x2 == x2 &&
               other.y2 == y2;
    }
    inline bool operator!=(const AbsoluteArea<PositionType>& other) {
        return !operator==(other);
    }
};

}

namespace Utility {

template<class PositionType> struct ConfigurationValue<Core::AbsoluteArea<PositionType> > {
    static std::string toString(const Core::AbsoluteArea<PositionType>& value, int flags = 0) {
        return
            ConfigurationValue<PositionType>::toString(value.x1, flags) + ' ' +
            ConfigurationValue<PositionType>::toString(value.y1, flags) + ' ' +
            ConfigurationValue<PositionType>::toString(value.x2, flags) + ' ' +
            ConfigurationValue<PositionType>::toString(value.y2, flags);
    }

    static Core::AbsoluteArea<PositionType> fromString(const std::string& stringValue, int flags = 0) {
        std::istringstream i(stringValue);
        std::string x1, y1, x2, y2;

        Core::AbsoluteArea<PositionType> area;
        (i >> x1) && (area.x1 = ConfigurationValue<PositionType>::fromString(x1, flags));
        (i >> y1) && (area.y1 = ConfigurationValue<PositionType>::fromString(y1, flags));
        (i >> x2) && (area.x2 = ConfigurationValue<PositionType>::fromString(x2, flags));
        (i >> y2) && (area.y2 = ConfigurationValue<PositionType>::fromString(y2, flags));

        return area;
    }
};

}}

#endif
