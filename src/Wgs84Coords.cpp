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

#include "Wgs84Coords.h"

#include <vector>
#include <sstream>
#include <cmath>

#include "basictypes.h"

using namespace std;

namespace Map2X { namespace Core {

const string Wgs84Coords::format = "\n \n\n°\n'\n\"\nN\nS\nE\nW";

Wgs84Coords::Wgs84Coords(double __lat, double __lon) {
    /** @todo -180 and 180 is the same, which will be converted to other? */
    if(__lon >= -180.0 && __lon <= 180.0 &&
       __lat >= -90.0 && __lat <= 90.0) {
        _lon = __lon;
        _lat = __lat;
        _isValid = true;

    } else {
        _lon = 0;
        _lat = 0;
        _isValid = false;
    }
}

std::string Wgs84Coords::toString(int precision, const string& _format) const {
    /* Don't return anything if coordinates are not valid */
    if(!isValid()) return "";

    /* Parse format string, return empty string on error */
    vector<string> formatters;
    string tmp;
    istringstream in(_format);
    while(getline(in, tmp, '\n'))
        formatters.push_back(tmp);
    if(formatters.size() != 10) return "";

    /* North / south, east / west */
    int ns, ew;
    if(_lat >= 0)   ns = 6;
    else            ns = 7;

    if(_lon >= 0)   ew = 8;
    else            ew = 9;

    double latAbs = abs(_lat);
    double lonAbs = abs(_lon);
    double latMinutes = (latAbs-floor(latAbs))*60;
    double lonMinutes = (lonAbs-floor(lonAbs))*60;
    double latSeconds = (latMinutes-floor(latMinutes))*60;
    double lonSeconds = (lonMinutes-floor(lonMinutes))*60;

    ostringstream out;
    out.setf(ostringstream::fixed, ostringstream::floatfield);
    out << formatters[0];

    /* Latitude */
    out << static_cast<int>(latAbs) << formatters[3]
        << static_cast<int>(latMinutes) << formatters[4];

    out.precision(precision);
    out << latSeconds << formatters[5] << formatters[ns] << formatters[1];

    /* Longtitude */
    out << static_cast<int>(lonAbs) << formatters[3]
        << static_cast<int>(lonMinutes) << formatters[4];

    out.precision(precision);
    out << lonSeconds << formatters[5] << formatters[ew] << formatters[2];

    return out.str();
}

bool operator==(const Map2X::Core::Wgs84Coords& a, const Map2X::Core::Wgs84Coords& b) {
    if(!a.isValid() || !b.isValid())
        return false;

    if(abs(a.latitude() - b.latitude()) < EPSILON && abs(a.longtitude() - b.longtitude()) < EPSILON)
        return true;

    return false;
}

}}
