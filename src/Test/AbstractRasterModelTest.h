#ifndef Kompas_Core_Test_AbstractRasterModelTest_h
#define Kompas_Core_Test_AbstractRasterModelTest_h
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

#include <QtCore/QObject>

#include "AbstractRasterModel.h"

namespace Kompas { namespace Core { namespace Test {

class AbstractRasterModelTest: public QObject {
    Q_OBJECT

    private slots:
        void tilesInArea_data();
        void tilesInArea();

    private:
        class TestRasterModel: public AbstractRasterModel {
            public:
                inline TestRasterModel(): AbstractRasterModel(0, "") {}
                inline int addPackage(const std::string &filename) { return -1; }
                TileArea area() const { return TileArea(); }
                virtual std::set<Zoom> zoomLevels() const { return std::set<Zoom>(); }
                virtual std::vector<std::string> layers() const { return std::vector<std::string>(); }
                virtual int packageCount() const { return 0; }
                virtual std::string tileFromPackage(const std::string &layer, Zoom z, const TileCoords &coords) { return ""; }
                virtual TileSize tileSize() const { return TileSize(256,128); }
        };

        TestRasterModel model;
};

}}}

#endif
