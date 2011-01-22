#ifndef Kompas_Core_AbstractRasterModel_h
#define Kompas_Core_AbstractRasterModel_h
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
 * @brief Class Kompas::Core::AbstractRasterModel and related types
 */

#include <string>
#include <vector>
#include <set>

#include "Coords.h"
#include "Area.h"
#include "Wgs84Coords.h"
#include "TranslatablePlugin.h"

namespace Kompas { namespace Core {

class AbstractProjection;

typedef Coords<unsigned int> TileSize;              /**< @brief Tile size */
typedef unsigned int Zoom;                          /**< @brief Map zoom */
typedef Coords<unsigned int> TileCoords;            /**< @brief Tile coordinates */
typedef Area<unsigned int, unsigned int> TileArea;  /**< @brief Tile area */

/**
@brief Abstract model for raster maps

@section AbstractRasterModel_Usage Usage
Raster map data are provided in form of tiles. The map is in most cases divided
into many tiles, where all tiles have the same dimensions. The model also
supports zoom levels, where every next zoom level has doubled width and
height. Map dimensions in given zoom level can be computed like this:
@code
// Some raster model and desired zoom level (the zoom level must be larger
// than lowest available zoom level of the model)
MyRasterModel model;
Zoom myZoom = 8;

// If the model has zero area or no zoom levels, exit...
if(!model.isReady()) return;

// Lowest available zoom level of the model
Zoom lowestZoom = model.zoomLevels()[0];

// Multiply map area (which is relative to lowest zoom level) to desired zoom
TileArea dimensions = model.area()*pow2(myZoom-lowestZoom);
@endcode

The model has one or more layers and zero or more overlays (images with alpha
channel), which can be placed on top of base layer. List of them can be
retrieved with layers() and overlays(). The model also provides translated
layer and overlay names, which can be retrieved with layerName(). More about
translations in @ref AbstractRasterModel_Subclassing_Read section.
@see SingleLayer, SingleZoom, WriteOnly

Map without known projection is almost useless, so if the model supports
@ref ConvertableCoords feature, projection() can return map projection. More
about projections and coordinate conversion in AbstractProjection class
documentation.

@subsection AbstractRasterModel_Usage_Read Getting the tile data
Tile data can be either read from local package, retrieved from cache or,
if anything other fails, downloaded. This hierarchy should be hold to avoid
unnecessary lookups in slow cache when the data are already in some package or
uneccessary downloads when the data are already in cache.

<em>Loading and reading packages:</em>

If you have more than one raster model available and an package of unknown
format, you can make use of automatic recognization of package type. In that
case the model must support @ref SelfRecognizable feature. Function
fileExtensions() returns a list of possible package extensions, function
recognize() takes filename and tries to guess the type from the filename or
first few bytes of the package.

The package can be loaded with addPackage(), the function returns ID of loaded
package on success. Count of loaded packages and package metadata can be
retrieved with packageCount() and packageAttribute().
@see MultiplePackages

Tile data can be retrieved from loaded packages with function tileFromPackage().

<em>Getting the tiles from cache, saving downloaded tiles to cache:</em>

The cache act as layer between fast packages and slow data download. The package
mostly contain an limited amount of tile data, but warrants fast access to the
data. The cache is here to manage often used tile data, so they don't have to
be downloaded on every request. The cache has a limited size and periodically
removes tile data, which aren't used for a long time. Because of that tile data
retrieving is not as fast as from the packages.

@todo @c VERSION-0.2 Merge cache branch, implement, then finish the doc

<em>Downloading tile data:</em>

If the raster model supports LoadableFromUrl feature, function tileUrl()
returns URL of tile data, which the client can then download.

@todo After finishing cache, document tileToCache() here

@subsection AbstractRasterModel_Usage_Write Creating map packages
If the model supports @ref WriteableFormat feature, the data can be not only
read, but also saved to new packages. Common process is to have one source model
(with attached packages, enabled cache, etc) and one destination model, which
is used only for creating the package. The source and destination model in most
cases doesn't even have to be of the same type, unless NonConvertableFormat
feature is set on destination model.

Package creation can be divided into three steps:

-# <em>Initializing the package</em> with initializePackage() and
optionally calling setPackageAttribute() to set any package attributes.
@see MultipleFileFormat

-# <em>Filling the package</em> - if the package initialization succeeded,
the package is filled with data using tileToPackage() function.
@see SequentialFormat

-# <em>Finalizing the package</em> - after filling the package with the
data, the package is finalized with finalizePackage(), which finishes and
closes all opened files. If finalizing succeeds, the package is then ready to
use.

@section AbstractRasterModel_Subclassing Subclassing

@subsection AbstractRasterModel_Subclassing_Read Implementing read support
Minimal function set you have to implement to have working raster model (which
can open only one package at a time (see @ref MultiplePackages feature), has a
few zoom levels, layers and no overlays):
@code
// Tile size. Could be hardcoded or based on currently opened package.
TileSize tileSize() const;

// Zoom levels, area at lowest zoom, list of available layers
std::set<Zoom> zoomLevels() const;
TileArea area() const;
std::vector<std::string> layers() const;

// Function for adding package. As the model supports only one package, this
// function should check whether any package is already opened and close it.
int addPackage(const std::string& filename);

// As the model supports only one package, this function returns either 0 or 1.
int packageCount() const;

// Function for getting tile from package. The function should check if given
// layer and zoom exists in the package and if the coordinates are in area
// for given zoom.
std::string tileFromPackage(const std::string& layer, Zoom z, const TileCoords& coords);
@endcode
Support for overlays can be added just with reimplementing this function:
@code
std::vector<std::string> overlays() const;
@endcode

As said above, the layers and overlays can have translated names. This is done
internally with Translator class. To make translate names, all you have to do is
add a new section(s) to your plugin configuration file, similarly like when
translating @ref PluginMetadata "plugin metadata". For example, if you have
your layers and overlays named @c base, @c satellite and @c labels and you want
to have English and Czech translation, add this to your plugin configuration
%file:
<pre>[translation]
base=Base map
satellite=Satellite view
labels=Labels

[translation/cs_CZ]
base=Základní
satellite=Satelitní
labels=Popisky
</pre>

If you want to have more features than this basic set, you have to expose which
additional features your model supports, for example:
@code
inline int features() const { return ConvertableCoords|MultiplePackages|LoadableFromUrl; }
@endcode
You could also limit some functionality, see @ref SingleLayer, @ref SingleZoom
and @ref WriteOnly features.

Support for <strong>map projections and coordinate conversion</strong> is done
via enabling @ref ConvertableCoords feature and returning pointer to an existing
and configured projection instance, for example:
@code
class MyRasterModel: public AbstractRasterModel {
    public:
        MyRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin): AbstractRasterModel(manager, plugin) {
            // Configure the projection here...
        }

        inline virtual int features() const { return ConvertableCoords; }
        inline virtual const AbstractProjection* projection() const { return &myProjection; }

        //...

    private:
        MyProjection myProjection;
};
@endcode

Support for <strong>multiple currently opened packages</strong> is done via
enabling @ref MultiplePackages feature, function addPackage() should then check
if the new package is compatible with those already opened (e.g. the same tile
size, projection etc.). Then the function must merge layer and overlay list with
the new package, add new zoom levels and enlarge available area. Function
tileFromPackage() then must go through list of all opened packages and look for
the tile in every package until it finds it.
@todo Move this functionality from KompasRasterModel here / create
    AbstractMultiplePackageRasterModel

The model should also be able to <strong>recognize its own packages</strong>.
This is done via enabling @ref SelfRecognizable feature and implementing these
two functions:
@code
// Helper function which returns possible file extensions. It can be then used
// to do first filtering of the file in file dialogs etc.
std::vector<std::string> fileExtensions() const;

// Function which looks at the filename and maybe at first few bytes of the file
// and says whether it supports the file or not. The implementation should be
// as fast as possible, so if it can guess the file format already from
// filename, it shouldn't e.g. parse the whole file and check for errors.
// Everything which is not required to guess support level should be done in
// addPackage() function.
SupportLevel recognizeFile(const std::string& filename, std::istream& file) const;
@endcode

Support for <strong>online maps</strong> is done via enabling
@ref LoadableFromUrl and implementing tileUrl() function. As online maps can be
combined with packages, the model should have an optimized way to return zoom
levels, area, etc. when the online maps are enabled and when not. Example:
@code
MyOnlineRasterModel: public AbstractRasterModel {
    public:
        MyRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin): AbstractRasterModel(manager, plugin) {
            // fill the areaOnline, zoomLevelsOnline, layersOnline variables ...
        }

        inline virtual std::set<Zoom> zoomLevels() const { return online() ? zoomLevelsOnline : zoomLevels; }
        inline virtual TileArea area() const { return online() ? areaOnline : area; }
        inline virtual std::vector<std::string> layers() const { return online() ? layersOnline : layers; }

        virtual int addPackage(const std::string& filename) {
            // operate with area, zoomLevels, layers variables ...
        }

        // ...

    private:
        TileArea area, areaOnline;
        std::vector<Zoom> zoomLevels, zoomLevelsOnline;
        std::vector<std::string> layers, layersOnline;
};
@endcode

There are also two functions which can be reimplemented to provide additional
information about the map: copyright() and packageAttribute().

@subsection AbstractRasterModel_Subclassing_Write Implementing write support
Write support can be implemented via enabling @ref WriteableFormat feature and
optionally another features like @ref MultipleFileFormat, @ref SequentialFormat,
@ref NonConvertableFormat, @ref WriteOnly, @ref SingleLayer, @ref SingleZoom
based on what the file format can and cannot do. You have to implement three
packaging functions:
@code
// Initialize package, check whether the format supports everything what is
// requested (e.g. single layer model will not support multiple layers and
// overlays).
bool initializePackage(const std::string& filename, const TileSize& tileSize, const std::vector<Zoom>& zoomLevels, const TileArea& area, const std::vector<std::string>& layers, const std::vector<std::string>& overlays);

// Set package attribute. If the model doesn't support any of them, this
// function doesn't have to be reimplemented.
bool setPackageAttribute(PackageAttribute type, const std::string& data);

// Add tile to package. If the format is sequential, the function should also
// check whether the tile came in right order (don't rely on user). This
// function should also check whether the coordinates aren't out of range and
// whether the layer is among the layers/overlays defined in
// initializePackage().
bool tileToPackage(const std::string& layer, Zoom z, const TileCoords& coords, const std::string& data);

// Finalize package. This function should be able to finish before all the
// tiles were added.
bool finalizePackage();
@endcode

@todo @c VERSION-0.2 Installing a image filter plugin for modifying tileData
    (compression with pngnq, etc.)
*/
class CORE_EXPORT AbstractRasterModel: public TranslatablePlugin {
    PLUGIN_INTERFACE("cz.mosra.Kompas.Core.AbstractRasterModel/0.1")

