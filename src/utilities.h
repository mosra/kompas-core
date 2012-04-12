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

/* Temporary fix for building with what will eventually become Corrade 0.3 */
#include "PluginManager/Plugin.h"

#ifndef PLUGIN_REGISTER_STATIC
#undef PLUGIN_REGISTER
#define PLUGIN_REGISTER_STATIC(name, className, interface)                  \
    inline void* pluginInstancer_##name(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& plugin) \
        { return new className(manager, plugin); }                          \
    int pluginInitializer_##name() {                                        \
        Corrade::PluginManager::AbstractPluginManager::importStaticPlugin(#name, PLUGIN_VERSION, interface, pluginInstancer_##name); return 1; \
    } AUTOMATIC_INITIALIZER(pluginInitializer_##name)
#define PLUGIN_REGISTER(className, interface)                               \
    extern "C" int pluginVersion() { return PLUGIN_VERSION; }               \
    extern "C" void* pluginInstancer(Corrade::PluginManager::AbstractPluginManager* manager, const std::string& plugin) \
        { return new className(manager, plugin); }                          \
    extern "C" std::string pluginInterface() { return interface; }
#endif

#endif
