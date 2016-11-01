#include "MakoGenericCgShader.h"
#include "MakoException.h"

MAKO_BEGIN_NAMESPACE

GenericCgShader::~GenericCgShader()
{
	cgDestroyProgram(cgVertexProgram);
	cgDestroyProgram(cgFragmentProgram);
}

MAKO_END_NAMESPACE
