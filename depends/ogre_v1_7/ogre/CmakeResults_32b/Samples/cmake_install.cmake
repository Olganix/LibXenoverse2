# Install script for directory: C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples

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
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE" TYPE FILE FILES
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/ExampleApplication.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/ExampleFrameListener.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/ExampleLoadingBar.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/FileSystemLayer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/OgreStaticPluginLoader.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/Sample.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/SampleContext.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/SamplePlugin.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/SdkCameraMan.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/SdkSample.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/Samples/Common/include/SdkTrays.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_LOCAL_ONLY)
  # Include the install script for each subdirectory.
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/BezierPatch/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/CameraTrack/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Character/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Compositor/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/FacialAnimation/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Grass/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Instancing/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Lighting/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/OceanDemo/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/ParticleFX/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/ShaderSystem/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Shadows/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/SkeletalAnimation/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/SkyBox/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/SkyDome/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/SkyPlane/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Smoke/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/SphereMapping/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Terrain/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/TextureFX/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Transparency/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/VolumeTex/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Water/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/BSP/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/CelShading/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/DeferredShading/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/CubeMapping/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Dot3Bump/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/DynTex/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Fresnel/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Isosurf/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/ParticleGS/cmake_install.cmake")
  INCLUDE("C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/Samples/Browser/cmake_install.cmake")

ENDIF(NOT CMAKE_INSTALL_LOCAL_ONLY)

