#include "GatherMeshInfoProc.h"
#include "stdmat.h"
#include <fstream>
#include <algorithm>
#include <IPathConfigMgr.h>

#if !defined(_HYBRID) && !defined(_DEBUG)
__forceinline void NULLFUNC(...) {}
#define DebugPrint NULLFUNC
#endif

// In the Materials Editor a user can choose which of the channels a texture map 
// is applied to. In the Coordinates rollout of the user interface, in the Texture 
// Mapping dropdown the user may choose "Explicit Map Channel". In this case the 
// Map Channel spinner is enabled and the user may choose a number between 1 and 99. 
// This number is effectively an index into the Mesh class mapping methods. 
//
// The number 1 corresponds to the pre-R3 Mesh class TVerts array. The numbers 2 through 
// 99 correspond to the R3 and later channels. If the user chooses "Vertex Color 
// Channel" from the dropdown then the Map Channel spinner is disabled and the vertex 
// color channel is used. This corresponds to an index of 0 in the Mesh class mapping 
// methods.

void HandleStdMat(SubMeshInfo* smi, MeshInfo& meshinfo, StdMat* stdm, Mako::UInt32 face)
{
	// Access the Diffuse map and see if it's a Bitmap texture
	Texmap* tmap = stdm->GetSubTexmap(ID_DI);
	if (tmap && tmap->ClassID() == Class_ID(BMTEX_CLASS_ID, 0))
	{
		// It is -- get the filepath of the texture and add it to the string table.
		BitmapTex* bmt = (BitmapTex*)tmap;
		TCHAR* origPath = bmt->GetMapName();
		size_t origPathLen = _tcslen(origPath);
		TCHAR* fp = NULL;
		
		// Fill fileAndExt
		TCHAR fileAndExt[256];

		size_t namePos = _tcslen(origPath);
		for (TCHAR* p = namePos + origPath; *p != '\\' && *p != '/'; --p, --namePos);
		++namePos;

		for (size_t i = namePos; i < origPathLen; ++i)
			fileAndExt[i-namePos] = origPath[i], fileAndExt[i-namePos+1] = '\0';
		// Fill DONE

		IPathConfigMgr* pathMgr = IPathConfigMgr::GetPathConfigMgr();

		TCHAR testPath[1024];
		for (unsigned int i = 0; i < pathMgr->GetMapDirCount(false); ++i)
		{
			const TCHAR* mapDir = pathMgr->GetMapDir(i, false);

			_tcscpy(testPath, mapDir);
			_tcscat(testPath, "\\");

			_tcscat(testPath, fileAndExt);

			if (DoesFileExist(testPath))
			{
				fp = testPath;
				break;
			}
		}

		if (fp)
		{
			// bmt->GetMapName() gets the file path of the bitmap
			// We have to do 3 things:
			// 1. Append file path in the string table
			// 2. Add the name of the file in the full path of the texture into texturesToCopy
			// 3. Add a new entry to subTextures at oldnumfaces with a new instance of
			// MakoSubMeshDiffTextureMtl

			const Mako::String texfp = Mako::ToString(fp);
			Mako::Int32 namePos;
			for (namePos = texfp.GetLength(); namePos >= 0 && (texfp[namePos] != '\\' && texfp[namePos] != '/'); --namePos);
			Mako::String texName = texfp.GetSubString(namePos + 1, texfp.GetLength() - namePos - 1);

			Mako::UInt32 pos = 0xFFFFFFFF;
			
			for (Mako::UInt32 i = 0; i < meshinfo.stringTable.size(); ++i)
			{
				if (meshinfo.stringTable[i] == texName)
					pos = i;
			}

			if (pos != 0xFFFFFFFF)
			{
				smi->subTextures[face] = new MakoSubMeshDiffTextureMtl(pos);
			}
			else
			{
				smi->subTextures[face] = new MakoSubMeshDiffTextureMtl(static_cast<Mako::UInt32>(meshinfo.stringTable.size()));
				meshinfo.stringTable.push_back(texName);
				meshinfo.texturesToCopy.push_back(texfp);
			}
		}
	}
}

