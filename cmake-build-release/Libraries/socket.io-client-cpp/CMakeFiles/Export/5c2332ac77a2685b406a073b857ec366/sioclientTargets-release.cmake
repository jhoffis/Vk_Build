#----------------------------------------------------------------
# Generated CMake target import file for configuration "Release".
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "sioclient::sioclient" for configuration "Release"
set_property(TARGET sioclient::sioclient APPEND PROPERTY IMPORTED_CONFIGURATIONS RELEASE)
set_target_properties(sioclient::sioclient PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_RELEASE "CXX"
  IMPORTED_LOCATION_RELEASE "${_IMPORT_PREFIX}/lib/sioclient.lib"
  )

list(APPEND _cmake_import_check_targets sioclient::sioclient )
list(APPEND _cmake_import_check_files_for_sioclient::sioclient "${_IMPORT_PREFIX}/lib/sioclient.lib" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
