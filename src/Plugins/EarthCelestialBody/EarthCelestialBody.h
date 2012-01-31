#ifndef Kompas_Plugins_EarthCelestialBody_h
#define Kompas_Plugins_EarthCelestialBody_h
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
 * @brief Class Kompas::Plugins::EarthCelestialBody
 */

#include "AbstractCelestialBody.h"

namespace Kompas { namespace Plugins {

/**
 * @brief Earth
 */
class EarthCelestialBody: public Core::AbstractCelestialBody {
    public:
        /** @copydoc Core::AbstractCelestialBody::AbstractCelestialBody */
        EarthCelestialBody(Corrade::PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""): AbstractCelestialBody(manager, plugin) {}

        /** @todo Which values are closer to reality? */
        inline double equatorialRadius() const { return 6378137; }
      //inline double equatorialRadius() const { return 6378136.6; }
        inline double polarRadius() const { return 6356752.3142; }
      //inline double polarRadius() const { return 6356751.9; }
        inline double flattening() const { return 1/298.257223563; }
      //inline double flattening() const { return 1/298.25642; }
};

}}

#endif
