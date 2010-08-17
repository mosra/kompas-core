#ifndef Map2X_Core_Wgs84Coords_h
#define Map2X_Core_Wgs84Coords_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

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
#include "Utility/ConfigurationGroup.h"

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
         * @brief Convert DMS representation to decimal number
         * @param dms       DMS string
         * @return Always positive decimal value or -1 on error.
         *
         * Goes through given string and gets at maximum three numeric parts,
         * which are then converted to decimal number.
         */
        static double dmsToDecimal(const std::string& dms);

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
         * @brief Create coordinates from string
         * @param coords    Coordinates as string
         * @param _format   Format of input string. See Wgs84Coords::format.
         *
         * Tries to create coordinates from string with given format. If
         * conversion fails, returns invalid coordinates.
         * @todo Parser error to stderr, try/catch...
         */
        Wgs84Coords(const std::string& coords, const std::string& _format = format);

        /**
         * @brief Convert coordinates to string
         * @param precision     Precision. Non-negative value greater sets count
         *      of decimal places. If set to @c -1, seconds are stripped away,
         *      if set to @c -2, only degrees are in output, minutes and seconds
         *      are stripped away.
         * @param _format       Format of converted string.
         *      See Wgs84Coords::format.
         * @return Converted string. If the format is not valid, returns empty
         *      string. If the coords are not valid, returns position 0°N 0°E.
         *
         * Converts numeric coordinates to text representation (e.g.
         * <tt>49°9'33.167"N 15°12'4.774"E</tt>).
         * @todo Math-correctly round degrees/minutes on negative precision
         * @todo Automatic precision (don't display M/S when there are zeros)
         */
        std::string toString(int precision = 3, const std::string& _format = format) const;

        /**
         * @brief Distance between two places
         * @param a             First place
         * @param b             Second place
         * @param precision     Round to given precision (in meters). Default
         *      is one decimeter.
         * @return Distance in meters. When places are nearly antipodial, the
         * function might fail to compute the distance. In that case returns -1.
         *
         * Computes distance between two given coordinates on WGS84 geoid.
         * Based on Vincenty Inverse Solution of Geodesics on the Ellipsoid ©
         * Chris Veness 2002-2010,
         * http://www.movable-type.co.uk/scripts/latlong-vincenty.html .
         */
        static double distance(const Wgs84Coords& a, const Wgs84Coords& b, double precision = 0.1);

    private:
        std::vector<std::string> parseFormatters(const std::string& format) const;
};

/** @brief Comparing operator */
bool operator==(const Wgs84Coords& a, const Wgs84Coords& b);

/** @brief Comparing operator */
inline bool operator!=(const Wgs84Coords& a, const Wgs84Coords& b) {
    return !operator==(a, b);
}

}

namespace Utility {

/**
 * @brief Convert string from configuration to Wgs84Coords
 * @copydetails Utility::configurationValueFromString()
 *
 * If the configuration string doesn't contain both numeric coordinates, invalid
 * coordinates are returned.
 */
template<> Core::Wgs84Coords configurationValueFromString(const std::string& stringValue, int flags);

/**
 * @brief Convert Wgs84Coords to string storable in configuration
 * @copydetails Utility::configurationValueToString()
 *
 * Invalid coordinates are converted to single zero character.
 */
template<> std::string configurationValueToString(const Core::Wgs84Coords& value, int flags);

}

}

#endif
