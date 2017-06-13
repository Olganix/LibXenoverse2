# Install script for directory: C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem

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
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/Release/OgreRTShaderSystem.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Release" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/Release/OgreRTShaderSystem.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/RelWithDebInfo/OgreRTShaderSystem.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/RelWithDebInfo/OgreRTShaderSystem.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/MinSizeRel/OgreRTShaderSystem.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/MinSizeRel" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/MinSizeRel/OgreRTShaderSystem.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/Debug/OgreRTShaderSystem_d.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Debug" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/Debug/OgreRTShaderSystem_d.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/RTShaderSystem" TYPE FILE FILES
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreRTShaderSystem.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderPrerequisites.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFFPColour.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFFPFog.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFFPLighting.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFFPRenderState.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFFPRenderStateBuilder.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFFPTexturing.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFFPTransform.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFunction.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderFunctionAtom.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderGenerator.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderParameter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderProgram.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderProgramManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderProgramSet.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderProgramWriter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderRenderState.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderSubRenderState.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderExNormalMapLighting.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderExPerPixelLighting.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderExIntegratedPSSM3.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderScriptTranslator.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderMaterialSerializerListener.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderProgramProcessor.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderCGProgramProcessor.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderCGProgramWriter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderGLSLProgramProcessor.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderGLSLProgramWriter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderProgramWriterManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderHLSLProgramProcessor.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Components/RTShaderSystem/include/OgreShaderHLSLProgramWriter.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

