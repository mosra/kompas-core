#ifndef Map2X_Core_Wgs84Coords_h
#define Map2X_Core_Wgs84Coords_h
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
 * @brief Class Map2X::Core::Wgs84Coords
 */

#include <string>

namespace Map2X { namespace Core {

/**
 * @brief WGS84 (GPS) coordinates
 *
 * Class for storing latitude and longtitude coordinates (in degrees).
 * Only valid coordinates are stored. If a valid coordinates were not passed,
 * resets both longtitude and latitude to zero and marks the coordinates as
 * invalid. Coordinates can be converted to string representation with user
 * defined decimal precision of seconds and string format.
 * @todo Conversion from string
 */
class Wgs84Coords {
    private:
        double _lat, _lon;
        bool _isValid;

    public:
        /**
         * @brief Coordinate string format
         *
         * Used for converting coordinates from and to string. The format string
         * contains of @b ten parts separated by (eight) LF characters
         * (@c '@\n').
         * -# Prefix before latitude - @c ''
         * -# Separator between latitude and longtitude - @c ' '
         * -# Suffix after longtitude - @c ''
         * -# Degrees sign - @c '°'
         * -# Minutes sign - @c '''
         * -# Seconds sign - @c '"'
         * -# Sign for north - @c 'N'
         * -# Sign for south - @c 'S'
         * -# Sign for east - @c 'E'
         * -# Sign for west - @c 'W'
         *
         * See also Wgs84Coords::toString().
         */
        static const std::string format;

        /**
         * @brief Latitude
         *
         * Positive value means north, negative south.
         */
        inline double latitude() const { return _lat; }

        /**
         * @brief Longitude
         *
         * Positive value means east, negative west.
         */
        inline double longtitude() const { return _lon; }

        /** @brief Whether the coordinates are valid */
        inline bool isValid() const { return _isValid; }

        /**
         * @brief Implicit constructor
         *
         * Produces invalid coordinates.
         */
        inline Wgs84Coords(): _lat(0), _lon(0), _isValid(false) {}

        /**
         * @brief Constructor
         * @param __lat     Latitude Positive value means north, negative south.
         * @param __lon     Longtitude. Positive value means east, negative west.
         *
         * Checks whether the values are in valid range, if not, produces
         * invalid coordinates.
         */
        Wgs84Coords(double __lat, double __lon);

        /**
         * @brief Convert coordinates to string
         * @param precision     Decimal precision for seconds.
         * @param _format       Format of converted string.
         *      See Wgs84Coords::format.
         * @return Converted string. If the coordinates or format are not valid
         *      returns empty string.
         *
         * Converts numeric coordinates to text representation (e.g.
         * <tt>49°9'33.167"N 15°12'4.774"E</tt>).
         */
        std::string toString(int precision = 3, const std::string& _format = format) const;
};

/** @brief Comparing operator */
bool operator==(const Wgs84Coords& a, const Wgs84Coords& b);

/** @brief Comparing operator */
inline bool operator!=(const Wgs84Coords& a, const Wgs84Coords& b) {
    return !operator==(a, b);
}

}}

#endif
