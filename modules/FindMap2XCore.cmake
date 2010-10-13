# Find Map2XCore - Map2X Core handling module for CMake
#
# This module defines:
#
# MAP2X_CORE_FOUND          - True if Map2X Core library is found
#
# MAP2X_CORE_INCLUDE_DIR    - Include dir for Map2X Core
# MAP2X_PLUGINS_INCLUDE_DIR - Include dir for Map2X plugins
#
# MAP2X_CORE_LIBRARY        - Map2X Core library
# MAP2X_RC_EXECUTABLE       - Map2X resource compiler executable
#
# MAP2X_BINARY_INSTALL_DIR              - Binary installation directory
# MAP2X_LIBRARY_INSTALL_DIR             - Library installation directory
# MAP2X_CMAKE_MODULE_INSTALL_DIR        - Installation dir for CMake modules
# MAP2X_CORE_INCLUDE_INSTALL_DIR        - Include installation directory for Core headers
# MAP2X_PLUGINS_INCLUDE_INSTALL_DIR     - Include installation directory for plugin headers
# MAP2X_PLUGINS_INSTALL_DIR             - Plugins installation directory
# MAP2X_PLUGINS_TILEMODEL_INSTALL_DIR   - Tile model plugins installation directory
# MAP2X_PLUGINS_PROJECTION_INSTALL_DIR  - Projection plugins installation directory
#

include(Map2XMacros)

if (MAP2X_CORE_INCLUDE_DIR AND MAP2X_PLUGINS_INCLUDE_DIR AND MAP2X_CORE_LIBRARY AND MAP2X_RC_EXECUTABLE)

    # Already in cache
    set(MAP2X_CORE_FOUND TRUE)

else()
    # Libraries
    find_library(MAP2X_CORE_LIBRARY Map2XCore)

    # RC executable
    find_program(MAP2X_RC_EXECUTABLE map2x-rc)

    # Paths
    find_path(MAP2X_CORE_INCLUDE_DIR
        NAMES PluginManager Utility
        PATH_SUFFIXES Map2X/Core
    )
    find_path(MAP2X_PLUGINS_INCLUDE_DIR
        NAMES MercatorProjection OpenStreetMapTileModel
        PATH_SUFFIXES Map2X/Plugins
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args("Map2X Core" DEFAULT_MSG
        MAP2X_CORE_INCLUDE_DIR
        MAP2X_PLUGINS_INCLUDE_DIR
        MAP2X_CORE_LIBRARY
        MAP2X_RC_EXECUTABLE
    )

endif()

# Installation dirs
set_parent_scope(MAP2X_BINARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set_parent_scope(MAP2X_LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set_parent_scope(MAP2X_CMAKE_MODULE_INSTALL_DIR ${CMAKE_ROOT}/Modules)
set_parent_scope(MAP2X_CORE_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/Map2X/Core)
set_parent_scope(MAP2X_PLUGINS_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/Map2X/Plugins)
set_parent_scope(MAP2X_PLUGINS_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/map2x/plugins)
set_parent_scope(MAP2X_PLUGINS_PROJECTION_INSTALL_DIR ${MAP2X_PLUGINS_INSTALL_DIR}/projections)
set_parent_scope(MAP2X_PLUGINS_TILEMODEL_INSTALL_DIR ${MAP2X_PLUGINS_INSTALL_DIR}/tileModels)
