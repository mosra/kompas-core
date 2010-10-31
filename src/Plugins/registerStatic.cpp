#include "PluginManager/AbstractPluginManager.h"
#include "Utility/utilities.h"

int registerCoreStaticPlugins() {
    PLUGIN_IMPORT(Map2XRasterModel)
    PLUGIN_IMPORT(OpenStreetMapRasterModel)
    PLUGIN_IMPORT(MercatorProjection)
    return 1;
} AUTOMATIC_INITIALIZER(registerCoreStaticPlugins)
