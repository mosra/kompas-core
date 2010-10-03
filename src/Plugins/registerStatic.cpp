#include "PluginManager/AbstractPluginManager.h"
#include "Utility/utilities.h"

int registerCoreStaticPlugins() {
    PLUGIN_IMPORT(OpenStreetMap)
    PLUGIN_IMPORT(MercatorProjection)
    return 1;
} AUTOMATIC_INITIALIZER(registerCoreStaticPlugins)
