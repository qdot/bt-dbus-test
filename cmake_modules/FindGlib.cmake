# - Try to find Glib-2.0 (with gobject)
# Once done, this will define
#
#  Glib_FOUND - system has Glib
#  Glib_INCLUDE_DIRS - the Glib include directories
#  Glib_LIBRARIES - link these to use Glib

# include(LibFindMacros)

# # Use pkg-config to get hints about paths
# libfind_pkg_check_modules(Glib_PKGCONF glib-2.0)

# Main include dir
find_path(Glib_INCLUDE_DIR
  NAMES glib.h
  PATHS ${Glib_PKGCONF_INCLUDE_DIRS}
  PATH_SUFFIXES glib-2.0
)

# Glib-related libraries also use a separate config header, which is in lib dir
find_path(GlibConfig_INCLUDE_DIR
  NAMES glibconfig.h
  PATHS ${Glib_PKGCONF_INCLUDE_DIRS} /usr /usr/lib
  PATH_SUFFIXES x86_64-linux-gnu/glib-2.0/include
)

# Finally the library itself
find_library(Glib_LIBRARY
  NAMES glib-2.0
  PATHS ${Glib_PKGCONF_LIBRARY_DIRS}
)

