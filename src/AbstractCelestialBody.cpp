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

#include "AbstractCelestialBody.h"

#include <cmath>

#include "constants.h"

using namespace std;

namespace Kompas { namespace Core {

double AbstractCelestialBody::distance(const LatLonCoords& a, const LatLonCoords& b) {
    double _a = equatorialRadius(), _b = polarRadius(), f = flattening();
    double L = (a.longitude()-b.longitude())*PI/180;
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
    return s;
}


}}
