#include "MakoGenericCgDevice.h"
#include "MakoGraphicsDevice.h"
#include "MakoException.h"
#include "MakoApplication.h"
#include "MakoFileSystem.h"

MAKO_BEGIN_NAMESPACE

GenericCgDevice::GenericCgDevice(GraphicsDevice* gdevice) : gdevice(gdevice)
{
	cgContext = cgCreateContext();
	cgSetParameterSettingMode(cgContext, CG_DEFERRED_PARAMETER_SETTING);

	cgSetErrorHandler(CgErrorHandler, nullptr);

	memset(builtInShaders, 0, sizeof(CgShader*) * MCS_ENUM_LENGTH);
}

GenericCgDevice::~GenericCgDevice()
{ cgDestroyContext(cgContext); }

void GenericCgDevice::DropMakoShaders()
{
	for (UInt i = 0; i < MCS_ENUM_LENGTH; ++i)
	{
		if (builtInShaders[i] != nullptr && builtInShaders[i]->Drop() == true)
			builtInShaders[i] = nullptr;
	}
}
CgShader* GenericCgDevice::GetMakoCgShader(MAKO_CG_SHADER whichone)
{
	if (builtInShaders[whichone] == nullptr)
	{
		switch (whichone)
		{
		case MCS_COLOR:
			{
				builtInShaders[whichone] = 
					CreateShader
					(
					ASCIIText("                                  \n\
void ColorMtlv(float4 position : POSITION,                       \n\
               out float4 oPosition : POSITION,                  \n\
			   uniform float4x4 modelViewProj)                   \n\
{                                                                \n\
	/* Transform position from object space to clip space */     \n\
	oPosition = mul(modelViewProj, position);                    \n\
}                                                                \n\
                                                                 \n\
void ColorMtlf(out float4 oColor : COLOR,                        \n\
               uniform float4 color)                             \n\
{                                                                \n\
	oColor = color;                                              \n\
}                                                                \n"),
					ASCIIText("ColorMtlv"),
					ASCIIText("ColorMtlf")
					);
				builtInShaders[whichone]->Hold();
				return builtInShaders[whichone];
			}
		case MCS_DIFF_TEX:
			{
				builtInShaders[whichone] = 
					CreateShader
					(
					ASCIIText("                                            \n\
void DiffTexMtlv(float4 position : POSITION,                                           \n\
                 float2 tcoord0 : TEXCOORD0,                                           \n\
                                                                                       \n\
				 out float4 oPosition : POSITION,                                      \n\
                 out float2 oTcoord0 : TEXCOORD0,                                      \n\
                                                                                       \n\
                 uniform float4x4 modelViewProj)                                       \n\
{                                                                                      \n\
	/* Transform position from object space to clip space */                           \n\
	oPosition = mul(modelViewProj, position);                                          \n\
	oTcoord0 = tcoord0;                                                                \n\
}                                                                                      \n\
                                                                                       \n\
void DiffTexMtlf(float2 tcoord0 : TEXCOORD0,                                           \n\
                                                                                       \n\
                 out float4 oColor : COLOR,                                            \n\
                                                                                       \n\
				 uniform sampler2D diffTex)                                            \n\
                                                                                       \n\
{                                                                                      \n\
	oColor = tex2D(diffTex, tcoord0);                                                  \n\
}                                                                                      \n"),
					ASCIIText("DiffTexMtlv"),
					ASCIIText("DiffTexMtlf")
					);
				builtInShaders[whichone]->Hold();
				return builtInShaders[whichone];
			}
		case MCS_LIGHTMAPPED_DIFF_TEX:
			{
				builtInShaders[whichone] = 
					CreateShader
					(
					ASCIIText("                                            \n\
void LMDiffTexMtlv(float4 position : POSITION,                                         \n\
                   float2 tcoord0 : TEXCOORD0,                                         \n\
                   float2 tcoord1 : TEXCOORD1,                                         \n\
                                                                                       \n\
				   out float4 oPosition : POSITION,                                    \n\
                   out float2 oTcoord0 : TEXCOORD0,                                    \n\
                   out float2 oTcoord1 : TEXCOORD1,                                    \n\
                                                                                       \n\
                   uniform float4x4 modelViewProj)                                     \n\
{                                                                                      \n\
	/* Transform position from object space to clip space */                           \n\
	oPosition = mul(modelViewProj, position);                                          \n\
	oTcoord0 = tcoord0;                                                                \n\
	oTcoord1 = tcoord1;                                                                \n\
}                                                                                      \n\
                                                                                       \n\
void LMDiffTexMtlf(float2 tcoord0 : TEXCOORD0,                                         \n\
                   float2 tcoord1 : TEXCOORD1,                                         \n\
                                                                                       \n\
                   out float4 oColor : COLOR,                                          \n\
                                                                                       \n\
				   uniform sampler2D diffTex,                                          \n\
				   uniform sampler2D lmTex)                                            \n\
                                                                                       \n\
{                                                                                      \n\
	float4 diffColor = tex2D(diffTex, tcoord0);                                        \n\
	float4 lmColor = tex2D(lmTex, tcoord1);                                            \n\
	oColor = diffColor * (lmColor * 2.0);                                              \n\
}                                                                                      \n"),
					ASCIIText("LMDiffTexMtlv"),
					ASCIIText("LMDiffTexMtlf")
					);
				builtInShaders[whichone]->Hold();
				return builtInShaders[whichone];
			}
		default:
			{
				throw Exception(Text("The parameter of type MAKO_CG_SHADER given to \
									 GenericCgDevice::GetMakoCgShader() was invalid."));
			}
		}
	}
	else
	{
		return builtInShaders[whichone];
	}
}

