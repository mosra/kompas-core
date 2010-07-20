/*
    Copyright © 2007, 2008, 2009, 2010 Vladimír Vondruš <mosra@centrum.cz>
    Copyright © 2010 Jan Dupal <dupal.j@seznam.cz>

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

double Wgs84Coords::distance(const Wgs84Coords& a, const Wgs84Coords& b) {
    /* WGS84 ellipsoid params */
    double _a = 6378137, _b = 6356752.3142,  f = 1/298.257223563;
    double L = (a.longtitude()-b.longtitude())*PI/180;
    double U1 = atan((1-f) * tan(a.latitude()*PI/180));
    double U2 = atan((1-f) * tan(b.latitude()*PI/180));

    double sinU1 = sin(U1), cosU1 = cos(U1);
    double sinU2 = sin(U2), cosU2 = cos(U2);

    double lambda = L, lambdaP, iterLimit = 100;

    double cosSqAlpha, sinSigma, cos2SigmaM, cosSigma, sigma;
    do {
        double sinLambda = sin(lambda), cosLambda = cos(lambda);
        sinSigma = sqrt((cosU2*sinLambda) * (cosU2*sinLambda) +
          (cosU1*sinU2-sinU1*cosU2*cosLambda) * (cosU1*sinU2-sinU1*cosU2*cosLambda));

        if (sinSigma==0) return 0;  // co-incident points

        cosSigma = sinU1*sinU2 + cosU1*cosU2*cosLambda;
        sigma = atan2(sinSigma, cosSigma);
        double sinAlpha = cosU1 * cosU2 * sinLambda / sinSigma;
        cosSqAlpha = 1 - sinAlpha*sinAlpha;
        if(cosSqAlpha == 0) cos2SigmaM = 0;  // equatorial line: cosSqAlpha=0 (§6)
        else cos2SigmaM = cosSigma - 2*sinU1*sinU2/cosSqAlpha;

        double C = f/16*cosSqAlpha*(4+f*(4-3*cosSqAlpha));
        lambdaP = lambda;
        lambda = L + (1-C) * f * sinAlpha *
          (sigma + C*sinSigma*(cos2SigmaM+C*cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)));
    } while (abs(lambda-lambdaP) > 1e-12 && --iterLimit>0);

    if (iterLimit==0) return -1;  // formula failed to converge

    double uSq = cosSqAlpha * (_a*_a - _b*_b) / (_b*_b);
    double A = 1 + uSq/16384*(4096+uSq*(-768+uSq*(320-175*uSq)));
    double B = uSq/1024 * (256+uSq*(-128+uSq*(74-47*uSq)));
    double deltaSigma = B*sinSigma*(cos2SigmaM+B/4*(cosSigma*(-1+2*cos2SigmaM*cos2SigmaM)-
    B/6*cos2SigmaM*(-3+4*sinSigma*sinSigma)*(-3+4*cos2SigmaM*cos2SigmaM)));
    double s = _b*A*(sigma-deltaSigma);

    s = round(s*1000.)/1000.; // round to 1mm precision, convert to meters
    return s;
}

}}
