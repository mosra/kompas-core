#ifndef Map2X_Core_AbstractTileModel_h
#define Map2X_Core_AbstractTileModel_h
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
 * @brief Class Map2X::Core::AbstractTileModel and related types
 */

#include <string>
#include <vector>

#include "basictypes.h"
#include "RasterCoords.h"
#include "Wgs84Coords.h"
#include "PluginManager/definitions.h"

namespace Map2X { namespace Core {

typedef Coords<unsigned int> TileSize;              /**< @brief Tile size */
typedef unsigned int Zoom;                          /**< @brief Map zoom */
typedef Coords<unsigned int> TileCoords;            /**< @brief Tile coordinates */
typedef Area<unsigned int, unsigned int> TileArea;  /**< @brief Tile area */

/**
 * @brief Abstract model for tiled maps
 *
 * @todo Subclassing documentation
 * @todo Saving tiles to cache, Cache* tileCache()
 * @todo Installing a image filter plugin for modifying tileData
 *      (compression with pngnq, etc.)
 */
class AbstractTileModel {
    PLUGIN_INTERFACE("cz.mosra.Map2X.Core.AbstractTileModel/0.1")

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

        /** @{ @name Model parameters
         * Common for all maps of the same type.
         */

        /**
         * @brief Features provided by model
         * @return OR-ed values from AbstractTileModel::Feature
         */
        virtual int features() const = 0;

        /** @brief Tile size */
        virtual TileSize tileSize() const = 0;

        /** @brief Map copyright */
        virtual std::string copyright() const = 0;

        /*@}*/

        /** @brief Constructor  */
        inline AbstractTileModel(): enabledOnline(false) {}

        /** @{ @name Map parameters */

        /**
         * @brief User defined map attributes
         * @param type      Map attribute type
         * @param package   Package ID (returned by addPackage)
         * @return Attribute value
         */
        std::string attribute(Attribute type, int package);

        /**
         * @brief Zoom levels
         *
         * All zoom levels provided by map.
         */
        virtual std::vector<Zoom> zoomLevels() const = 0;

        /**
         * @brief Zoom step
         *
         * How many times will the map enlarge in each direction when zooming
         * in. Default value is 2.
         */
        virtual inline double zoomStep() const { return 2; }

        /**
         * @brief Map area for given zoom
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
         * @see AbstractTileModel::overlays()
         */
        virtual std::vector<std::string> layers() const = 0;

        /**
         * @brief Overlays
         *
         * List of overlays provided by map. Overlay is semi-transparent layer
         * which can be rendered on top of normal layers and brings an
         * additional to them.
         * @see AbstractTileModel::layers()
         */
        virtual std::vector<std::string> overlays() const = 0;

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
         * @see AbstractTileModel::LoadableFromFile
         */
        virtual int addPackage(std::string packageDir);

        /**
         * @brief Enable/disable online maps
         * @return Whether online maps are enabled or disabled. Enabling can
         *      fail if a package doesn't implement online features.
         *
         * When online maps are enabled, all map parameters are set to maximum
         * possible values. Otherwise the map area size is determined from
         * loaded packages.
         * @see AbstractTileModel::LoadableFromUrl
         */
        virtual bool setOnline(bool enabled);

        /**
         * @brief Set cache for online maps
         * @param cacheDir  Directory with cache files.
         * @return Whether the cache is available. Enabling can fail if cache
         *      dir is not readable or writable.
         *
         * Downloaded tiles can be pushed into cache for later usage.
         */
        virtual bool setCache(std::string cacheDir);

        /*@}*/

        /** @{ @name Map data */

        /**
         * @brief Tile URL
         * @param layer     Map layer or overlay
         * @param z         Zoom level
         * @param coords    Coordinates
         * @return Tile URL
         *
         * Returns URL of tile at given position. Recommended method of getting
         * tiles from URL is:
         * -# get the URL from model
         * -# download tile data from the URL
         * -# save that data to cache
         * -# load tile with AbstractTileModel::tileData()
         * This way can be properly used the tile cache and image filters
         * applied to tiles.
         * @note Default implementation returns empty string.
         */
        virtual inline std::string tileUrl(std::string layer, Zoom z, const TileCoords& coords) const { return ""; }

        /**
         * @brief Tile filename
         * @param layer     Map layer or overlay
         * @param z         Zoom level
         * @param coords    Coordinates
         * @return Filename (with optional path)
         *
         * Returns tile filename (without path).
         * @note Default implementation returns empty string.
         */
        virtual std::string tileFilename(std::string layer, Zoom z, const TileCoords& coords) const { return ""; }

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
         * @see AbstractTileModel::LoadableFromFile, AbstractTileModel::tileFile(), AbstractTileModel::enableOnlineWithCache()
         */
        virtual std::string tileData(std::string layer, Zoom z, const TileCoords& coords) const;

        /*@}*/

        /** @{ @name Coordinates conversion */

        /**
         * @brief Get raster map coordinates from WGS84 coordinates
         * @param z         Zoom level of raster coordinates
         * @param coords    WGS84 coordinates
         * @return Raster map coordinates
         *
         * Default implementation in AbstractTileModel returns invalid coordinates.
         */
        inline virtual RasterCoords fromWgs84(Zoom z, const Wgs84Coords& coords) const { return RasterCoords(); }

        /**
         * @brief Get WGS84 coordinates from raster map coordinates
         * @param z         Zoom level of raster coordinates
         * @param coords    Raster map coordinates
         * @return WGS84 coordinates
         *
         * Default implementation in AbstractTileModel returns invalid coordinates.
         */
        inline virtual Wgs84Coords toWgs84(Zoom z, const RasterCoords& coords) const { return Wgs84Coords(); }

        /*@}*/

    private:
        bool enabledOnline;
};

}}

#endif
