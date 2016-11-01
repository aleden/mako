#pragma once
#include "Cg/cg.h"
#include "MakoCgShader.h"

MAKO_BEGIN_NAMESPACE

class GenericCgShader : public CgShader
{
private:
	CGcontext cgContext;
	CgDevice* cgdevice;
protected:
	// GenericCgShader destroys these.
	CGprogram cgVertexProgram, cgFragmentProgram;
public:
	MAKO_INLINE GenericCgShader(CgDevice* cgdevice, CGcontext cgContext)
		: cgdevice(cgdevice), cgContext(cgContext) {}
	~GenericCgShader();
	
	MAKO_INLINE void BeginEditingParameters() {}
	MAKO_INLINE void EndEditingParameters()
	{
		cgUpdateProgramParameters(cgFragmentProgram);
		cgUpdateProgramParameters(cgVertexProgram);
	}

	MAKO_INLINE CGprogram GetVertexCGProgram() const
	{ return cgVertexProgram; }

	MAKO_INLINE CGprogram GetFragmentCGProgram() const
	{ return cgFragmentProgram; }

	MAKO_INLINE CGcontext GetCGContext() const
	{ return cgContext; }
	
	MAKO_INLINE CgDevice* GetCgDevice() const
	{ return cgdevice; }
};

MAKO_END_NAMESPACE