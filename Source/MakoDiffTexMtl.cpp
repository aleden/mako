#include "MakoDiffTexMtl.h"
#include "MakoGraphicsDevice.h"
#include "MakoApplication.h"
#include "MakoD3D9Device.h"
#include "MakoD3D9Texture.h"
#include "MakoException.h"
#include "MakoCgDevice.h"

MAKO_BEGIN_NAMESPACE

DiffTexMtl::DiffTexMtl(Texture* tex, UInt8 tCoordChannel)
: tex(tex), texCoordChannel(tCoordChannel),
  CgMtl(tex->GetGraphicsDevice()->GetCgDevice()->GetMakoCgShader(MCS_DIFF_TEX))
{
	tex->Hold();
	modelViewProjCgParam = (Float4x4CgParam*)GetCgShader()->GetParameterByName(ASCIIText("modelViewProj"),
		CPT_FLOAT4X4, CPC_VERTEX_FUNCT);
	modelViewProjCgParam->Hold();

	difftexCgParam = (Sampler2dCgParam*)GetCgShader()->GetParameterByName(ASCIIText("diffTex"),
		CPT_SAMPLER2D, CPC_FRAGMENT_FUNC);
	difftexCgParam->Hold();
}

DiffTexMtl::~DiffTexMtl()
{
	tex->Drop();
	modelViewProjCgParam->Drop();
	difftexCgParam->Drop();
}

void DiffTexMtl::Bind(GraphicsDevice* gd) const
{
	CgMtl::Bind(gd);

	GetCgShader()->BeginEditingParameters();

	modelViewProjCgParam->SetValue((gd->GetTransform(TS_PROJECTION) *
		                            gd->GetTransform(TS_VIEW) *
									gd->GetTransform(TS_WORLD)).GetTransposed());
	difftexCgParam->SetValue(tex);

	GetCgShader()->EndEditingParameters();
}


MAKO_END_NAMESPACE