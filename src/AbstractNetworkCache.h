#ifndef Kompas_Core_AbstractNetworkCache_h
#define Kompas_Core_AbstractNetworkCache_h
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
 * @brief Class Kompas::Core::AbstractNetworkCache
 */

#include "AbstractCache.h"

namespace Kompas { namespace Core {

/**
@brief Convenience base implementation for network caches

As network cache shouldn't be maintained by its users for security reasons,
functions for getting/setting cache size, purging and optimizing the cache have
their default dummy implementations.
*/
class AbstractNetworkCache: public AbstractCache {
    public:
        /** @copydoc AbstractCache::AbstractCache */
        inline AbstractNetworkCache(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            AbstractCache(manager, plugin) {}

        virtual int features() { return MultiUser|Network; }
        inline virtual size_t cacheSize() { return 0; }
        inline virtual void setCacheSize(size_t size) {}
        inline virtual void purge() {}
        inline virtual void optimize() {}
};

}}

#endif
