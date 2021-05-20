# Install script for directory: F:/quicNew/mvfst/quic

# Set the install prefix
if(NOT DEFINED CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "C:/Program Files/mvfst")
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

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/quic" TYPE FILE FILES "F:/quicNew/mvfst/quic/QuicConstants.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/quicNew/mvfst/vs2019/quic/Debug/mvfst_constants.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/quicNew/mvfst/vs2019/quic/Release/mvfst_constants.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/quicNew/mvfst/vs2019/quic/MinSizeRel/mvfst_constants.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/quicNew/mvfst/vs2019/quic/RelWithDebInfo/mvfst_constants.lib")
  endif()
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/quic" TYPE FILE FILES "F:/quicNew/mvfst/quic/QuicException.h")
endif()

if("x${CMAKE_INSTALL_COMPONENT}x" STREQUAL "xUnspecifiedx" OR NOT CMAKE_INSTALL_COMPONENT)
  if("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/quicNew/mvfst/vs2019/quic/Debug/mvfst_exception.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/quicNew/mvfst/vs2019/quic/Release/mvfst_exception.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/quicNew/mvfst/vs2019/quic/MinSizeRel/mvfst_exception.lib")
  elseif("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    file(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib" TYPE STATIC_LIBRARY FILES "F:/quicNew/mvfst/vs2019/quic/RelWithDebInfo/mvfst_exception.lib")
  endif()
endif()

if(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  include("F:/quicNew/mvfst/vs2019/quic/api/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/client/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/codec/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/common/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/congestion_control/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/fizz/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/flowcontrol/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/handshake/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/happyeyeballs/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/logging/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/loss/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/samples/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/server/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/state/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/tools/cmake_install.cmake")
  include("F:/quicNew/mvfst/vs2019/quic/d6d/cmake_install.cmake")

endif()

