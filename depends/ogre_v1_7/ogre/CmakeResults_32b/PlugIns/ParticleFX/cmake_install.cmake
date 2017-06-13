# Install script for directory: C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX

# Set the install prefix
IF(NOT DEFINED CMAKE_INSTALL_PREFIX)
  SET(CMAKE_INSTALL_PREFIX "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/")
ENDIF(NOT DEFINED CMAKE_INSTALL_PREFIX)
STRING(REGEX REPLACE "/$" "" CMAKE_INSTALL_PREFIX "${CMAKE_INSTALL_PREFIX}")

# Set the install configuration name.
IF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)
  IF(BUILD_TYPE)
    STRING(REGEX REPLACE "^[^A-Za-z0-9_]+" ""
           CMAKE_INSTALL_CONFIG_NAME "${BUILD_TYPE}")
  ELSE(BUILD_TYPE)
    SET(CMAKE_INSTALL_CONFIG_NAME "Release")
  ENDIF(BUILD_TYPE)
  MESSAGE(STATUS "Install configuration: \"${CMAKE_INSTALL_CONFIG_NAME}\"")
ENDIF(NOT DEFINED CMAKE_INSTALL_CONFIG_NAME)

# Set the component getting installed.
IF(NOT CMAKE_INSTALL_COMPONENT)
  IF(COMPONENT)
    MESSAGE(STATUS "Install component: \"${COMPONENT}\"")
    SET(CMAKE_INSTALL_COMPONENT "${COMPONENT}")
  ELSE(COMPONENT)
    SET(CMAKE_INSTALL_COMPONENT)
  ENDIF(COMPONENT)
ENDIF(NOT CMAKE_INSTALL_COMPONENT)

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/Release/Plugin_ParticleFX.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Release" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/Release/Plugin_ParticleFX.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/RelWithDebInfo/Plugin_ParticleFX.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/RelWithDebInfo/Plugin_ParticleFX.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/MinSizeRel/Plugin_ParticleFX.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/MinSizeRel" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/MinSizeRel/Plugin_ParticleFX.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug/opt" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/Debug/Plugin_ParticleFX_d.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Debug" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/Debug/Plugin_ParticleFX_d.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/Plugins/ParticleFX" TYPE FILE FILES
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreAreaEmitter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreBoxEmitterFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreBoxEmitter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreColourFaderAffector2.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreColourFaderAffectorFactory2.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreColourFaderAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreColourFaderAffector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreColourImageAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreColourImageAffector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreColourInterpolatorAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreColourInterpolatorAffector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreCylinderEmitterFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreCylinderEmitter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreDeflectorPlaneAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreDeflectorPlaneAffector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreDirectionRandomiserAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreDirectionRandomiserAffector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreEllipsoidEmitterFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreEllipsoidEmitter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreHollowEllipsoidEmitterFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreHollowEllipsoidEmitter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreLinearForceAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreLinearForceAffector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreParticleFXPlugin.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreParticleFXPrerequisites.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgrePointEmitterFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgrePointEmitter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreRingEmitterFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreRingEmitter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreRotationAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreRotationAffector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreScaleAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/PlugIns/ParticleFX/include/OgreScaleAffector.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