    public:
        /**
         * @brief Features provided by map model
         *
         * Every map model must be capable of at least loading maps from
         * package, if not anything else.
         */
        enum Feature {
            /**
             * Map tiles are loadable from URLs.
             * @see tileUrl()
             */
            LoadableFromUrl         = 0x01,

            /**
             * Coordinates can be converted to and from WGS84.
             * @note ConvertableCoords feature doesn't always mean that
             * projection() returns valid AbstractProjection object!
             * @see projection()
             */
            ConvertableCoords       = 0x02,

            /**
             * Multiple packages can be loaded at once.
             * @see addPackage(), tileFromPackage()
             */
            MultiplePackages        = 0x04,

            /**
             * The format supports writing new packages.
             * @see initializePackage(), tileToPackage(), finalizePackage()
             */
            WriteableFormat         = 0x08,

            /**
             * The model is for writing only, thus has empty area, no zoom
             * levels, no layers and opening or recongizing any package will
             * fail.
             */
            WriteOnly               = 0x400,

            /**
             * New packages can be created only from data from model of the
             * same type. The model doesn't check (and cannot check) if the
             * tile data are of the same type, so it is only a hint to the user,
             * otherwise the resulting package can be in undefined state.
             * @see tileToPackage()
             */
            NonConvertableFormat    = 0x10,

