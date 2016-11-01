#pragma once
#include "MakoCommon.h"
#include "MakoCgShader.h"
#include "MakoFileStream.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class GraphicsDevice;

enum MAKO_CG_SHADER
{
	MCS_COLOR,
	MCS_DIFF_TEX,
	MCS_LIGHTMAPPED_DIFF_TEX,
	MCS_ENUM_LENGTH
};

//! Interface allowing the use of Nvidia's shader language Cg
class CgDevice
{
public:
	//! Load a Cg program from a text file.
	//! \param[in] fp The file path of the text file which contains the
	//! Cg program's source code. The text file must be ascii encoded.
	//! \param[in] vertProgName The name of the vertex program function
	//! \param[in] fragProgName The name of the fragment program function
	//! \return The newly loaded Cg program.
	virtual CgShader* LoadShaderFromFile(const FilePath& fp,
		const ASCIIString& vertexProgName = defaultCgVertProgName,
		const ASCIIString& fragProgName = defaultCgFragProgName) = 0;

	//! Create a CgShader from it's raw source code.
	//! \param[in] sourceCode The source code of the Cg program
	//! \param[in] vertProgName The name of the vertex program function
	//! \param[in] fragProgName The name of the fragment program function
	//! \return The newly created program.
	virtual CgShader* CreateShader(const ASCIIString& sourceCode,
		const ASCIIString& vertexProgName = defaultCgVertProgName,
		const ASCIIString& fragProgName = defaultCgFragProgName) = 0;

	//! Get a built-in Cg shader that works in the core of the Mako library.
	//! This will use lazy-evaluation to not compile Cg shaders that are never
	//! used. It will also work to prevent the duplication of compiling the same
	//! shader more than once.
	//! \param[in] whichone Identifying which of the built-in Cg shaders you want
	//! \return The Cg shader requested
	virtual CgShader* GetMakoCgShader(MAKO_CG_SHADER whichone) = 0;

	//! Get the GraphicsDevice
	//! \return The GraphicsDevice
	virtual GraphicsDevice* GetGraphicsDevice() const = 0;

	virtual ~CgDevice() {}
};

MAKO_END_NAMESPACE