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

#include "constants.h"

using namespace std;

namespace Map2X { namespace Core {

const string Wgs84Coords::format = "\n \n\n°\n'\n\"\nN\nS\nE\nW";

double Wgs84Coords::dmsToDecimal(const std::string& dms) {
    /* Invariant: 'begin' points to end of last non-numeric character sequence in
        actually parsed part of string, 'numbers' stores all found numeric
        sequences. */
    vector<double> numbers;
    size_t begin = 0;
    for(size_t i = 0; i != dms.size(); ++i) {
        /* Numbers 0-9 or period, not moving begin */
        if((dms[i] >= 0x30 && dms[i] <= 0x39) || dms[i] == '.') continue;

        /* If there is already a numeric seuqence, convert it
            to double and store in vector */
        if(begin != i) {
            string numberString = dms.substr(begin, i-begin);

            /* Skipping periods without numbers */
            if(!(numberString[0] == '.' && numberString[numberString.size()-1] == '.')) {

                /* There is already enough number parts => error */
                if(numbers.size() == 3) return -1;

                double number;
                istringstream stream(numberString);
                stream >> number;
                numbers.push_back(number);
            }
        }

        /* Move begin to next character */
        begin=i+1;
    }

    /* Convert last numeric sequence if it is there */
    if(begin != dms.size() && numbers.size() != 3) {
        double number;
        istringstream stream(dms.substr(begin));
        stream >> number;
        numbers.push_back(number);
    }

    /* No numbers found => error */
    if(numbers.size() == 0) return -1;

    double decimal = numbers[0];
    if(numbers.size() > 1) decimal += numbers[1]/60;
    if(numbers.size() > 2) decimal += numbers[2]/3600;

    return decimal;
}

Wgs84Coords::Wgs84Coords(double __lat, double __lon) {
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

    /* Convert to (-180, 180] */
    if(_lon == -180) _lon = 180;
}

Wgs84Coords::Wgs84Coords(const std::string& coords, const std::string& _format): _lat(0), _lon(0), _isValid(false) {
    vector<string> formatters = parseFormatters(_format);
    if(formatters.empty()) return;

    /* Get N/S E/W position */
    size_t nPos = coords.find_first_of(formatters[6]);
    size_t sPos = coords.find_first_of(formatters[7]);
    size_t ePos = coords.find_first_of(formatters[8]);
    size_t wPos = coords.find_first_of(formatters[9]);

    /* N/S and E/W were not found, try to convert it as a
        "configuration value"-format: two signed doubles */
    if(nPos == string::npos && sPos == string::npos && ePos == string::npos && wPos == string::npos) {
        double lat, lon;
        istringstream stream(coords);
        stream >> lat >> lon;

        if(stream.fail()) return;
        _lat = lat; _lon = lon; _isValid = true;
        return;
    }

    /* Positive/negative lat/lon */
    int latMultiplier = 0; int lonMultiplier = 0;

    /* Where latitude number ends */
    size_t half = string::npos;

    /* Decide on N/S */
    if(nPos != string::npos && sPos == string::npos) {
        half = nPos;
        latMultiplier = 1;
    }
    else if(nPos == string::npos && sPos != string::npos) {
        half = sPos;
        latMultiplier = -1;
    } else return;

    /* Decide on E/W, check whether it is after N/S position */
    if(ePos != string::npos && wPos == string::npos && ePos > half)
        lonMultiplier = 1;
    else if(ePos == string::npos && wPos != string::npos && wPos > half)
        lonMultiplier = -1;
    else return;

    double lat = dmsToDecimal(coords.substr(0, half));
    double lon = dmsToDecimal(coords.substr(half+1));
    if(lat == -1 || lon == -1) return;

    /* Get decimal number from latitude and longitude part, make them signed */
    _lat = lat*latMultiplier;
    _lon = lon*lonMultiplier;
    _isValid = true;
}

std::string Wgs84Coords::toString(int precision, bool skipTrailingZeros, const string& _format) const {
    /* Parse format string, return empty string on error */
    vector<string> formatters = parseFormatters(_format);
    if(formatters.empty()) return "";

    /* North / south, east / west */
    int ns, ew;
    if(_lat >= 0)   ns = 6;
    else            ns = 7;

    if(_lon >= 0)   ew = 8;
    else            ew = 9;

    ostringstream out;
    out.setf(ostringstream::fixed, ostringstream::floatfield);
    out << formatters[0];

    angleToString(precision, skipTrailingZeros, formatters, out, _lat, ns);
    angleToString(precision, skipTrailingZeros, formatters, out, _lon, ew);

    return out.str();
}

double Wgs84Coords::distance(const Wgs84Coords& a, const Wgs84Coords& b, double precision) {
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

    s = round(s/precision)*precision; // round to given precision
    return s;
}

vector<string> Wgs84Coords::parseFormatters(const std::string& format) const {
    vector<string> formatters;
    string tmp;
    istringstream in(format);
    while(getline(in, tmp, '\n'))
        formatters.push_back(tmp);

    if(formatters.size() != 10) return vector<string>();
    return formatters;
}

void Wgs84Coords::angleToString(int precision, bool skipTrailingZeros, vector<string>& formatters, ostringstream& out, double angle, int nsew) const {
    double dDegrees, dMinutes;
    int degrees, minutes;
    double seconds;

    if(precision == -2) {
        dDegrees = abs(angle);
        degrees = round(dDegrees);
        minutes = seconds = 0;
    }

    if(precision == -1) {
        dDegrees = abs(angle);
        degrees = floor(dDegrees);
        dMinutes = (dDegrees-floor(dDegrees))*60;
        minutes = round(dMinutes);
        seconds = 0;
    }

    if(precision >= 0) {
        /* Round degrees first to prevent 60" in output */
        int p = pow(10, precision);
        dDegrees = round(abs(angle)*60*60*p)/(60*60*p);

        degrees = floor(dDegrees);
        dMinutes = (dDegrees-floor(dDegrees))*60;
        minutes = floor(dMinutes);
        seconds = round((dMinutes-floor(dMinutes))*60*p)/p;
    }

    /* Write degrees */
    out << degrees << formatters[3];

    /* Write minutes if precision allows them and we aren't skipping trailing zeros */
    if(precision >= -1 && !(skipTrailingZeros && minutes == 0 && seconds == 0))
        out << minutes << formatters[4];

    /* Write seconds if precision allows them and we aren't skipping trailing zeros */
    if(precision >= 0 && !(skipTrailingZeros && seconds == 0)) {
        out.precision(precision);
        out << seconds << formatters[5];
    }

    /* Add N/S and formatter between lat/lon or E/W and formatter after E/W */
    out << formatters[nsew] << formatters[nsew < 8 ? 1 : 2];
}

bool Wgs84Coords::operator==(const Wgs84Coords& other) const {
    if(!isValid() || !other.isValid()) {
        if(!isValid() && !other.isValid()) return true;
        return false;
    }

    if(abs(latitude() - other.latitude()) < EPSILON && abs(longtitude() - other.longtitude()) < EPSILON)
        return true;

    return false;
}

}

#ifndef DOXYGEN_GENERATING_OUTPUT
namespace Utility {

Core::Wgs84Coords ConfigurationValue<Core::Wgs84Coords>::fromString(const string& stringValue, int flags) {
    double lat, lon;
    istringstream stream(stringValue);
    stream >> lat >> lon;

    if(stream.fail()) return Core::Wgs84Coords();
    return Core::Wgs84Coords(lat, lon);
}

string ConfigurationValue<Core::Wgs84Coords>::toString(const Core::Wgs84Coords& value, int flags) {
    if(!value.isValid()) return "0";

    ostringstream stream;
    stream.precision(12);
    stream << value.latitude() << " " << value.longtitude();

    return stream.str();
}

} // namespace Utility
#endif

}
