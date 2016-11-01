#pragma once
#include "MakoCgDevice.h"
#include "MakoCgShader.h"
#include <cg\cg.h>

MAKO_BEGIN_NAMESPACE

// Forward declaration
typedef UInt16 StringChar;

//! Use this function after creating a new CGprogram to check if there
//! is a compiler error. The Cg error callback will NOT report compiler
//! errors, because it does not have a pointer to the _CGcontext, which
//! is needed to retrieve compiler errors. This function will also not
//! report non Cg compiler errors. If this function finds that there was
//! a compiler error, it will throw an exception.
//! \param[in] vertOrFrag Identifying whether you were compiling the vertex
//! or fragment program. Used for creating error message.
//! \param[in] cgContext The CGcontext, which is needed to retrieve the Cg compile
//! error (if it is present).
void CheckForCgCompileError(CG_PROGRAM_COMPONENT vertOrFrag, CGcontext cgContext);

//! This class provides functionality for CgDevice
//! that usually won't be platform-independant. When
//! writing a new GraphicsDevice and writing a new
//! CgDevice, you should inherit from this class to
//! prevent code duplication. This class provides
//! setting a Cg error callback, and the functions
//! listed below that it fullfills from the CgDevice
//! interface.
class GenericCgDevice : public CgDevice
{
private:
	CGcontext cgContext;
	GraphicsDevice* gdevice;
	CgShader* builtInShaders[MCS_ENUM_LENGTH];
protected:
	//! Classes who inherit GenericCgDevice: Call this
	//! method at the start of your deconstructor.
	void DropMakoShaders();
public:
	GenericCgDevice(GraphicsDevice* gdevice);
	virtual ~GenericCgDevice();
	
	CgShader* LoadShaderFromFile(const FilePath& fp,
		const ASCIIString& vertexProgName = defaultCgVertProgName,
		const ASCIIString& fragProgName = defaultCgFragProgName);

	CgShader* GetMakoCgShader(MAKO_CG_SHADER whichone);

	MAKO_INLINE GraphicsDevice* GetGraphicsDevice() const
	{ return gdevice; }

	MAKO_INLINE CGcontext GetCGContext()
	{ return cgContext; }

	//! Generic CgDevice provides a generic Cg error callback
	//! that throws Mako::Exceptions.
	static void CgErrorHandler(CGcontext context, CGerror error, void *data);
};

MAKO_END_NAMESPACE