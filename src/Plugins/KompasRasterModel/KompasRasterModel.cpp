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

#include "KompasRasterModel.h"

#include <algorithm>

#include "Utility/Directory.h"
#include "Utility/Debug.h"

using namespace std;
using namespace Corrade::Utility;
using namespace Kompas::Core;

PLUGIN_REGISTER(KompasRasterModel, Kompas::Plugins::KompasRasterModel,
                "cz.mosra.Kompas.Core.AbstractRasterModel/0.2")

namespace Kompas { namespace Plugins {

KompasRasterModel::~KompasRasterModel() {
    /* Close all opened packages */
    closePackages();

    /* If an package creation was in progress, delete the object */
    if(currentlyCreatedPackage)
        delete currentlyCreatedPackage;
}

AbstractRasterModel::SupportLevel KompasRasterModel::recognizeFile(const std::string& filename, istream& file) const {
    if(filename.substr(filename.size()-5) != ".conf") return NotSupported;

    Configuration conf(file, Configuration::ReadOnly);

    /* Check configuration file version */
    if(conf.value<int>("version") != 3) return NotSupported;

    /* Generic model fully supportes packages created with generic model and
        partially supports everything else */
    if(plugin().empty() || plugin() == "KompasRasterModel") {
        if(conf.value<string>("model") == "KompasRasterModel")
            return FullySupported;

        else return PartiallySupported;

    /* If this is not generic model, it (fully) supports only packages created
        with the same model */
    } else if(plugin() == conf.value<string>("model"))
        return FullySupported;

    /* Not anything else */
    else return NotSupported;
}

string KompasRasterModel::packageAttribute(int package, PackageAttribute type) const {
    if(package < 0 || (size_t) package >= packages.size()) return "";

    string key;
    switch(type) {
        case Filename:      return packages[package]->filename;
        case Name:          return packages[package]->name;
        case Description:   return packages[package]->description;
        case Packager:      return packages[package]->packager;
        default:            return "";
    }
}

int KompasRasterModel::addPackage(const string& filename) {
    /* Don't add another package, if there is already one and multiple packages
       are not supported */
    if(!packages.empty() && !(features() & MultiplePackages)) return -1;

    Configuration conf(filename);
    if(!conf.isValid()) return -1;
    Package* p = parsePackage(&conf);
    if(!p) return -1;

    /* If this is first opened package, set everything to this package */
    if(packages.empty()) {
        _area = p->area;
        _layers = p->layers;
        _overlays = p->overlays;
        _zoomLevels = p->zoomLevels;

    /* Else merge with current */
    } else {

        /* Package area in current minimal zoom */
        TileArea packageArea;

        /* If package minimal zoom is lower than current minimal zoom,
            divide current area */
        if(*p->zoomLevels.begin() < *_zoomLevels.begin()) {
            unsigned int divisor = pow2(*_zoomLevels.begin()-*p->zoomLevels.begin());
            TileArea newArea;

            /* Expand to largest possible, don't crop anything */
            newArea.x = _area.x/divisor;
            newArea.y = _area.y/divisor;
            newArea.w = (_area.x+_area.w)/divisor-newArea.x + ((_area.x+_area.w)%divisor == 0 ? 0 : 1);
            newArea.h = (_area.y+_area.h)/divisor-newArea.y + ((_area.y+_area.h)%divisor == 0 ? 0 : 1);

            _area = newArea;
            packageArea = p->area;

        /* If package minimal zoom is greater than current minimal zoom,
            divide package area */
        } else if(*p->zoomLevels.begin() > *_zoomLevels.begin()) {
            unsigned int divisor = pow2(*p->zoomLevels.begin()-*_zoomLevels.begin());

            /* Expand to largest possible, don't crop anything */
            packageArea.x = p->area.x/divisor;
            packageArea.y = p->area.y/divisor;
            packageArea.w = (p->area.x+p->area.w)/divisor-packageArea.x + ((p->area.x+p->area.w)%divisor == 0 ? 0 : 1);
            packageArea.h = (p->area.y+p->area.h)/divisor-packageArea.y + ((p->area.y+p->area.h)%divisor == 0 ? 0 : 1);

        /* If they are same, nothing to do */
        } else packageArea = p->area;

        /* Both areas are for the same zoom (and thus comparable), now
            expand map parameters to new package */

        if(packageArea.x < _area.x) {
            _area.w = (_area.x+_area.w)-packageArea.x;
            _area.x = packageArea.x;
        }
        if(packageArea.y < _area.y) {
            _area.h = (_area.y+_area.h)-packageArea.y;
            _area.y = packageArea.y;
        }
        if(packageArea.x+packageArea.w > _area.x+_area.w)
            _area.w = packageArea.x+packageArea.w-_area.x;
        if(packageArea.y+packageArea.h > _area.y+_area.h)
            _area.h = packageArea.y+packageArea.h-_area.y;

        /* Merge layers, overlays and zoom levels */

        vector<string> l;
        set_union(p->layers.begin(), p->layers.end(), _layers.begin(), _layers.end(), inserter(l, l.begin()));
        swap(_layers, l);

        vector<string> o;
        set_union(p->overlays.begin(), p->overlays.end(), _overlays.begin(), _overlays.end(), inserter(o, o.begin()));
        swap(_overlays, o);

        _zoomLevels.insert(p->zoomLevels.begin(), p->zoomLevels.end());
    }

    packages.push_back(p);

    return packages.size()-1;
}

string KompasRasterModel::tileFromPackage(const string& layer, Zoom z, const TileCoords& coords) {
    /* Getting tiles from another zoom level, close all opened archives */
    if(z != currentPackageZoom) closeArchives();

    for(vector<Package*>::iterator package = packages.begin(); package != packages.end(); ++package) {
        /* If the zoom level is not in current package, go to next package */
        set<Zoom>::const_iterator foundZoom = (*package)->zoomLevels.find(z);
        if(foundZoom == (*package)->zoomLevels.end()) continue;

        /* Multiply tile area for current zoom level */
        TileArea area = (*package)->area*pow2(z-*(*package)->zoomLevels.begin());

        /* If the coordinates are not in current package area, go to next */
        if(coords.x < area.x || coords.x >= area.x+area.w ||
           coords.y < area.y || coords.y >= area.y+area.h)
            continue;

        /* If the layer is not in current package, go to next package */
        vector<string>::const_iterator foundLayer = find((*package)->layers.begin(), (*package)->layers.end(), layer);
        if(foundLayer == (*package)->layers.end()) {
            foundLayer = find((*package)->overlays.begin(), (*package)->overlays.end(), layer);
            if(foundLayer == (*package)->overlays.end()) continue;
        }

        /* Get archive with given layer or open new */
        map<string, vector<KompasRasterArchiveReader*> >::iterator archives = (*package)->archives.find(layer);
        if(archives == (*package)->archives.end()) {
            (*package)->archives.insert(make_pair(layer, vector<KompasRasterArchiveReader*>()));

            archives = (*package)->archives.find(layer);
        }

        /* Recursively find the tile in archives */
        return tileFromArchive(Directory::path((*package)->filename), layer, z, &archives->second, 0, (*package)->version, area.w*(coords.y-area.y)+(coords.x-area.x));
    }

    /* Not found in any package, return empty string */
    return "";
}

KompasRasterModel::Package* KompasRasterModel::parsePackage(const Configuration* conf) {
    /* Check package version */
    if(conf->value<int>("version") != 3) return 0;

    /* Allow opening everything in generic model and only packages with the
        same name in subclassed plugins */
    if(!plugin().empty() && plugin() != "KompasRasterModel" && plugin() != conf->value<string>("model"))
        return 0;

    /* If tile size is already set, check if the package has the same */
    if(tileSize() != TileSize() && tileSize() != conf->value<TileSize>("tileSize"))
        return 0;

    Package* p = new Package;
    p->version = 3;
    p->filename = conf->filename();
    p->name = conf->value<string>("name");
    p->description = conf->value<string>("description");
    p->packager = conf->value<string>("packager");
    p->area = conf->value<TileArea>("area");
    vector<Zoom> z = conf->values<Zoom>("zoom");
    p->zoomLevels.insert(z.begin(), z.end());
    p->layers = conf->values<string>("layer");
    p->overlays = conf->values<string>("overlay");

    /* The package should have non-empty area, at least one zoom level and layer */
    if(p->area == TileArea() || p->zoomLevels.empty() || p->layers.empty()) {
        delete p;
        return 0;
    }

    /* Everything should be OK now. Set tile size if is not set yet. */
    if(tileSize() == TileSize()) _tileSize = conf->value<TileSize>("tileSize");

    return p;
}

bool KompasRasterModel::initializePackage(const string& filename, const TileSize& tileSize, const vector<Zoom>& zoomLevels, const TileArea& area, const vector<string>& layers, const vector<string>& overlays) {
    if(currentlyCreatedPackage != 0) return false;

    std::string path = Directory::path(filename);

    /* Create path, if it doesn't exist */
    if(!Directory::mkpath(path)) {
        Error() << "Cannot create package directory" << path;
        return false;
    }

    /* Check if there is non-zero area, at least one zoom level and one layer */
    if(zoomLevels.empty() || !area.w || !area.h || layers.empty()) return false;

    /* Sort zoom level array */
    vector<Zoom> zoomLevelsSorted = zoomLevels;
    sort(zoomLevelsSorted.begin(), zoomLevelsSorted.end());

    /* Fill in configuration file */
    currentlyCreatedPackage = new CurrentlyCreatedPackage(filename);
    currentlyCreatedPackage->conf.setValue("version", 3);
    /** @todo Test model name in subclasses */
    currentlyCreatedPackage->conf.setValue<string>("model", plugin().empty() ? "KompasRasterModel" : plugin());
    currentlyCreatedPackage->conf.setValue("tileSize", tileSize);
    currentlyCreatedPackage->conf.setValue("area", area);

    for(vector<Zoom>::const_iterator it = zoomLevelsSorted.begin(); it != zoomLevelsSorted.end(); ++it)
        currentlyCreatedPackage->conf.addValue("zoom", *it);

    for(vector<string>::const_iterator it = layers.begin(); it != layers.end(); ++it)
        currentlyCreatedPackage->conf.addValue("layer", *it);

    for(vector<string>::const_iterator it = overlays.begin(); it != overlays.end(); ++it)
        currentlyCreatedPackage->conf.addValue("overlay", *it);

    /* Save frequently used values */
    currentlyCreatedPackage->path = path;
    currentlyCreatedPackage->area = area;
    currentlyCreatedPackage->minZoom = zoomLevelsSorted[0];

    return true;
}

bool KompasRasterModel::setPackageAttribute(KompasRasterModel::PackageAttribute type, const std::string& data) {
    if(!currentlyCreatedPackage) return false;

    string attribute;
    switch(type) {
        case Name:          attribute = "name";         break;
        case Description:   attribute = "description";  break;
        case Packager:      attribute = "packager";     break;
        default:            return false;
    }

    currentlyCreatedPackage->conf.setValue(attribute, data);
    return true;
}

bool KompasRasterModel::tileToPackage(const string& layer, Zoom z, const TileCoords& coords, const string& data) {
    if(!currentlyCreatedPackage) return false;

    /* Archive prefix */
    ostringstream prefix;
    prefix << layer << '/' << z;

    /* Compute total count of tiles in current zoom level */
    TileArea area = currentlyCreatedPackage->area*pow2(z-currentlyCreatedPackage->minZoom);

    /* Try to find archive with that prefix, otherwise create new */
    map<string, KompasRasterArchiveMaker*>::iterator found = currentlyCreatedPackage->archives.find(prefix.str());
    if(found == currentlyCreatedPackage->archives.end()) {
        /* Make directory for given layer, if not exists */
        if(!Directory::mkpath(Directory::join(currentlyCreatedPackage->path, layer))) {
            Error() << "Cannot create zoom level directory" << Directory::join(currentlyCreatedPackage->path, layer);
            return false;
        }

        unsigned int total = area.w*area.h;
        KompasRasterArchiveMaker* maker = new KompasRasterArchiveMaker(Directory::join(currentlyCreatedPackage->path, prefix.str()), 3, total);

        found = currentlyCreatedPackage->archives.insert(make_pair(prefix.str(), maker)).first;
    }

    /* Tile came out of order */
    if((coords.y-area.y)*area.w+(coords.x-area.x) != found->second->tileCount()) {
        Error() << "Tile came out of order, expected" << TileCoords(found->second->tileCount()%area.w, found->second->tileCount()/area.w) << "got" << coords-TileCoords(area.x, area.y);
        return false;
    }

    /* Append tile */
    int ret = found->second->append(data);
    if(ret == KompasRasterArchiveMaker::Ok || ret == KompasRasterArchiveMaker::NextFile) return true;

    Debug d;
    d << "Cannot append tile to the package:";
    switch(ret) {
        case KompasRasterArchiveMaker::VersionError:
            d << "Unsupported archive version."; break;
        case KompasRasterArchiveMaker::FileError:
            d << "Cannot open the file for writing."; break;
        case KompasRasterArchiveMaker::WriteError:
            d << "Cannot write to the file."; break;
        case KompasRasterArchiveMaker::TotalMismatch:
            d << "Tile count mismatch, file created for" << found->second->tileCount() << "and adding tile " << coords.y*area.w+coords.y; break;
    }

    return false;
}

bool KompasRasterModel::finalizePackage() {
    if(!currentlyCreatedPackage) return false;

    /* Delete all makers */
    for(map<string, KompasRasterArchiveMaker*>::iterator it = currentlyCreatedPackage->archives.begin(); it != currentlyCreatedPackage->archives.end(); ++it)
        delete it->second;

    /* Everything should be automagically saved & finished upon destruction */
    delete currentlyCreatedPackage;
    currentlyCreatedPackage = 0;

    return true;
}

string KompasRasterModel::tileFromArchive(const string& path, const string& layer, Zoom z, vector<KompasRasterArchiveReader*>* archives, unsigned int archiveId, int packageVersion, unsigned int tileId) {
    /* In the archive vector could be only one invalid archive, placed at the end */

    /* Try to open next archive if available, if not, return empty string */
    if(archiveId == archives->size()) {

        /* Opening first archive or there could be another archive */
        if(archiveId == 0 || archives->front()->total() > archives->back()->end()) {

            /* Filename is in format zoom-archiveId.map */
            ostringstream filename;
            filename << z;
            if(archiveId > 0) filename << '-' << archiveId;
            if(packageVersion < 3) filename << ".map";
            else filename << ".kps";

            /* If the archive is invalid, exit */
            archives->push_back(new KompasRasterArchiveReader(Directory::join(Directory::join(path, layer), filename.str())));
            if(!archives->back()->isValid()) return "";

        } else return "";
    }

    KompasRasterArchiveReader* archive = (*archives)[archiveId];

    /* Archive is invalid or tile is not in the package at all */
    if(!archive->isValid() || tileId >= archive->total())
        return "";

    /* Tile is in current archive, return it */
    if(tileId >= archive->begin() && tileId < archive->end())
        return archive->get(tileId);

    /* The tile is not in current archive, search for it in the next archive */
    return KompasRasterModel::tileFromArchive(path, layer, z, archives, ++archiveId, packageVersion, tileId);
}

void KompasRasterModel::closeArchives() {
    for(vector<Package*>::iterator package = packages.begin(); package != packages.end(); ++package) {
        for(map<string, vector<KompasRasterArchiveReader*> >::iterator layer = (*package)->archives.begin(); layer != (*package)->archives.end(); ++layer) {
            for(vector<KompasRasterArchiveReader*>::iterator archive = layer->second.begin(); archive != layer->second.end(); ++archive)
                delete *archive;
        }

        (*package)->archives.clear();
    }
}

void KompasRasterModel::closePackages() {
    for(vector<Package*>::iterator package = packages.begin(); package != packages.end(); ++package) {
        for(map<string, vector<KompasRasterArchiveReader*> >::iterator layer = (*package)->archives.begin(); layer != (*package)->archives.end(); ++layer) {
            for(vector<KompasRasterArchiveReader*>::iterator archive = layer->second.begin(); archive != layer->second.end(); ++archive)
                delete *archive;
        }

        (*package)->archives.clear();
        delete *package;
    }
}

}}
