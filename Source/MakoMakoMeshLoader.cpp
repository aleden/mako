#include "MakoMakoMeshLoader.h"
#include "MakoSimpleMesh.h"
#include "MakoFileIO.h"
#include "MakoArrayList.h"
#include "MakoVec3d.h"
#include "MakoVec3d.h"
#include "MakoString.h"
#include "MakoStandardVertex.h"
#include "MakoT2Vertex.h"
#include "Makomath.h"
#include "MakoApplication.h"
#include "MakoGraphicsDevice.h"
#include "MakoConsole.h"
#include "MakoDiffTexMtl.h"
#include "MakoOSDevice.h"
#include "MakoStream.h"
#include "MakoFileSystem.h"
#include "makomeshtypes.h"

MAKO_BEGIN_NAMESPACE

Mesh* MakoMeshLoader::Load(InputStream* stream)
{
	SimpleMesh* mesh = new SimpleMesh;

	MakoMeshHeader mh;
	stream->Read(mh);
	
	ArrayList<String> strtable(mh.numStrings);
	ArrayList<Texture*> texturesLoaded(mh.numStrings);
	memset(&texturesLoaded[0], 0, sizeof(Texture*) * texturesLoaded.size());

	for (UInt i = 0; i < mh.numStrings; ++i)
		stream->Read(strtable[i]);

	for (UInt iSubMesh = 0; iSubMesh < mh.numSubMeshes; ++iSubMesh)
	{
		MakoSubMeshHeader smh;
		stream->Read(smh);

		IndexedMeshDataCreationParams p;

		ArrayList<Pos3d> vertPositions(smh.numVerts);
		ArrayList<UInt16>   indices(smh.numFaces*3);

		if (indices.size() % 3 != 0)
		{
			MAKO_DEBUG_BREAK;
			throw Exception(Text("There was an error in the Mako Mesh loader."));
		}

		// Load the number of texcoords in every channel
		UInt32 totalNumTCoordVerts;
		stream->Read(totalNumTCoordVerts);

		ArrayList<Vec2df> tcoords(totalNumTCoordVerts);

		// Load vertspos
		stream->ReadTo(&vertPositions[0], sizeof(Pos3d) * vertPositions.size());

		// Load texcoords
		stream->ReadTo(&tcoords[0], sizeof(TCoord) * tcoords.size());

		ArrayList<StandardVertex> sverts;
		ArrayList<T2Vertex> t2verts;

		UInt32 indicesCount = 0;
		// Parse faces, generate indices/sverts from them.
		for (UInt iFace = 0; iFace < smh.numFaces; ++iFace)
		{
			UInt8 entrytype;
			stream->Read(entrytype);
			switch (entrytype)
			{
			case MMET_DIFFTEXTURE:
				{
					FilePath fp;
					UInt32 texfpIndex = stream->Read32BitUInt();
					if (texturesLoaded[texfpIndex])
					{
						p.materials[indicesCount/3] = new DiffTexMtl(texturesLoaded[texfpIndex]);
					}
					else if (!(fp = APP()->FS()->FindFile(strtable[texfpIndex])).GetAbs().IsEmpty())
					{
						texturesLoaded[texfpIndex] = APP()->GD()->LoadTextureFromFile(fp);
						p.materials[indicesCount/3] = new DiffTexMtl(texturesLoaded[texfpIndex]);
					}
					else
					{
						APP()->GetConsole()->Log(LL_MEDIUM, Text("Failed to load texture (") + strtable[texfpIndex] + Text(") in MakoMeshLoader::Load()"));
					}

					--iFace; // doesn't count as a face.
					break;
				}
			case MMET_FACE:
				{
					UInt32 posi, posi2, posi3;
					ArrayList<UInt32> texci(smh.numTCoordChannels), texci2(smh.numTCoordChannels),
					               texci3(smh.numTCoordChannels);

					stream->Read(posi);
					for (UInt channel = 0; channel < smh.numTCoordChannels; ++channel)
						stream->Read(texci[channel]);

					stream->Read(posi2);
					for (UInt channel = 0; channel < smh.numTCoordChannels; ++channel)
						stream->Read(texci2[channel]);

					stream->Read(posi3);
					for (UInt channel = 0; channel < smh.numTCoordChannels; ++channel)
						stream->Read(texci3[channel]);

					switch (smh.numTCoordChannels)
					{
					case 0:
						sverts.push_back(StandardVertex(vertPositions[posi],  0.f));
						sverts.push_back(StandardVertex(vertPositions[posi2], 0.f));
						sverts.push_back(StandardVertex(vertPositions[posi3], 0.f));
						break;
					case 1:
						sverts.push_back(StandardVertex(vertPositions[posi],  tcoords[texci[0]]));
						sverts.push_back(StandardVertex(vertPositions[posi2], tcoords[texci2[0]]));
						sverts.push_back(StandardVertex(vertPositions[posi3], tcoords[texci3[0]]));
						break;
					case 2:
						t2verts.push_back(T2Vertex(vertPositions[posi],  tcoords[texci[0]],  tcoords[texci[1]]));
						t2verts.push_back(T2Vertex(vertPositions[posi2], tcoords[texci2[0]], tcoords[texci2[1]]));
						t2verts.push_back(T2Vertex(vertPositions[posi3], tcoords[texci3[0]], tcoords[texci3[1]]));
						break;
					}

					indices[indicesCount] = indicesCount + 2;
					++indicesCount;
					indices[indicesCount] = indicesCount + 0;
					++indicesCount;
					indices[indicesCount] = indicesCount - 2;
					++indicesCount;
					break;
				}
			}
		}

		p.numPrimitives = indices.size()/3;
		p.numVertBufferIndices = indices.size();
		if (smh.numTCoordChannels == 1 || smh.numTCoordChannels == 0)
		{
			p.numVertices = sverts.size();
			p.vertices = static_cast<void*>(&sverts[0]);
			p.vertexType = VT_STANDARD;
		}
		else if (smh.numTCoordChannels == 2)
		{
			p.numVertices = t2verts.size();
			p.vertices = static_cast<void*>(&t2verts[0]);
			p.vertexType = VT_T2;
		}
		p.primitiveType = PT_TRIANGLELIST;
		p.vertBufferIndexType  = VBIT_16;
		p.vertBufferIndices = static_cast<void*>(&indices[0]);
		if (p.materials.size() == 0)
			p.materials[0] = APP()->GD()->GetDefaultMaterial();

		mesh->AddSubMesh(APP()->GetGraphicsDevice()->CreateIndexedMeshData(p));
	}

	return mesh;
}

MAKO_END_NAMESPACE