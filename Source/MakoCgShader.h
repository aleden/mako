#pragma once
#include "MakoCommon.h"
#include "MakoCgParameters.h"

MAKO_BEGIN_NAMESPACE

enum CG_PROGRAM_COMPONENT
{ CPC_VERTEX_FUNCT, CPC_FRAGMENT_FUNC, CPC_ENUM_LENGTH };

//! This represents a Cg program
class CgShader : public ReferenceCounted
{
public:
	//! This will set the CgShader to render with the graphics device
	virtual void Bind() = 0;

	//! This will set no CgShader to render with the graphics device
	virtual void UnBind() = 0;

	virtual void BeginEditingParameters() = 0;
	virtual void EndEditingParameters() = 0;

	virtual CgParameter* GetParameterByName(const ASCIIString& name,
		CG_PARAMETER_TYPE type, CG_PROGRAM_COMPONENT func) = 0;

	//! This will get the CgDevice it was created with
	virtual CgDevice* GetCgDevice() const = 0;
};

MAKO_END_NAMESPACE