void GatherMeshInfoProc::BuildOnMeshInfo(Interface* ip, INode* node, MeshInfo& meshinfo)
{
	int deleteIt;
	TriObject* to = GetTriObjectFromNode(node, ip, deleteIt);
	
	// Is there a triangle mesh available for the node?
	if (!to)
		return ;
	Mesh& mesh = to->GetMesh();	
	meshinfo.stringTable.push_back(Mako::ToString(node->GetName()));

	// The number of tcoord channels is the deciding factor on whether to make
	// a new SubMeshInfo or not.
	Mako::UInt32 numtcoordchannels = mesh.getNumMaps() - 1;

	// This points to the SubMeshInfo we will build on.
	SubMeshInfo* smi = NULL;
	
	// Check if there is already a SubMeshInfo existing that we can build upon.
	// If so, set smi to it so we can deal with it later.
	typedef std::vector<SubMeshInfo*>::iterator SubMeshInfoIt;
	for (SubMeshInfoIt it = meshinfo.subMeshes.begin(); it != meshinfo.subMeshes.end(); ++it)
	{
		if ((*it)->numtcoords.size() == numtcoordchannels)
		{
			smi = *it;
			break;
		}
	}

	// We need to make a new SubMeshInfo to accomodate the number of tcoord channels there are,
	// and set smi to this new SubMeshInfo.
	if (!smi)
	{
		smi = new SubMeshInfo;
		meshinfo.subMeshes.push_back(smi);
		smi->numtcoords.resize(numtcoordchannels);
		smi->tcoords.resize(numtcoordchannels);
		
		// Set all the num tcoords to 0
		memset(&smi->numtcoords[0], 0, sizeof(Mako::UInt32) * smi->numtcoords.size());
	}

	// These will be kept for later to offset stuff; used when we are combining two existing
	// sub mesh infos
	Mako::UInt32 oldnumverts = static_cast<Mako::UInt32>(smi->vertices.size());
	std::vector<Mako::UInt32> oldnumtcoords = smi->numtcoords;
	Mako::UInt32 oldnumfaces = static_cast<Mako::UInt32>(smi->faces.size());
	
	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Material stuff here
	// the 3ds Max Standard material has a Class_ID of DMTL_CLASS_ID while the Multi/Sub-Object 
	// material uses MULTI_CLASS_ID
	Mtl* mtl = node->GetMtl();
	if (mtl && mtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) // Is it a StdMat?
	{
		StdMat* stdm = static_cast<StdMat*>(mtl);
		HandleStdMat(smi, meshinfo, stdm, oldnumfaces);

	}
	else if (mtl && mtl->ClassID() == Class_ID(MULTI_CLASS_ID, 0))
	{
		//For every face:
		//	Get mtl id
		//	Is this mtl id diff from the last face's?
		//		Mod mtl id down by the number of (num sub mtls - 1)
		//		Get the material that is that sub mtl
		//		Check if it's a StdMtl (or anyone that we handle)
		//			Add a new material entry in the sub mtls, at face
		MultiMtl* msubobjmtl = static_cast<MultiMtl*>(mtl);
		for (Mako::UInt32 iFace = 0; iFace < mesh.getNumFaces(); ++iFace)
		{
			MtlID mtlId = mesh.faces[iFace].getMatID();
			if (iFace == 0 || mesh.faces[iFace - 1].getMatID() != mtlId)
			{
				MtlID newId = mtlId % (msubobjmtl->NumSubMtls() - 1);
				Mtl* submtl = msubobjmtl->GetSubMtl(mtlId);
				if (submtl && submtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0)) // Is it a StdMat?
				{
					StdMat* stdm = static_cast<StdMat*>(submtl);
					HandleStdMat(smi, meshinfo, stdm, oldnumfaces + iFace);
				}
			}
		}
	}
	
	// Add the number of texcoords there are in every tex coord channel. We start
	// tcoordchannel at 1 because 0 are the vertex colors. We just want the tcoords.
	for (Mako::UInt32 tcoordchannel = 1; tcoordchannel < mesh.getNumMaps(); ++tcoordchannel)
	{
		smi->numtcoords[tcoordchannel-1] += mesh.getNumMapVerts(tcoordchannel);
		smi->tcoords[tcoordchannel-1].resize(smi->tcoords[tcoordchannel-1].size() + mesh.getNumMapVerts(tcoordchannel));
	}

	// Get the tcoords
	for (Mako::UInt32 tcoordchannel = 1; tcoordchannel < mesh.getNumMaps(); ++tcoordchannel)
	{
		for (Mako::UInt32 i = 0; i < mesh.getNumMapVerts(tcoordchannel); ++i)
		{
			// 1 - on v component to change handedness.
			Point2 uv(mesh.mapVerts(tcoordchannel)[i].x, 1 - mesh.mapVerts(tcoordchannel)[i].y);
			smi->tcoords[tcoordchannel-1][oldnumtcoords[tcoordchannel-1] + i] = uv;
		}
	}

	// Get the modeling transformation of node
	Matrix3 tm = node->GetObjTMAfterWSM(0);

	// Resize it to prevent array reallocations in the background (this is for
	// optimization).
	smi->vertices.resize(smi->vertices.size() + mesh.getNumVerts());

	// Get the vertices
	for (Mako::UInt32 i = 0; i < mesh.getNumVerts(); ++i)
	{
		// Multiply the modeling transformation by the vertex position in object
		// space to get the vertex position in world space (which is what we want,
		// since the modeling transformation is not exported).
		Point3 vpos = tm * mesh.getVert(i);
		
		// Swap the z and y components to change the handedness from 3ds max's (right handed) to
		// left handed
		Mako::Float32 oldz = vpos.z;
		vpos.z = vpos.y;
		vpos.y = oldz;
		
		smi->vertices[oldnumverts + i] = vpos;
	}

	// Resize it to prevent array reallocations in the background (this is for
	// optimization).
	smi->faces.resize(smi->faces.size() + mesh.getNumFaces());
	
	// Add the faces.
	for (Mako::UInt32 face = 0; face < mesh.getNumFaces(); ++face)
	{
		MakoFace mf;
		mf.v1 = mesh.faces[face].getVert(0) + oldnumverts;
		mf.v2 = mesh.faces[face].getVert(1) + oldnumverts;
		mf.v3 = mesh.faces[face].getVert(2) + oldnumverts;
		
		mf.tcoords1.resize(mesh.getNumMaps() - 1);
		for (Mako::UInt32 map = 1; map < mesh.getNumMaps(); ++map)
		{
			if (map != 1)
			{
				Mako::UInt32 texcoordsbefore = 0;
				for (Mako::Int32 i = map - 2; i >= 0; --i)
					texcoordsbefore += smi->numtcoords[i];
				mf.tcoords1[map-1] = mesh.mapFaces(map)[face].getTVert(0) + oldnumtcoords[map-1] + texcoordsbefore;
			}
			else
				mf.tcoords1[map-1] = mesh.mapFaces(map)[face].getTVert(0) + oldnumtcoords[map-1];
		}

		mf.tcoords2.resize(mesh.getNumMaps() - 1);
		for (Mako::UInt32 map = 1; map < mesh.getNumMaps(); ++map)
		{
			if (map != 1)
			{
				Mako::UInt32 texcoordsbefore = 0;
				for (Mako::Int32 i = map - 2; i >= 0; --i)
					texcoordsbefore += smi->numtcoords[i];
				mf.tcoords2[map-1] = mesh.mapFaces(map)[face].getTVert(1) + oldnumtcoords[map-1] + texcoordsbefore;
			}
			else
				mf.tcoords2[map-1] = mesh.mapFaces(map)[face].getTVert(1) + oldnumtcoords[map-1];
		}

		mf.tcoords3.resize(mesh.getNumMaps() - 1);
		for (Mako::UInt32 map = 1; map < mesh.getNumMaps(); ++map)
		{
			if (map != 1)
			{
				Mako::UInt32 texcoordsbefore = 0;
				for (Mako::Int32 i = map - 2; i >= 0; --i)
					texcoordsbefore += smi->numtcoords[i];
				mf.tcoords3[map-1] = mesh.mapFaces(map)[face].getTVert(2) + oldnumtcoords[map-1] + texcoordsbefore;
			}
			else
				mf.tcoords3[map-1] = mesh.mapFaces(map)[face].getTVert(2) + oldnumtcoords[map-1];
		}

		smi->faces[face + oldnumfaces] = mf;
	}
	
	// Delete triangle mesh if needed when done
	if (deleteIt)
		to->DeleteMe();
}

int GatherMeshInfoProc::callback(INode *node)
{
	BuildOnMeshInfo(ip, node, meshinfo);
	return TREE_CONTINUE;
}

TriObject* GetTriObjectFromNode(INode *node, Interface *ip, int &deleteIt)
{
	deleteIt = FALSE;
	Object *obj = node->EvalWorldState(ip->GetTime()).obj;

	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject *tri = (TriObject *) obj->ConvertToType(ip->GetTime(),
			Class_ID(TRIOBJ_CLASS_ID, 0));
		// Note that the TriObject should only be deleted
		// if the pointer to it is not equal to the object
		// pointer that called ConvertToType()
		if (obj != tri) deleteIt = TRUE;
		return tri;
	}
	else
	{
		return NULL;
	}
}