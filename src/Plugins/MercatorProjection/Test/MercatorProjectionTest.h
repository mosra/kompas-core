#ifndef Kompas_Plugins_Test_MercatorProjectionTest_h
#define Kompas_Plugins_Test_MercatorProjectionTest_h
/*
    Copyright © 2007, 2008, 2009, 2010, 2011 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

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
#include "../MercatorProjection.h"

namespace Kompas { namespace Plugins { namespace Test {

class MercatorProjectionTest: public QObject {
    Q_OBJECT

    private:
        MercatorProjection projection;

    private slots:
        void coords_data();
        void coords();
};

}}}

#endif