CgShader* GenericCgDevice::LoadShaderFromFile(const FilePath& fileName,
											  const ASCIIString& vertexProgName,
											  const ASCIIString& fragProgName)
{
	FilePath found = APP()->FS()->FindFile(fileName);
	if (found.GetAbs().IsEmpty())
		throw Exception(Text("The Cg shader [") + fileName.GetAbs() + Text("] does not exist"));

	FileInputStream file(found);
	char* contents = new char[file.GetSize() + 1];
	file.ReadTo(contents, file.GetSize());
	contents[file.GetSize()] = '\0';
	return CreateShader(ASCIIString::FromAllocData(contents), vertexProgName, fragProgName);
}

void GenericCgDevice::CgErrorHandler(CGcontext context, CGerror error, void *data)
{
	//MAKO_DEBUG_BREAK;
	//// cgGetError returns the last error condition that has occured.
	//// The error condition is reset after cgGetError is called. 
	//CGerror error = cgGetError();
	//if (error != CG_NO_ERROR && error != CG_COMPILER_ERROR)
	//{
	//	MAKO_DEBUG_BREAK;
	//	throw Exception(Text("Cg runtime error:") + ToString(cgGetErrorString(error)));
	//}
	printf("error=%d", error);
	if (error == CG_COMPILER_ERROR)
	{
		throw Exception(Text("Cg compilation error(s):\n") + ToString(cgGetLastListing(context)));
	}
	else
	{
		throw Exception(Text("Cg runtime error:") + ToString(cgGetErrorString(error)));
	}
}

void CheckForCgCompileError(CG_PROGRAM_COMPONENT vertOrFrag, CGcontext cgContext)
{
	CGerror error = cgGetError();
	if (error != CG_NO_ERROR && error == CG_COMPILER_ERROR)
	{
		MAKO_DEBUG_BREAK;
		throw Exception(Text("Cg compilation error(s) while compiling ") +
		                (vertOrFrag == CPC_VERTEX_FUNCT ? Text("vertex program") :
						                                  Text("fragment program")) + 
						Text(":\n") + ToString(cgGetLastListing(cgContext)));
	}
}

MAKO_END_NAMESPACE