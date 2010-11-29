#ifndef Map2X_Plugins_Test_Map2XRasterModelTest_h
#define Map2X_Plugins_Test_Map2XRasterModelTest_h
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

#include <QtCore/QObject>

#include "Map2XRasterModel/Map2XRasterModel.h"

namespace Map2X { namespace Plugins { namespace Test {

class Map2XMultiRasterModel: public Map2XRasterModel {
    public:
        Map2XMultiRasterModel(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""):
            Map2XRasterModel(manager, plugin) {}

        inline virtual int features() const { return Map2XRasterModel::features()|MultiplePackages; }
        inline virtual Core::TileSize tileSize() const { return Core::TileSize(256, 256); }
};

class Map2XMultiRasterModelTest: public QObject {
    Q_OBJECT

    private slots:
        void initialization();
        void expansion();
        void get();

    private:
        Map2XMultiRasterModel model;
};

}}}

#endif
