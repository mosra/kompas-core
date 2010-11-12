#ifndef Map2X_Plugins_Map2XRasterModel_h
#define Map2X_Plugins_Map2XRasterModel_h
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
 * @brief Class Map2X::Plugins::Map2XRasterModel
 */

#include "AbstractRasterModel.h"

#include "Map2XRasterArchiveReader.h"
#include "Map2XRasterArchiveMaker.h"

namespace Map2X { namespace Plugins {

/**
 * @brief %Map2X raster model
 *
 * Built-in format for storing offline maps.
 * @todo Document subclassing
 */
class Map2XRasterModel: public Core::AbstractRasterModel {
    public:
        /** @copydoc Core::AbstractRasterModel::AbstractRasterModel */
        inline Map2XRasterModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            AbstractRasterModel(manager, plugin), _zoomStep(0), currentPackageZoom(0), currentlyCreatedPackage(0) {}
        inline virtual ~Map2XRasterModel() { closePackages(); }

        inline virtual int features() const { return MultiplePackages|WriteableFormat|SequentialFormat|SelfRecognizable; }
        virtual SupportLevel recognizeFile(const std::string& filename, std::istream& file) const;
        inline virtual Core::TileSize tileSize() const { return _tileSize; }

        virtual std::string attribute(Attribute type, int package) const;
        inline virtual double zoomStep() const { return _zoomStep; }
        inline virtual std::vector<Core::Zoom> zoomLevels() const { return _zoomLevels; }
        inline virtual Core::TileArea area() const { return _area; }
        inline virtual std::vector<std::string> layers() const { return _layers; }
        inline virtual std::vector<std::string> overlays() const { return _overlays; }

        /**
         * @copydoc AbstractRasterModel::addPackage()
         *
         * Calls parsePackage(), then expands map zoom levels, map area and
         * available layers with package data.
         */
        virtual int addPackage(const std::string& filename);
        virtual std::string tileFromPackage(const std::string& layer, Core::Zoom z, const Map2X::Core::TileCoords& coords);

        virtual bool initializePackage(const std::string& filename, Core::TileSize tileSize, std::vector<Core::Zoom> zoomLevels, double zoomStep, Core::TileArea area, std::vector<std::string> layers, std::vector<std::string> overlays);
        virtual bool setPackageAttribute(Attribute type, const std::string& data);
        virtual bool tileToPackage(const std::string& layer, Core::Zoom z, const Map2X::Core::TileCoords& coords, const std::string& data);
        virtual bool finalizePackage();

    protected:
        /** @brief Opened package */
        struct Package {
            /** @brief Opened archives for layers in current zoom */
            std::map<std::string, std::vector<Map2XRasterArchiveReader*> > archives;

            Core::TileArea area;        /**< @brief Package area */
            std::vector<Core::Zoom>
                zoomLevels;             /**< @brief Package zoom levels */
            std::vector<std::string>
                layers,                 /**< @brief Package layers */
                overlays;               /**< @brief Package overlays */
            std::string filename,       /**< @brief Package filename */
                name,                   /**< @brief Package name */
                description,            /**< @brief Package description */
                packager;               /**< @brief Packager name */
            int version;                /**< @brief Package version */
        };

        /**
         * @brief Parse package configuration file
         * @param conf              Configuration file
         * @return Pointer to new package or 0, if package is invalid.
         *
         * Called from addPackage(). Checks configuration file version and
         * fills in package data.
         */
        virtual Package* parsePackage(const Utility::Configuration* conf);

        /**
         * @brief Get tile from given archive
         * @param path              Path to package root
         * @param layer             Map layer
         * @param z                 Zoom
         * @param archives          Pointer to vector of archives for given map layer
         * @param archiveId         ID of archive where the tile should be
         * @param packageVersion    Package version (from Package::version).
         *      Not used in this function, but can be used in subclasses for
         *      distincing package versions.
         * @return Tile data or empty string if archive with given ID is not
         *      valid.
         *
         * Tries to get an tile from archive specified with archiveId (the
         * archive is opened if given id doesn't exist in archives vector). If
         * the archive doesn't contain the tile, calls itself with next
         * archiveId.
         */
        virtual std::string tileFromArchive(const std::string& path, const std::string& layer, Core::Zoom z, std::vector<Map2XRasterArchiveReader*>* archives, unsigned int archiveId, int packageVersion, unsigned int tileId);

    private:
        struct CurrentlyCreatedPackage {
            CurrentlyCreatedPackage(const std::string& filename): conf(filename, Utility::Configuration::Truncate) {}
            Utility::Configuration conf;
            std::string path;
            std::map<std::string, Map2XRasterArchiveMaker*> archives;
            Core::TileArea area;
            Core::Zoom minZoom;
            double zoomStep;
        };

        Core::TileSize _tileSize;
        double _zoomStep;
        std::vector<Core::Zoom> _zoomLevels;
        Core::TileArea _area;
        std::vector<std::string> _layers, _overlays;
        Core::Zoom currentPackageZoom;
        std::vector<Package*> packages;

        CurrentlyCreatedPackage* currentlyCreatedPackage;

        void closeArchives();
        void closePackages();
};

}}

#endif
