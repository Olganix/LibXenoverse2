# Install script for directory: C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain

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
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Release" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/Release/OgreMain.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Release" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/Release/OgreMain.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ee][Aa][Ss][Ee])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/RelWithDebInfo" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/RelWithDebInfo/OgreMain.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/RelWithDebInfo" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/RelWithDebInfo/OgreMain.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Rr][Ee][Ll][Ww][Ii][Tt][Hh][Dd][Ee][Bb][Ii][Nn][Ff][Oo])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/MinSizeRel" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/MinSizeRel/OgreMain.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/MinSizeRel" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/MinSizeRel/OgreMain.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Mm][Ii][Nn][Ss][Ii][Zz][Ee][Rr][Ee][Ll])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/lib/Debug" TYPE STATIC_LIBRARY OPTIONAL FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/lib/Debug/OgreMain_d.lib")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  IF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
    FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/bin/Debug" TYPE SHARED_LIBRARY FILES "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/bin/Debug/OgreMain_d.dll")
  ENDIF("${CMAKE_INSTALL_CONFIG_NAME}" MATCHES "^([Dd][Ee][Bb][Uu][Gg])$")
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE" TYPE FILE FILES
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/asm_math.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/Ogre.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAlignedAllocator.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAnimable.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAnimation.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAnimationState.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAnimationTrack.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAny.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreArchive.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreArchiveFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreArchiveManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAtomicWrappers.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAutoParamDataSource.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreAxisAlignedBox.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreBillboard.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreBillboardChain.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreBillboardParticleRenderer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreBillboardSet.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreBitwise.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreBlendMode.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreBone.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreBorderPanelOverlayElement.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/CmakeResults_32b/include/OgreBuildSettings.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCamera.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCodec.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreColourValue.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCommon.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCompositionPass.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCompositionTargetPass.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCompositionTechnique.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCompositor.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCompositorChain.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCompositorLogic.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCompositorInstance.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCompositorManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreConfig.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreConfigDialog.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreConfigFile.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreConfigOptionMap.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreController.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreControllerManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreConvexBody.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreCustomCompositionPass.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreDataStream.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreDefaultHardwareBufferManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreDistanceLodStrategy.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreDynLib.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreDynLibManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreEdgeListBuilder.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreEntity.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreErrorDialog.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreException.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreExternalTextureSource.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreExternalTextureSourceManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreFactoryObj.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreFileSystem.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreFont.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreFontManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreFrameListener.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreFrustum.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreGpuProgram.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreGpuProgramManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreGpuProgramParams.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreGpuProgramUsage.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHardwareBuffer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHardwareBufferManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHardwareIndexBuffer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHardwareOcclusionQuery.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHardwarePixelBuffer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHardwareVertexBuffer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHeaderPrefix.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHeaderSuffix.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHighLevelGpuProgram.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreHighLevelGpuProgramManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreImage.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreImageCodec.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreInstancedGeometry.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreIteratorRange.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreIteratorWrapper.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreIteratorWrappers.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreKeyFrame.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreLight.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreLodListener.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreLodStrategy.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreLodStrategyManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreLog.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreLogManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreManualObject.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMaterial.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMaterialManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMaterialSerializer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMath.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMatrix3.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMatrix4.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMemoryAllocatedObject.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMemoryAllocatorConfig.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMemoryNedAlloc.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMemoryNedPooling.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMemoryStdAlloc.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMemorySTLAllocator.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMemoryTracker.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMesh.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMeshFileFormat.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMeshManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMeshSerializer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMeshSerializerImpl.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMovableObject.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreMovablePlane.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreNode.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreNumerics.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreOptimisedUtil.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreOverlay.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreOverlayContainer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreOverlayElement.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreOverlayElementCommands.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreOverlayElementFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreOverlayManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePanelOverlayElement.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticle.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleAffector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleAffectorFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleEmitter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleEmitterCommands.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleEmitterFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleIterator.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleScriptCompiler.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleSystem.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleSystemManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreParticleSystemRenderer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePass.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePatchMesh.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePatchSurface.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePixelCountLodStrategy.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePixelFormat.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePlane.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePlaneBoundedVolume.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePlatform.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePlatformInformation.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePlugin.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePolygon.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePose.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePredefinedControllers.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePrefabFactory.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgrePrerequisites.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreProfiler.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreProgressiveMesh.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreQuaternion.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRadixSort.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRay.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRectangle.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRectangle2D.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderable.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderObjectListener.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderOperation.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderQueue.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderQueueInvocation.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderQueueListener.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderQueueSortingGrouping.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderSystem.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderSystemCapabilities.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderSystemCapabilitiesManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderSystemCapabilitiesSerializer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderTarget.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderTargetListener.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderTexture.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderToVertexBuffer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRenderWindow.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreResource.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreResourceBackgroundQueue.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreResourceGroupManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreResourceManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRibbonTrail.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRoot.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreRotationalSpline.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSceneManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSceneManagerEnumerator.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSceneNode.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSceneQuery.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreScriptCompiler.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreScriptLexer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreScriptLoader.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreScriptParser.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreScriptTranslator.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSearchOps.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSerializer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreShadowCameraSetup.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreShadowCameraSetupFocused.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreShadowCameraSetupLiSPSM.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreShadowCameraSetupPlaneOptimal.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreShadowCameraSetupPSSM.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreShadowCaster.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreShadowTextureManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreShadowVolumeExtrudeProgram.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSharedPtr.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSimpleRenderable.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSimpleSpline.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSingleton.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSkeleton.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSkeletonFileFormat.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSkeletonInstance.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSkeletonManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSkeletonSerializer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSphere.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSpotShadowFadePng.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreStableHeaders.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreStaticFaceGroup.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreStaticGeometry.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreStdHeaders.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreStreamSerialiser.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreString.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreStringConverter.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreStringInterface.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreStringVector.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSubEntity.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreSubMesh.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreTagPoint.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreTangentSpaceCalc.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreTechnique.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreTextAreaOverlayElement.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreTexture.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreTextureManager.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreTextureUnitState.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreTimer.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreUnifiedHighLevelGpuProgram.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreUserObjectBindings.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreUTFString.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreVector2.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreVector3.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreVector4.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreVertexBoneAssignment.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreVertexIndexData.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreViewport.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreWindowEventUtilities.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreWireBoundingBox.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreWorkQueue.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreFreeImageCodec.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreDDSCodec.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/OgreZip.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/WIN32" TYPE FILE FILES
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/WIN32/OgreConfigDialogImp.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/WIN32/OgreErrorDialogImp.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/WIN32/OgreTimerImp.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/WIN32/OgreMinGWSupport.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

IF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")
  FILE(INSTALL DESTINATION "${CMAKE_INSTALL_PREFIX}/include/OGRE/Threading" TYPE FILE FILES
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/Threading/OgreThreadDefines.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/Threading/OgreThreadHeaders.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/Threading/OgreDefaultWorkQueue.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/Threading/OgreThreadDefinesNone.h"
    "C:/Developpement/Dev/Ogre/GTS_v2_0_vs2008/ogre/OgreMain/include/Threading/OgreDefaultWorkQueueStandard.h"
    )
ENDIF(NOT CMAKE_INSTALL_COMPONENT OR "${CMAKE_INSTALL_COMPONENT}" STREQUAL "Unspecified")

