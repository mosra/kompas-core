#ifndef Kompas_Core_AbstractCache_h
#define Kompas_Core_AbstractCache_h
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
 * @brief Class Kompas::Core::AbstractCache
 */

#include "PluginManager/Plugin.h"
#include "AbstractRasterModel.h"

namespace Kompas { namespace Core {

/**
@brief Interface for cache plugins

See also AbstractNetworkCache, which can be used as convenient base for network
caches.
*/
class AbstractCache: public PluginManager::Plugin {
    PLUGIN_INTERFACE("cz.mosra.Kompas.Core.AbstractCache/0.2")

    public:
        /** @brief Features */
        enum Features {
            /**
             * The cache can be used by multiple users simultaenously. If this
             * feature is not supported, the cache should prevent it.
             */
            MultiUser = 0x01,

            /**
             * The cache is block based - files in the cache are occupying
             * blocks of space instead of their exact size and block size is
             * be configured by the user.
             */
            BlockBased = 0x02,

            /**
             * The cache is network based, thus user can only save or retrieve
             * files from the cache.
             */
            Network = 0x04
        };

        /** @copydoc PluginManager::Plugin::Plugin */
        inline AbstractCache(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            Plugin(manager, plugin) {}

        /**
         * @brief Features
         * @return OR-ed list of features.
         */
        virtual int features() const = 0;

        /**
         * @brief Initialize cache
         * @param url       For network cache (with @ref Network feature) URL of
         *      the cache, otherwise path to existing directory.
         * @return Whether the cache was successfully initialized
         *
         * If the initialization failed, implementation shouldn't allow setting
         * data to cache.
         */
        virtual bool initializeCache(const std::string& url) = 0;

        /**
         * @brief Finalize cache
         *
         * Saves all unsaved cache data and cleans up the memory.
         * @attention This should be called from subclass destructor.
         */
        virtual void finalizeCache() = 0;

        /**
         * @brief Block size
         * @return Block size, in bytes.
         *
         * If the cache doesn't have @ref BlockBased feature, this function does
         * nothing.
         */
        inline virtual size_t blockSize() const { return 0; }

        /**
         * @brief Set block size
         * @param size      Block size, in bytes.
         *
         * If the cache is network or doesn't have @ref BlockBased feature, this
         * function does nothing.
         */
        virtual void setBlockSize(size_t size) {}

        /**
         * @brief Cache size
         * @return Cache size, in bytes.
         *
         * For network cache returns 0.
         */
        virtual size_t cacheSize() const = 0;

        /**
         * @brief Set cache size
         * @param size      Cache size, in bytes.
         *
         * For network cache this function does nothing.
         */
        virtual void setCacheSize(size_t size) = 0;

        /** @brief Used size */
        virtual size_t usedSize() const = 0;

        /**
         * @brief Purge cache
         *
         * Deletes all cache files. For network cache this function does
         * nothing.
         */
        virtual void purge() = 0;

        /**
         * @brief Optimize cache
         *
         * Optimizes sparse files, speeds up searching, etc. For network cache
         * this function does nothing.
         */
        virtual void optimize() = 0;

        /**
         * @brief Get raster tile from cache
         * @param model     Model name
         * @param layer     Layer
         * @param z         Zoom
         * @param coords    Coordinates
         * @return  Tile data or empty string, if the tile wasn't found.
         */
        std::string rasterTile(const std::string& model, const std::string& layer, Zoom z, const TileCoords& coords) {
            return get(key(model, layer, z, coords));
        }

        /**
         * @brief Save raster tile to cache
         * @param model     Model name
         * @param layer     Layer
         * @param z         Zoom
         * @param coords    Coordinates
         * @param data      Tile data
         */
        bool setRasterTile(const std::string& model, const std::string& layer, Zoom z, const TileCoords& coords, const std::string& data) {
            return set(key(model, layer, z, coords), data);
        }

    protected:
        static const char RasterTile = 'T';

        /**
         * @brief Get data from cache
         * @param key       Key
         */
        virtual std::string get(const std::string& key) = 0;

        /**
         * @brief Save data to cache
         * @param key       Key
         * @param data      Data
         */
        virtual bool set(const std::string& key, const std::string& data) = 0;

    private:
        std::string key(const std::string &model, const std::string &layer, Core::Zoom z, Core::TileCoords coords) const;
};

}}

#endif