            /**
             * Tiles to new packages must be saved in sequential row-major
             * order. If not set, the tiles can be saved in arbitrary order.
             * @see tileToPackage()
             */
            SequentialFormat        = 0x20,

            /**
             * One package is composed from multiple files. Used as hint when
             * saving new package, destination filename should be in clean
             * folder to prevent data loss. The model doesn't check it in any
             * way.
             */
            MultipleFileFormat      = 0x40,

            /**
             * The model can recognize its own packages from given filename and
             * optionally a few first bytes of the file.
             * @see SupportLevel, recognizeFile()
             */
            SelfRecognizable        = 0x80,

            /**
             * The model can have only one layer (no overlays).
             */
            SingleLayer             = 0x100,

            /**
             * The model can have only one zoom level.
             */
            SingleZoom              = 0x200
        };

        /** @brief Package attribute types */
        enum PackageAttribute {
            Filename,       /**< @brief Package filename */
            Name,           /**< @brief Map name */
            Description,    /**< @brief Map description */
            Packager        /**< @brief Map packager */
        };

        /** @brief Support level of given file */
        enum SupportLevel {
            NotSupported,           /**< @brief The model cannot open given file */
            PartiallySupported,     /**< @brief The model can open the file, but doesn't support all its features */
            FullySupported,         /**< @brief The model supports all file features */
            DeprecatedSupport       /**< @brief The model can open the file, but the support is deprecated */
        };

