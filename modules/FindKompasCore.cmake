# Find KompasCore - Kompas Core handling module for CMake
#
# This module defines:
#
# KOMPAS_CORE_FOUND          - True if Kompas Core library is found
#
# KOMPAS_CORE_INCLUDE_DIR    - Include dir for Kompas Core
# KOMPAS_PLUGINS_INCLUDE_DIR - Include dir for Kompas plugins
#
# KOMPAS_CORE_LIBRARY        - Kompas Core library
# KOMPAS_RC_EXECUTABLE       - Kompas resource compiler executable
#
# KOMPAS_BINARY_INSTALL_DIR              - Binary installation directory
# KOMPAS_LIBRARY_INSTALL_DIR             - Library installation directory
# KOMPAS_CMAKE_MODULE_INSTALL_DIR        - Installation dir for CMake modules
# KOMPAS_CORE_INCLUDE_INSTALL_DIR        - Include installation directory for Core headers
# KOMPAS_PLUGINS_INCLUDE_INSTALL_DIR     - Include installation directory for plugin headers
# KOMPAS_PLUGINS_INSTALL_DIR             - Plugins installation directory
# KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR - Tile model plugins installation directory
# KOMPAS_PLUGINS_PROJECTION_INSTALL_DIR  - Projection plugins installation directory
#

include(KompasMacros)

if (KOMPAS_CORE_INCLUDE_DIR AND KOMPAS_PLUGINS_INCLUDE_DIR AND KOMPAS_CORE_LIBRARY AND KOMPAS_RC_EXECUTABLE)

    # Already in cache
    set(KOMPAS_CORE_FOUND TRUE)

else()
    # Libraries
    find_library(KOMPAS_CORE_LIBRARY KompasCore)

    # RC executable
    find_program(KOMPAS_RC_EXECUTABLE kompas-rc)

    # Paths
    find_path(KOMPAS_CORE_INCLUDE_DIR
        NAMES PluginManager Utility
        PATH_SUFFIXES Kompas/Core
    )
    find_path(KOMPAS_PLUGINS_INCLUDE_DIR
        NAMES MercatorProjection OpenStreetMapRasterModel
        PATH_SUFFIXES Kompas/Plugins
    )

    include(FindPackageHandleStandardArgs)
    find_package_handle_standard_args("Kompas Core" DEFAULT_MSG
        KOMPAS_CORE_INCLUDE_DIR
        KOMPAS_PLUGINS_INCLUDE_DIR
        KOMPAS_CORE_LIBRARY
        KOMPAS_RC_EXECUTABLE
    )

endif()

# Installation dirs
set_parent_scope(KOMPAS_BINARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin)
set_parent_scope(KOMPAS_LIBRARY_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib)
set_parent_scope(KOMPAS_CMAKE_MODULE_INSTALL_DIR ${CMAKE_ROOT}/Modules)
set_parent_scope(KOMPAS_CORE_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/Kompas/Core)
set_parent_scope(KOMPAS_PLUGINS_INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include/Kompas/Plugins)
set_parent_scope(KOMPAS_PLUGINS_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/kompas/plugins)
set_parent_scope(KOMPAS_PLUGINS_PROJECTION_INSTALL_DIR ${KOMPAS_PLUGINS_INSTALL_DIR}/projections)
set_parent_scope(KOMPAS_PLUGINS_RASTERMODEL_INSTALL_DIR ${KOMPAS_PLUGINS_INSTALL_DIR}/rasterModels)