#include "MakoLightmappedDiffTexMtl.h"
#include "MakoGraphicsDevice.h"
#include "MakoApplication.h"
#include "MakoD3D9Device.h"
#include "MakoD3D9Texture.h"
#include "MakoException.h"
#include "MakoCgDevice.h"

MAKO_BEGIN_NAMESPACE

LightmappedDiffTexMtl::LightmappedDiffTexMtl(Texture* difftex, Texture* lmtex)
: difftex(difftex), lmtex(lmtex), 
  CgMtl(difftex->GetGraphicsDevice()->GetCgDevice()->GetMakoCgShader(MCS_LIGHTMAPPED_DIFF_TEX))
{
	difftex->Hold();
	lmtex->Hold();
	
	modelViewProjCgParam = (Float4x4CgParam*)GetCgShader()->GetParameterByName(ASCIIText("modelViewProj"),
		CPT_FLOAT4X4, CPC_VERTEX_FUNCT);
	modelViewProjCgParam->Hold();

	difftexCgParam = (Sampler2dCgParam*)GetCgShader()->GetParameterByName(ASCIIText("diffTex"),
		CPT_SAMPLER2D, CPC_FRAGMENT_FUNC);
	difftexCgParam->Hold();

	lmtexCgParam = (Sampler2dCgParam*)GetCgShader()->GetParameterByName(ASCIIText("lmTex"),
		CPT_SAMPLER2D, CPC_FRAGMENT_FUNC);
	lmtexCgParam->Hold();
}

LightmappedDiffTexMtl::~LightmappedDiffTexMtl()
{
	difftex->Drop();
	lmtex->Drop();
	modelViewProjCgParam->Drop();
	difftexCgParam->Drop();
	lmtexCgParam->Drop();
}

void LightmappedDiffTexMtl::Bind(GraphicsDevice* gd) const
{
	CgMtl::Bind(gd);

	GetCgShader()->BeginEditingParameters();

	modelViewProjCgParam->SetValue((gd->GetTransform(TS_PROJECTION) *
		                            gd->GetTransform(TS_VIEW) *
									gd->GetTransform(TS_WORLD)).GetTransposed());
	difftexCgParam->SetValue(difftex);
	lmtexCgParam->SetValue(lmtex);

	GetCgShader()->EndEditingParameters();
}


MAKO_END_NAMESPACE