        /** @{ @name Utilites */

        /**
         * @brief Minimum count of tiles to fill given area
         * @param area      %Area size in pixels
         * @return Tile count
         */
        inline Coords<unsigned int> tilesInArea(const Coords<unsigned int>& area) const {
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
        inline virtual int features() const { return 0; }

        /**
         * @brief Possible file extensions
         * @return Vector of possible file extensions (with wildcards). Default
         *      implementation returns empty vector.
         *
         * Returns list of possible file extensions, which can be used for
         * directory listing filtering. This is just a helper, proper checking
         * if the file can be opened should be done in recognizeFile().
         */
        inline virtual std::vector<std::string> fileExtensions() const { return std::vector<std::string>(); }

        /**
         * @brief Recognize given file
         * @param filename      Filename with full path
         * @param file          Read access to the file
         * @return If file type cannot be distinguished or file format is
         *      unsupported, returns NotSupported, if the file can be opened,
         *      but some features can be missing (such as projection), returns
         *      PartiallySupported. If whole specification of the format is
         *      supported, returns FullySupported. Default implementation
         *      returns NotSupported.
         *
         * Checks whether given file can be opened with the model.
         * Reimplementations should return FullySupported only if everything
         * possible is supported, so the application can choose best matching
         * plugin automatically (plugin with full support will be chosen if it
         * exists, otherwise fallback to plugins with partial support).
         */
        inline virtual SupportLevel recognizeFile(const std::string& filename, std::istream& file) const { return NotSupported; }

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
         * @todo @c VERSION-0.2 Depending on coordinates and map layer (to "properly" support Google)
         */
        inline virtual std::string copyright() const { return ""; }

        /*@}*/

        /** @copydoc PluginManager::Plugin::Plugin() */
        AbstractRasterModel(PluginManager::AbstractPluginManager* manager, const std::string& plugin):
            TranslatablePlugin(manager, plugin), _online(false) {}

        /** @{ @name Map parameters */

        /**
         * @brief Whether the map is usable
         *
         * Map is usable when it has at least one layer, one zoom level and
         * non-empty area.
         */
        inline bool isUsable() const {
            return !layers().empty() && !zoomLevels().empty() && area() != TileArea();
        }

        /**
         * @brief Zoom levels
         * @return List of zooms ordered in ascending order (lowest zoom first).
         *
         * All zoom levels provided by map.
         */
        virtual std::set<Zoom> zoomLevels() const = 0;

        /**
         * @brief Map area for lowest zoom
         *
         * Position and size of map area which is available for the lowest
         * available zoom. Map area at greater zoom values can be computed with
         * pow2().
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

        /**
         * @brief Translated layer name
         * @param layer     Layer or overlay returned by layers() or overlays()
         */
        inline const std::string* layerName(const std::string& layer) { return tr(layer); }

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
         * @see tileFromPackage(), AbstractRasterModel::MultiplePackages
         */
        virtual int addPackage(const std::string& filename) = 0;

        /** @brief Count of loaded packages */
        virtual int packageCount() const = 0;

        /**
         * @brief User defined map attributes
         * @param package   Package ID (returned by addPackage)
         * @param type      Map attribute type
         * @return Attribute value. Default implementation returns empty string.
         */
        inline virtual std::string packageAttribute(int package, PackageAttribute type) const { return ""; }

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
         * @brief Get tile data from package
         * @param layer     Map layer or overlay
         * @param z         Zoom level
         * @param coords    Coordinates
         * @return Tile data (image) or empty string, if tile was not found in
         *      any loaded package.
         *
         * Tries to get given tile from all packages in ascending order (first
         * from first package, if not, from second package and so on).
         * @see addPackage(), AbstractRasterModel::MultiplePackages
         */
        virtual std::string tileFromPackage(const std::string& layer, Zoom z, const TileCoords& coords) = 0;

        /*@}*/

        /** @{ @name Saving map data */

        /**
         * @brief Create new package
         * @param filename      Package filename
         * @param tileSize      Tile size
         * @param zoomLevels    Zoom levels
         * @param area          Tile area for lowest zoom
         * @param layers        Map layers
         * @param overlays      Map overlays
         * @return Whether the package was successfully initialized. Default
         *      implementation returns false.
         *
         * Creates new package which can then be filled with tileToPackage().
         * @see finalizePackage(), AbstractRasterModel::WriteableFormat
         */
        inline virtual bool initializePackage(const std::string& filename, const TileSize& tileSize, const std::vector<Zoom>& zoomLevels, const TileArea& area, const std::vector<std::string>& layers, const std::vector<std::string>& overlays) { return false; }

        /**
         * @brief Set package attribute
         * @param type      Attribute type
         * @param data      Attribute data
         * @return Whether the attribute was set. If the package wasn't
         *      initialized with initializePackage(), the attribute is not
         *      supported in the format, or saving was not successful,
         *      returns false. Default implementation returns false.
         */
        inline virtual bool setPackageAttribute(PackageAttribute type, const std::string& data) { return false; }

        /**
         * @brief Save tile to package
         * @param layer     Map layer or overlay
         * @param z         Zoom level
         * @param coords    Coordinates
         * @param data      Tile data
         * @return If new package wasn't initialized with initializePackage() or
         *      the saving was not successful, returns false. Default
         *      implementation returns false.
         * @see initializePackage(), finalizePackage(), AbstractRasterModel::WriteableFormat, AbstractRasterModel::NonConvertableFormat, AbstractRasterModel::SequentialFormat
         */
        inline virtual bool tileToPackage(const std::string& layer, Zoom z, const TileCoords& coords, const std::string& data) { return false; }

        /**
         * @brief Finalize package
         * @return If new package wasn't initialized with initializePackage() or
         *      finalizing was not successful, returns false. Default
         *      implementation returns false.
         *
         * Saves all remaining data to the package and closes it, so it's
         * possible to create another package.
         * @see initializePackage(), tileToPackage(), AbstractRasterModel::WriteableFormat
         */
        inline virtual bool finalizePackage() { return false; }

        /*@}*/

    private:
        bool _online;
};

}}

#endif
