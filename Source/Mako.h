#pragma once

/*--======================================================================--*\
|                        - Mako Game Engine-                                 |
|                         anthoneden0@gmail.com                              |
|~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~|
|        VERSION : .1                       PLATFORMS: WinXP                 |
|        LISCENCE: N/A                      DIALECT  : C++                   |
|>--------------------------------------------------------------------------<|
| DESC: 2d/3d C++ game engine                                                |
\*~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~*/

#include "MakoCompileConfig.h"

#ifdef MAKO_COMPILING
	#undef MAKO_COMPILING
#endif

#include "MakoCommon.h"
#include "MakoNetworkingDevice.h"
#include "MakoVersion.h"
#include "MakoRUN_MAKO_APPLICATION.h"
#include "MakoAnimatedMesh.h"
#include "MakoApplication.h"
#include "MakoLightmappedDiffTexMtl.h"
#include "MakoArrayList.h"
#include "MakoAudioDevice.h"
#include "MakoBitManipulator.h"
#include "MakoCamera.h"
#include "MakoColor.h"
#include "MakoConsole.h"
#include "MakoColorMtl.h"
#include <MakoCgDevice.h>
#include <MakoCgMtl.h>
#include <MakoCgShader.h>
#include "MakoDevice.h"
#include "MakoDynamicBox.h"
#include "MakoDynamicSphere.h"
#include "MakoEntity3d.h"
#include "Makoevents.h"
#include "MakoException.h"
#include "MakoT2Vertex.h"
#include "MakoFileIO.h"
#include "MakoFileStream.h"
#include "MakoFPSCamera.h"
#include "MakoFont.h"
#include "MakoGameStateApplication.h"
#include "MakoMaterial.h"
#include "MakoGraphicsDevice.h"
#include "MakoIndexedMeshData.h"
#include "MakoLinkedList.h"
#include "MakoMakoMeshLoader.h"
#include "MakoMap.h"
#include "MakoMaterial.h"
#include "MakoMath.h"
#include "MakoMatrix4.h"
#include "MakoMemoryStream.h"
#include "MakoMesh.h"
#include "MakoMeshData.h"
#include "MakoMeshLoader.h"
#include "MakoMeshManipulator.h"
#include "MakoMeshSceneNode.h"
#include "MakoObjMeshLoader.h"
#include "MakoOSDevice.h"
#include "MakoPhysics3dDevice.h"
#include "Makoplatform.h"
#include "MakoReferenceCounted.h"
#include "MakoScene2d.h"
#include "MakoScene2dNode.h"
#include "MakoScene3d.h"
#include "MakoScene3dNode.h"
#include "MakoSimpleApplication.h"
#include "MakoSimpleKeyEventReceiver.h"
#include "MakoSimpleMesh.h"
#include "MakoDiffTexMtl.h"
#include "MakoSkybox.h"
#include "MakoSprite.h"
#include "MakoStandardVertex.h"
#include "MakoStaticBox.h"
#include "MakoStaticGeometry.h"
#include "MakoStaticPlane.h"
#include "MakoStaticSphere.h"
#include "MakoStream.h"
#include "MakoString.h"
#include "MakoString.h"
#include "MakoTexture.h"
#include "MakoUtilities.h"
#include "MakoVec2d.h"
#include "MakoVec3d.h"
#include "MakoVertex.h"
#include "MakoColorMtl.h"
#include "MakoWireframeMtl.h"

//#if MAKO_PLATFORM == MAKO_PLATFORM_WIN32
//extern "C"
//{
//extern "C" __declspec(dllimport) Application* Mako::appinst;
//}
//#endif