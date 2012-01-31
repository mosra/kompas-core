#ifndef Kompas_Core_AbstractCelestialBody_h
#define Kompas_Core_AbstractCelestialBody_h
/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

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
 * @brief Class Kompas::Core::AbstractCelestialBody
 */

#include "PluginManager/Plugin.h"
#include "LatLonCoords.h"

namespace Kompas { namespace Core {

/**
 * @brief Abstract class for celestial bodies
 */
class CORE_EXPORT AbstractCelestialBody: public Corrade::PluginManager::Plugin {
    PLUGIN_INTERFACE("cz.mosra.Kompas.Core.AbstractCelestialBody/0.2")

    public:
        /** @copydoc PluginManager::Plugin::Plugin() */
        inline AbstractCelestialBody(Corrade::PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            Plugin(manager, plugin) {}

        /**
         * @brief Equatorial radius
         *
         * In meters.
         */
        virtual double equatorialRadius() const = 0;

        /**
         * @brief Polar radius
         *
         * In meters. If unknown, can be computed from equatorial radius and
         * flattening:
         * @f[
         *     polarRadius = equatorialRadius*(1 - flattening)
         * @f]
         */
        virtual double polarRadius() const = 0;

        /**
         * @brief Flattening
         *
         * If unknown, can be computed from equatorial radius and polar radius:
         * @f[
         *     flattening = {{equatorialRadius - polarRadius} \over equatorialRadius}
         * @f]
         */
        virtual double flattening() const = 0;

        /**
         * @brief Distance between two places
         * @param a             First place
         * @param b             Second place
         * @return Distance in meters. When places are nearly antipodial, the
         * function might fail to compute the distance. In that case returns -1.
         *
         * Default implementation computes the distance using equatorialRadius(),
         * polarRadius() and flattening(). Based on Vincenty Inverse Solution
         * of Geodesics on the Ellipsoid © Chris Veness 2002-2010,
         * http://www.movable-type.co.uk/scripts/latlong-vincenty.html .
         */
        virtual double distance(const LatLonCoords& a, const LatLonCoords& b);
};

}}

#endif
