#ifndef Kompas_Plugins_KompasRasterModel_h
#define Kompas_Plugins_KompasRasterModel_h
/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>

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
 * @brief Class Kompas::Plugins::KompasRasterModel
 */

#include "AbstractRasterModel.h"

#include "KompasRasterArchiveReader.h"
#include "KompasRasterArchiveMaker.h"

namespace Kompas { namespace Plugins {

/**
 * @brief %Kompas raster model
 *
 * Built-in format for storing offline maps.
 * @todo Document subclassing
 */
class CORE_EXPORT KompasRasterModel: public Core::AbstractRasterModel {
    public:
        /** @copydoc Core::AbstractRasterModel::AbstractRasterModel */
        inline KompasRasterModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""): AbstractRasterModel(manager, plugin), currentPackageZoom(0), currentlyCreatedPackage(0) {
            extensions.push_back("*.conf");
        }

        virtual ~KompasRasterModel();

        inline virtual int features() const { return WriteableFormat|SequentialFormat|MultipleFileFormat|SelfRecognizable; }
        inline virtual std::vector<std::string> fileExtensions() const { return extensions; }
        virtual SupportLevel recognizeFile(const std::string& filename, std::istream& file) const;
        inline virtual Core::TileSize tileSize() const { return _tileSize; }

        inline virtual std::set<Core::Zoom> zoomLevels() const { return _zoomLevels; }
        inline virtual Core::TileArea area() const { return _area; }
        inline virtual std::vector<std::string> layers() const { return _layers; }
        inline virtual std::vector<std::string> overlays() const { return _overlays; }

        /**
         * @copydoc Core::AbstractRasterModel::addPackage()
         *
         * Calls parsePackage(), then expands map zoom levels, map area and
         * available layers with package data.
         */
        virtual int addPackage(const std::string& filename);
        inline virtual int packageCount() const { return packages.size(); }
        virtual std::string packageAttribute(int package, PackageAttribute type) const;
        virtual std::string tileFromPackage(const std::string& layer, Core::Zoom z, const Core::TileCoords& coords);

        virtual bool initializePackage(const std::string& filename, const Core::TileSize& tileSize, const std::vector<Core::Zoom>& zoomLevels, const Core::TileArea& area, const std::vector< std::string>& layers, const std::vector<std::string>& overlays);
        virtual bool setPackageAttribute(PackageAttribute type, const std::string& data);
        virtual bool tileToPackage(const std::string& layer, Core::Zoom z, const Core::TileCoords& coords, const std::string& data);
        virtual bool finalizePackage();

    protected:
        /** @brief Opened package */
        struct Package {
            /** @brief Opened archives for layers in current zoom */
            std::map<std::string, std::vector<KompasRasterArchiveReader*> > archives;

            Core::TileArea area;        /**< @brief Package area */
            std::set<Core::Zoom>
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
         *      If the version is lower than 3, opens @c *.map extension instead
         *      of @c *.kps extension.
         * @param tileId            Tile ID
         * @return Tile data or empty string if archive with given ID is not
         *      valid.
         *
         * Tries to get an tile from archive specified with archiveId (the
         * archive is opened if given id doesn't exist in archives vector). If
         * the archive doesn't contain the tile, calls itself with next
         * archiveId.
         */
        virtual std::string tileFromArchive(const std::string& path, const std::string& layer, Core::Zoom z, std::vector<KompasRasterArchiveReader*>* archives, unsigned int archiveId, int packageVersion, unsigned int tileId);

    private:
        struct CurrentlyCreatedPackage {
            CurrentlyCreatedPackage(const std::string& filename): conf(filename, Utility::Configuration::Truncate), minZoom(0) {}
            Utility::Configuration conf;
            std::string path;
            std::map<std::string, KompasRasterArchiveMaker*> archives;
            Core::TileArea area;
            Core::Zoom minZoom;
        };

        std::vector<std::string> extensions;
        Core::TileSize _tileSize;
        std::set<Core::Zoom> _zoomLevels;
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
