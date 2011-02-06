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

#include "Wgs84Coords.h"

#include <vector>
#include <sstream>
#include <cmath>

#include "constants.h"

using namespace std;

namespace Kompas { namespace Core {

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

string Wgs84Coords::decimalToDms(double decimal, int precision, bool skipTrailingZeros, const string& _format) {
    vector<string> formatters = parseFormatters(_format);
    if(formatters.empty()) return "";

    ostringstream out;
    out.setf(ostringstream::fixed, ostringstream::floatfield);
    decimalToDms(decimal, precision, skipTrailingZeros, formatters, out);

    return out.str();
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

    /* Add begin formatter */
    out << formatters[0];

    decimalToDms(_lat, precision, skipTrailingZeros, formatters, out);

    /* Add N/S and middle formatter */
    out << formatters[ns] << formatters[1];

    decimalToDms(_lon, precision, skipTrailingZeros, formatters, out);

    /* Add E/W and end formatter */
    out << formatters[ew] << formatters[2];

    return out.str();
}

vector<string> Wgs84Coords::parseFormatters(const std::string& format) {
    vector<string> formatters;
    string tmp;
    istringstream in(format);
    while(getline(in, tmp, '\n'))
        formatters.push_back(tmp);

    if(formatters.size() != 10) return vector<string>();
    return formatters;
}

void Wgs84Coords::decimalToDms(double angle, int precision, bool skipTrailingZeros, const vector<string>& formatters, ostringstream& out) {
    double dDegrees, dMinutes;
    int degrees, minutes;
    double seconds;

    if(precision <= -2) {
        dDegrees = abs(angle);
        degrees = round(dDegrees);
        minutes = seconds = 0;

    } else if(precision == -1) {
        dDegrees = abs(angle);
        degrees = floor(dDegrees);
        dMinutes = (dDegrees-floor(dDegrees))*60;
        minutes = round(dMinutes);
        seconds = 0;

    } else {
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

Debug& operator<<(Debug debug, const Core::Wgs84Coords& value) {
    std::ostringstream o;
    o << "Wgs84Coords(" << value.latitude() << ", " << value.longtitude() << ")";

    return debug << o.str();
}

} // namespace Utility
#endif

}
