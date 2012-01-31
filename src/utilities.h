#ifndef Kompas_utilities_h
#define Kompas_utilities_h
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
 * @brief Utilities
 */

#ifdef _WIN32
#ifdef CORE_EXPORTING
    #define CORE_EXPORT __declspec(dllexport)
#else
    #define CORE_EXPORT __declspec(dllimport)
#endif
#else
    #define CORE_EXPORT
#endif

#endif
