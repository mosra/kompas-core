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

#include "ConfigurationGroup.h"

#include "Configuration.h"

using namespace std;

namespace Map2X { namespace Utility {

template<> bool ConfigurationGroup::value(const string& key, string* value, unsigned int number, int _flags) const {
    unsigned int foundNumber = 0;
    for(vector<Item>::const_iterator it = _items.begin(); it != _items.end(); ++it) {
        if(it->key == key) {
            if(foundNumber++ == number) {
                *value = it->value;
                return true;
            }
        }
    }

    return false;
}

template<> vector<string> ConfigurationGroup::values(const string& key, int _flags) const {
    vector<string> found;

    for(vector<Item>::const_iterator it = _items.begin(); it != _items.end(); ++it)
        if(it->key == key) found.push_back(it->value);

    return found;
}

unsigned int ConfigurationGroup::valueCount(const string& key) const {
    unsigned int count = 0;
    for(vector<Item>::const_iterator it = _items.begin(); it != _items.end(); ++it)
        if(it->key == key) count++;

    return count;
}

template<> bool ConfigurationGroup::setValue(const string& key, const string& value, unsigned int number, int _flags) {
    if(configuration->flags & Configuration::ReadOnly || !(configuration->flags & Configuration::IsValid))
        return false;

    /* Key cannot be empty => this would change comments / empty lines */
    if(key.empty()) return false;

    unsigned int foundNumber = 0;
    for(vector<Item>::iterator it = _items.begin(); it != _items.end(); ++it) {
        if(it->key == key && foundNumber++ == number) {
            it->value = value;
            configuration->flags |= Configuration::Changed;
            return true;
        }
    }

    /* No value with that name was found, add new */
    Item i;
    i.key = key;
    i.value = value;
    _items.push_back(i);

    configuration->flags |= Configuration::Changed;
    return true;
}

template<> bool ConfigurationGroup::addValue(const string& key, const string& value, int _flags) {
    if(configuration->flags & Configuration::ReadOnly || !(configuration->flags & Configuration::IsValid))
        return false;

    /* Key cannot be empty => empty keys are treated as comments / empty lines */
    if(key.empty()) return false;

    /* Check for unique keys */
    if(configuration->flags & Configuration::UniqueKeys) {
        for(vector<Item>::const_iterator it = _items.begin(); it != _items.end(); ++it)
            if(it->key == key) return false;
    }

    Item i;
    i.key = key;
    i.value = value;
    _items.push_back(i);

    configuration->flags |= Configuration::Changed;
    return true;
}

bool ConfigurationGroup::removeValue(const string& key, unsigned int number) {
    if(configuration->flags & Configuration::ReadOnly || !(configuration->flags & Configuration::IsValid))
        return false;

    /* Key cannot be empty => empty keys are treated as comments / empty lines */
    if(key.empty()) return false;

    unsigned int foundNumber = 0;
    for(vector<Item>::iterator it = _items.begin(); it != _items.end(); ++it) {
        if(it->key == key && foundNumber++ == number) {
            _items.erase(it);
            configuration->flags |= Configuration::Changed;
            return true;
        }
    }

    return false;
}

bool ConfigurationGroup::removeAllValues(const std::string& key) {
    if(configuration->flags & Configuration::ReadOnly || !(configuration->flags & Configuration::IsValid))
        return false;

    /** @todo Do it better & faster */
    for(int i = _items.size()-1; i >= 0; --i) {
        if(_items[i].key == key) _items.erase(_items.begin()+i);
    }

    configuration->flags |= Configuration::Changed;
    return true;
}

}}
