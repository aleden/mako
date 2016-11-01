#if 0
#include "MakoObjMeshLoader.h"
#include "MakoSimpleMesh.h"
#include "MakoFileIO.h"
#include "MakoArrayList.h"
#include "MakoVec3d.h"
#include "MakoVec3d.h"
#include "MakoString.h"
#include "MakoStandardVertex.h"
#include "Makomath.h"
#include "MakoApplication.h"
#include "MakoGraphicsDevice.h"

MAKO_BEGIN_NAMESPACE
static const UInt32 WORD_BUFFER_LENGTH = 512;

//! Constructor
ObjMeshLoader::ObjMeshLoader(Application* app)
{ this->app = app; }

ObjMeshLoader::~ObjMeshLoader()
{}

Mesh* ObjMeshLoader::Load(InputStream* stream)
{
	Int8* contents = static_cast<Int8*>(new Byte[stream->GetSize()]);
	Int8* const copyOfContents = contents;
	stream->ReadTo(contents, stream->GetSize());

	const UInt32 fileSize = stream->GetSize();
	//FileIO::ReadBinaryFile(filePath, contents, &fileSize);
	const Int8* const bufEnd = contents + fileSize;

	if (!fileSize)
		return nullptr;

	const UInt32 WORD_BUFFER_LENGTH = 512;

	ArrayList<StandardVertex> verts;
	//ArrayList<UInt16> indices;
	
	ArrayList<Vec3df> positions;
	ArrayList<Vec2df> texcoords;

	UInt32 cvt = 0, cv = 0, cvn = 0, cf = 0;
	// Process obj information
	const Int8* bufPtr = contents;
	while(bufPtr != bufEnd)
	{
		switch(*bufPtr)
		{
			// mtllib (material)
		case 'm':
			{
			}
			break;
		case 'v':
			{
				// v, vn, vt
				switch(*(bufPtr+1))
				{
				case ' ':
					{
						// Vertex
						positions.push_back(ReadVec3df(bufPtr, bufEnd));
						++cv;
					}
					break;
				case 'n':
					{
						// Normal
						++cvn;
					}
					break;

				case 't':
					{
						// Texcoord
						texcoords.push_back(ReadVec2df(bufPtr, bufEnd));
						++cvt;
					}
					break;
				}
				break;
			}
		case 'g':
			{
				// Group
			}
			break;

		case 's':
			{
				// Smoothing group
			}
			break;

		case 'u':
			{
				// Usemtl
			}
			break;
		case 'f':
			{
				/*
				v  -5.0000 0.0000 5.0000
				# 8 vertices

				vt 5.5000 -4.5000 -4.5000
				# 8 texture coords

				f 1/1 2/2 3/3 
				# 12 faces
				*/

				// Face
				// vertex/texture/normal
				UInt32 posi,  texci, posi2, texci2, posi3, texci3;
				
				const Int8* p;
				std::string str;
				for (p = bufPtr+2; *p != '/' && !IsCharSpace(*p); ++p)
					str += *p;

				posi =  CStringDataToS32(str.c_str());

				str.clear();
				for (p += 1; *p != '/' && !IsCharSpace(*p); ++p)
					str += *p;

				texci =  CStringDataToS32(str.c_str());

				str.clear();
				for (p += 1; *p != '/' && !IsCharSpace(*p); ++p)
					str += *p;

				posi2 =  CStringDataToS32(str.c_str());

				str.clear();
				for (p += 1; *p != '/' && !IsCharSpace(*p); ++p)
					str += *p;

				texci2 =  CStringDataToS32(str.c_str());

				str.clear();
				for (p += 1; *p != '/' && !IsCharSpace(*p); ++p)
					str += *p;

				posi3 =  CStringDataToS32(str.c_str());

				str.clear();
				for (p += 1; *p != '/' && !IsCharSpace(*p); ++p)
					str += *p;

				texci3 =  CStringDataToS32(str.c_str());

				verts.push_back(StandardVertex(positions[posi-1], texcoords[texci-1]));
				verts.push_back(StandardVertex(positions[posi2-1], texcoords[texci2-1]));
				verts.push_back(StandardVertex(positions[posi3-1], texcoords[texci3-1]));

				//indices.push_back(indices.size() + 2);
				//indices.push_back(indices.size() + 0);
				//indices.push_back(indices.size() - 2));
			}
			break;
		case '#':
			{
				// Comment
			}
		default:
			break;
		}
		// End switch, eat up rest of line
		GoNextLine(bufPtr, bufEnd);
	}

	SimpleMesh* mesh = new SimpleMesh;

	MeshDataCreationParams p;
	p.numPrimitives = verts.size()/3;
	p.numVertices   = verts.size();
	p.primitiveType = PT_TRIANGLELIST;
	p.vertexType    = VT_STANDARD;
	p.vertices      = static_cast<void*>(&verts[0]);
	
	mesh->AddSubMesh(app->GetGraphicsDevice()->CreateMeshData(p));

	delete [] copyOfContents;

	return mesh;
}

