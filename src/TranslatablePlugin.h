#ifndef Kompas_Core_TranslatablePlugin_h
#define Kompas_Core_TranslatablePlugin_h
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

/** @file
 * @brief Class Kompas::Core::TranslatablePlugin
 */

#include "PluginManager/Plugin.h"
#include "Utility/Translator.h"

namespace Kompas { namespace Core {

/**
 * @brief Plugin with translation support
 *
 * Translated strings for primary/fallback language are stored in plugin
 * configuration file in group named @c translation, translations for another
 * languages are stored in subgroups named after given locale.
 *
 * Translated strings can be retrieved with tr() function, Translator is set to
 * dynamic language, so after calling Translator::setLocale() the translations
 * are updated to new language.
 */
class CORE_EXPORT TranslatablePlugin: public PluginManager::Plugin {
    public:
        /** @copydoc PluginManager::Plugin::Plugin */
        TranslatablePlugin(PluginManager::AbstractPluginManager* manager = 0, const std::string& plugin = ""): Plugin(manager, plugin) {
            if(!configuration()) return;
            const Utility::ConfigurationGroup* g = configuration()->group("translation");
            translator.setPrimary(g, true);
            translator.setFallback(g);
        }

    protected:
        /**
         * @brief Translation for given key
         * @param key       Key name
         */
        inline const std::string* tr(const std::string& key) { return translator.get(key); }

    private:
        Utility::Translator translator;
};

}}

#endif
