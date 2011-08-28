#ifndef Kompas_Core_LatLonCoords_h
#define Kompas_Core_LatLonCoords_h
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
 * @brief Class Kompas::Core::LatLonCoords
 */

#include <string>

#include "Utility/ConfigurationGroup.h"
#include "Utility/Debug.h"

namespace Kompas { namespace Core {

/**
 * @brief Lat/lon (GPS) coordinates
 *
 * Class for storing latitude and longitude coordinates (in degrees).
 * Only valid coordinates are stored. If a valid coordinates were not passed,
 * resets both longitude and latitude to zero and marks the coordinates as
 * invalid. Coordinates can be converted to string representation with user
 * defined precision and string format.
 */
class CORE_EXPORT LatLonCoords {
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
         * -# Separator between latitude and longitude - @c ' '
         * -# Suffix after longitude - @c ''
         * -# Degrees sign - @c '°'
         * -# Minutes sign - @c '''
         * -# Seconds sign - @c '"'
         * -# Sign for north - @c 'N'
         * -# Sign for south - @c 'S'
         * -# Sign for east - @c 'E'
         * -# Sign for west - @c 'W'
         *
         * See also LatLonCoords::toString().
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
         * @brief Convert decimal number to DMS representation
         * @param decimal       Decimal number
         * @param precision     Precision. See LatLonCoords::toString().
         * @param skipTrailingZeros Whether to skip trailing zeros.
         * @param _format       Formatting string. See LatLonCoords::format.
         * @return DMS string or empty string on error.
         */
        static std::string decimalToDms(double decimal, int precision = 3, bool skipTrailingZeros = false, const std::string& _format = format);

        /**
         * @brief Create coordinates from point on a sphere
         * @param x             X coordinate (right is positive)
         * @param y             Y coordinate (up is positive)
         * @param z             Z coordinate (backward is positive)
         * @return If the coordinates are not point on a sphere (vector length
         * is not 1), returns invalid coordinates.
         *
         * Converts coordinates in right-handed Cartesian space to latitude
         * and longitude:
         * @f[
         *     \begin{array}{lcl}
         *         latitude & = & \arcsin y                                 \\
         *         longitude & = & \arcsin {x \over \sqrt {x^2 + z^2}}
         *     \end{array}
         * @f]
         * Longitude is computed only to range
         * @f$ [ - 90^\circ ; 90^\circ ] @f$, so if the Z coordinate
         * is negative, the longitude is converted to range
         * @f$ [ -180^\circ ; -90^\circ ] \cup [ 90^\circ ; 180^\circ ] @f$.
         */
        static LatLonCoords fromPointOnSphere(double x, double y, double z);

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
        inline double longitude() const { return _lon; }

        /** @brief Whether the coordinates are valid */
        inline bool isValid() const { return _isValid; }

        /**
         * @brief Implicit constructor
         *
         * Produces invalid coordinates.
         */
        inline LatLonCoords(): _lat(0), _lon(0), _isValid(false) {}

        /**
         * @brief Constructor
         * @param __lat     Latitude Positive value means north, negative south.
         * @param __lon     Longtitude. Positive value means east, negative west.
         *
         * Checks whether the values are in valid range, if not, produces
         * invalid coordinates.
         */
        LatLonCoords(double __lat, double __lon);

        /**
         * @brief Create coordinates from string
         * @param coords    Coordinates as string
         * @param _format   Format of input string. See LatLonCoords::format.
         *
         * Tries to create coordinates from string with given format. If
         * conversion fails, returns invalid coordinates.
         */
        LatLonCoords(const std::string& coords, const std::string& _format = format);

        /**
         * @brief Convert coordinates to string
         * @param precision     Precision. Non-negative value greater sets count
         *      of decimal places. If set to @c -1, seconds are stripped away,
         *      if set to @c -2, only degrees are in output, minutes and seconds
         *      are stripped away.
         * @param skipTrailingZeros Whether to skip trailing zeros.
         * @param _format       Format of converted string.
         *      See LatLonCoords::format.
         * @return Converted string. If the format is not valid, returns empty
         *      string. If the coords are not valid, returns position 0°N 0°E.
         *
         * Converts numeric coordinates to text representation (e.g.
         * <tt>49°9'33.167"N 15°12'4.774"E</tt>).
         */
        std::string toString(int precision = 3, bool skipTrailingZeros = false, const std::string& _format = format) const;

        /**
         * @brief Convert coordinates to point on a sphere
         * @param x             Pointer to store X coordinate (right is positive)
         * @param y             Pointer to store Y coordinate (up is positive)
         * @param z             Pointer to store Z coordinate (backward is positive)
         *
         * Converts coordinates to right-handed Cartesian space:
         * @f[
         *     \begin{array}{lcl}
         *         x & = & \sin longitude \cos latitude                     \\
         *         y & = & \sin latitude                                    \\
         *         z & = & \cos longitude \cos latitude
         *     \end{array}
         * @f]
         */
        void toPointOnSphere(double* x, double* y, double* z) const;

        /** @brief Equality operator */
        bool operator==(const LatLonCoords& other) const;

        /** @brief Non-equality operator */
        inline bool operator!=(const LatLonCoords& other) const {
            return !operator==(other);
        }

    private:
        static std::vector<std::string> parseFormatters(const std::string& format);

        static void decimalToDms(double decimal, int precision, bool skipTrailingZeros, const std::vector<std::string>& formatters, std::ostringstream& out);
};

}

namespace Utility {

/** @copydoc Utility::ConfigurationValue */
template<> struct CORE_EXPORT ConfigurationValue<Core::LatLonCoords> {
    /**
     * @copydoc Utility::ConfigurationValue::fromString()
     * Expects two values separated with space. If the configuration string
     * doesn't contain both numeric coordinates, invalid coordinates are
     * returned.
     */
    static Core::LatLonCoords fromString(const std::string& stringValue, int flags);

    /**
     * @copydoc Utility::ConfigurationValue::toString()
     * Returns two values separated with space. Invalid coordinates are
     * converted to single zero character.
     */
    static std::string toString(const Core::LatLonCoords& value, int flags);
};

/** @copydoc operator<<(Debug, const T&) */
Debug& operator<<(Debug debug, const Core::LatLonCoords& value);

}

}

#endif
