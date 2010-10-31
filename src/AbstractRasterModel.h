#ifndef Map2X_Core_AbstractRasterModel_h
#define Map2X_Core_AbstractRasterModel_h
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
 * @brief Class Map2X::Core::AbstractRasterModel and related types
 */

#include <string>
#include <vector>

#include "Coords.h"
#include "Area.h"
#include "Wgs84Coords.h"
#include "PluginManager/Plugin.h"

namespace Map2X { namespace Core {

class AbstractProjection;

typedef Coords<unsigned int> TileSize;              /**< @brief Tile size */
typedef unsigned int Zoom;                          /**< @brief Map zoom */
typedef Coords<unsigned int> TileCoords;            /**< @brief Tile coordinates */
typedef Area<unsigned int, unsigned int> TileArea;  /**< @brief Tile area */

/**
 * @brief Abstract model for raster maps
 *
 * @todo Subclassing documentation
 * @todo Saving tiles to cache, Cache* tileCache()
 * @todo Installing a image filter plugin for modifying tileData
 *      (compression with pngnq, etc.)
 */
class AbstractRasterModel: public PluginManager::Plugin {
    PLUGIN_INTERFACE("cz.mosra.Map2X.Core.AbstractRasterModel/0.1")

    public:
        /**
         * @brief Features provided by map model
         *
         * Every map model is capable of at least loading maps from map archive,
         * if not anything else.
         */
        enum Feature {
            LoadableFromUrl     = 0x01, /**< @brief Maps are loadable from URLs */
            LoadableFromFile    = 0x02, /**< @brief Maps are loadable from local files */
            ConvertableCoords   = 0x04  /**< @brief Coordinates can be converted to and from WGS84 */
        };

        /** @brief Map attribute types */
        enum Attribute {
            Name,           /**< @brief Map name */
            Description,    /**< @brief Map description */
            Packager        /**< @brief Map packager */
        };

        /** @{ @name Utilites */

        /**
         * @brief Minimum count of tiles to fill given area
         * @param area  Area size in pixels
         * @return Tile count
         */
        inline Coords<unsigned int> tilesInArea(Coords<unsigned int> area) const {
            return Coords<unsigned int>((area.x-2)/tileSize().x + 2,
                                        (area.y-2)/tileSize().y + 2);
        }

        /*@}*/

        /** @{ @name Model parameters
         * Common for all maps of the same type.
         */

        /**
         * @brief Features provided by model
         * @return OR-ed values from AbstractRasterModel::Feature
         */
        virtual int features() const = 0;

        /**
         * @brief Map projection
         *
         * Default implementation returns zero pointer which means that
         * this particular RasterModel doesn't have specified projection.
         */
        inline virtual const AbstractProjection* projection() const { return 0; }

        /** @brief Tile size */
        virtual TileSize tileSize() const = 0;

        /**
         * @brief Map copyright
         * @todo Depending on coordinates and map layer (to "properly" support Google)
         */
        inline virtual std::string copyright() const { return ""; }

        /*@}*/

        /** @brief Constructor  */
        AbstractRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin):
            Plugin(manager, plugin), _online(false) {}

        /** @{ @name Map parameters */

        /**
         * @brief User defined map attributes
         * @param type      Map attribute type
         * @param package   Package ID (returned by addPackage)
         * @return Attribute value. Default implementation returns empty string.
         */
        inline virtual std::string attribute(Attribute type, int package) const { return ""; }

        /**
         * @brief Zoom levels
         * @return List of zooms ordered in ascending order (lowest zoom first).
         *
         * All zoom levels provided by map.
         */
        virtual std::vector<Zoom> zoomLevels() const = 0;

        /**
         * @brief Zoom step
         *
         * How many times will the map enlarge in each direction when zooming
         * in. Default value is 2.
         * @todo Go to hell with that (unneccessary floating point arithmetrics).
         */
        virtual inline double zoomStep() const { return 2; }

        /**
         * @brief Map area for lowest zoom
         *
         * Position and size of map area which is available for the lowest
         * available zoom. Map area at greater zoom values can be computed using
         * zoomStep().
         */
        virtual TileArea area() const = 0;

        /**
         * @brief Layers
         *
         * List of layers provided by map. Layer is not transparent map
         * background and on top of it can be placed overlays.
         * @see AbstractRasterModel::overlays()
         */
        virtual std::vector<std::string> layers() const = 0;

        /**
         * @brief Overlays
         *
         * List of overlays provided by map. Overlay is semi-transparent layer
         * which can be rendered on top of normal layers and brings an
         * additional to them.
         * @see AbstractRasterModel::layers()
         */
        inline virtual std::vector<std::string> overlays() const {
            return std::vector<std::string>();
        }

        /*@}*/

        /** @{ @name Map initialization
         * The map model contains nothing after startup. After enabling online
         * maps or adding a package the model can provide data.
         */

        /**
         * @brief Add map package
         * @return Number of actually loaded package or -1 on failure. Loading
         *      can fail if a package doesn't belong to this model or if package
         *      is contained of single files for which the model doesn't have
         *      implemented features.
         *
         * When a package is successfully added, all map parameters are extended
         * to everything what a given map package provides. More packages can
         * be added to a single model and tiles from them will be loaded in
         * reverse order (tiles will be first searched in most-recently-added
         * packages).
         * @see AbstractRasterModel::LoadableFromFile
         */
        virtual int addPackage(const std::string& packageDir);

        /**
         * @brief Enable/disable online maps
         * @return Whether online maps are enabled or disabled. Enabling can
         *      fail if a package doesn't implement online features.
         *
         * When online maps are enabled, all map parameters are set to maximum
         * possible values. Otherwise the map area size is determined from
         * loaded packages.
         * @see AbstractRasterModel::LoadableFromUrl
         */
        virtual bool setOnline(bool enabled);

        /** @brief Whether online maps are enabled */
        inline bool online() const { return _online; }

        /**
         * @brief Set cache for online maps
         * @param cacheDir  Directory with cache files.
         * @return Whether the cache is available. Enabling can fail if cache
         *      dir is not readable or writable.
         *
         * Downloaded tiles can be pushed into cache for later usage.
         */
        virtual bool setCache(const std::string& cacheDir);

        /*@}*/

        /** @{ @name Getting map data */

        /**
         * @brief Get URL of tile at given position
         * @param layer     Map layer or overlay
         * @param z         Zoom level
         * @param coords    Coordinates
         * @return Tile URL. Default implementation returns empty string.
         * @see AbstractRasterModel::LoadableFromUrl
         */
        virtual inline std::string tileUrl(const std::string& layer, Zoom z, const TileCoords& coords) const { return ""; }

        /**
         * @brief Tile data
         * @param layer     Map layer or overlay
         * @param z         Zoom level
         * @param coords    Coordinates
         * @return Tile data (image) or empty string
         *
         * Tries to find tile data in this order:
         * - from map archive (if any package is added)
         * - from tile cache (if any cache is set up)
         * - from single file (if any package is added and file feature is
         *   implemented)
         * If nothing found, returns empty string.
         * @see AbstractRasterModel::LoadableFromFile, AbstractRasterModel::tileFile(), AbstractRasterModel::enableOnlineWithCache()
         */
        virtual std::string tileData(const std::string& layer, Zoom z, const TileCoords& coords);

        /*@}*/

    private:
        bool _online;
};

}}

#endif