//! skip current word and stop at beginning of next one
void ObjMeshLoader::GoNextWord(const Int8*& buf, const Int8* const bufEnd, bool acrossNewlines)
{
	// skip current word
	while(( buf != bufEnd ) && !IsCharSpace(*buf))
		++buf;

	return GoFirstWord(buf, bufEnd, acrossNewlines);
}

// copies the current word from the inBuf to the outBuf
UInt32 ObjMeshLoader::CopyWord(Int8* outBuf, const Int8* const inBuf, UInt32 outBufLength, const Int8* const bufEnd)
{
	if (!outBufLength)
		return 0;
	if (!inBuf)
	{
		*outBuf = 0;
		return 0;
	}

	UInt32 i = 0;
	while(inBuf[i])
	{
		if (IsCharSpace(inBuf[i]) || &(inBuf[i]) == bufEnd)
			break;
		++i;
	}

	UInt32 length = std::min(i, outBufLength-1);
	for (UInt j=0; j<length; ++j)
		outBuf[j] = inBuf[j];

	outBuf[i] = 0;
	return length;
}

void ObjMeshLoader::GoAndCopyNextWord(Int8* outBuf, const Int8*& inBuf, UInt32 outBufLength, const Int8* const bufEnd)
{
	GoNextWord(inBuf, bufEnd, false);
	CopyWord(outBuf, inBuf, outBufLength, bufEnd);
}

// Read 3d vector of floats
Vec3df ObjMeshLoader::ReadVec3df(const Int8*& bufPtr, const Int8* const bufEnd)
{
	Vec3df vec;
	const UInt32 WORD_BUFFER_LENGTH = 256;
	Int8 wordBuffer[WORD_BUFFER_LENGTH];

	GoAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	// change handedness
	vec.x = FastCStringDataToS32(wordBuffer);
	GoAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.z = FastCStringDataToS32(wordBuffer);
	GoAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.y = FastCStringDataToS32(wordBuffer);

	return vec;
}

// Read 2d vector of floats
Vec2df ObjMeshLoader::ReadVec2df(const Int8*& bufPtr, const Int8* const bufEnd)
{
	const UInt32 WORD_BUFFER_LENGTH = 256;
	Int8 wordBuffer[WORD_BUFFER_LENGTH];

	Vec2df vec;

	GoAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.x = FastCStringDataToS32(wordBuffer);
	GoAndCopyNextWord(wordBuffer, bufPtr, WORD_BUFFER_LENGTH, bufEnd);
	vec.y =  - FastCStringDataToS32(wordBuffer); // change handedness

	return vec;
}

// Read until line break is reached and stop at the next non-space character
void ObjMeshLoader::GoNextLine(const Int8*& buf, const Int8* const bufEnd)
{
	// look for newline characters
	while(buf != bufEnd)
	{
		// found it, so leave
		if (*buf=='\n' || *buf=='\r')
			break;
		++buf;
	}
	return GoFirstWord(buf, bufEnd);
}

// Skip space characters and stop on first non-space
void ObjMeshLoader::GoFirstWord(const Int8*& buf, const Int8* const bufEnd, bool acrossNewlines)
{
	// Skip space characters
	if (acrossNewlines)
		while((buf != bufEnd) && IsCharSpace(*buf))
			++buf;
	else
		while((buf != bufEnd) && IsCharSpace(*buf) && (*buf != '\n'))
			++buf;
}

MAKO_END_NAMESPACE
#endif