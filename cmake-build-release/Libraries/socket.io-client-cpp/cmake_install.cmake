# Install script for directory: C:/Users/jonah/Vk_Build/Libraries/socket.io-client-cpp

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files (x86)/Vulkan")
endif()
string(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
if(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  if(BUILD_TYPE)
    string(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  else()
    set(CMAKE_INSTALL_CONFIG_NAME "Release")
  endif()
  message(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
endif()

# Set the component getting installed.
if(NOT CMAKE_INSTALL_COMPONENT)
  if(COMPONENT)
    message(STATUS "Install component: \"${COMPONENT}\"")
    set(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  else()
    set(CMAKE_INSTALL_COMPONENT)
  endif()
endif()

# Is this installation the result of a crosscompile?
if(NOT DEFINED CMAKE_CROSSCOMPILING)
  set(CMAKE_CROSSCOMPILING "FALSE")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include" TYPE FILE FILES
    "C:/Users/jonah/Vk_Build/Libraries/socket.io-client-cpp/src/sio_client.h"
    "C:/Users/jonah/Vk_Build/Libraries/socket.io-client-cpp/src/sio_message.h"
    "C:/Users/jonah/Vk_Build/Libraries/socket.io-client-cpp/src/sio_socket.h"
    )
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "C:/Users/jonah/Vk_Build/cmake-build-release/Libraries/socket.io-client-cpp/sioclient.lib")
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  if(EXISTS "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/sioclient/sioclientTargets.cmake")
    file(DIFFERENT _cmake_export_file_changed FILES
         "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/sioclient/sioclientTargets.cmake"
         "C:/Users/jonah/Vk_Build/cmake-build-release/Libraries/socket.io-client-cpp/CMakeFiles/Export/5c2332ac77a2685b406a073b857ec366/sioclientTargets.cmake")
    if(_cmake_export_file_changed)
      file(GLOB _cmake_old_config_files "$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/sioclient/sioclientTargets-*.cmake")
      if(_cmake_old_config_files)
        string(REPLACE ";" ", " _cmake_old_config_files_text "${_cmake_old_config_files}")
        message(STATUS "Old export file \"$ENV{DESTDIR}${CMAKE_INSTALL_PREFIX}/lib/cmake/sioclient/sioclientTargets.cmake\" will be replaced.  Removing files [${_cmake_old_config_files_text}].")
        unset(_cmake_old_config_files_text)
        file(REMOVE ${_cmake_old_config_files})
      endif()
      unset(_cmake_old_config_files)
    endif()
    unset(_cmake_export_file_changed)
  endif()
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/sioclient" TYPE FILE FILES "C:/Users/jonah/Vk_Build/cmake-build-release/Libraries/socket.io-client-cpp/CMakeFiles/Export/5c2332ac77a2685b406a073b857ec366/sioclientTargets.cmake")
  if(CMAKE_INSTALL_CONFIG_NAME MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/sioclient" TYPE FILE FILES "C:/Users/jonah/Vk_Build/cmake-build-release/Libraries/socket.io-client-cpp/CMakeFiles/Export/5c2332ac77a2685b406a073b857ec366/sioclientTargets-release.cmake")
  endif()
endif()

if(CMAKE_INSTALL_COMPONENT STREQUAL "Unspecified" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/cmake/sioclient" TYPE FILE FILES
    "C:/Users/jonah/Vk_Build/cmake-build-release/Libraries/socket.io-client-cpp/sioclient/sioclientConfig.cmake"
    "C:/Users/jonah/Vk_Build/cmake-build-release/Libraries/socket.io-client-cpp/sioclient/sioclientConfigVersion.cmake"
    "C:/Users/jonah/Vk_Build/cmake-build-release/Libraries/socket.io-client-cpp/sioclient/sioclientTargets.cmake"
    )
endif()

