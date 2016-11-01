#include "MakoColorMtl.h"
#include "MakoD3D9Device.h"
#include "MakoCgDevice.h"
#include <cg/cgd3d9.h>

MAKO_BEGIN_NAMESPACE

ColorMtl::ColorMtl(GraphicsDevice* gd, const Color& c)
: c(c), CgMtl(gd->GetCgDevice()->GetMakoCgShader(MCS_COLOR)) // Fragment program name
{
	modelViewProjCgParam = (Float4x4CgParam*)GetCgShader()->GetParameterByName(ASCIIText("modelViewProj"),
		CPT_FLOAT4X4, CPC_VERTEX_FUNCT);
	modelViewProjCgParam->Hold();

	colorCgParam = (Float4CgParam*)GetCgShader()->GetParameterByName(ASCIIText("color"),
		CPT_FLOAT4, CPC_FRAGMENT_FUNC);
	colorCgParam->Hold();
}

ColorMtl::~ColorMtl()
{
	colorCgParam->Drop();
	modelViewProjCgParam->Drop();
}

Int32 ColorMtl::GetType() const
{ return MTLT_COLOR; }

void ColorMtl::Bind(GraphicsDevice* gd) const
{
	CgMtl::Bind(gd);

	GetCgShader()->BeginEditingParameters();

	colorCgParam->SetValue(static_cast<Float32>(c.GetR())/255.f,
		static_cast<Float32>(c.GetG())/255.f, static_cast<Float32>(c.GetB())/255.f,
		static_cast<Float32>(c.GetA())/255.f);
	modelViewProjCgParam->SetValue((gd->GetTransform(TS_PROJECTION) *
		                            gd->GetTransform(TS_VIEW) *
									gd->GetTransform(TS_WORLD)).GetTransposed());

	GetCgShader()->EndEditingParameters();
}

MAKO_END_NAMESPACE