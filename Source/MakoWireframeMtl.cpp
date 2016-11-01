#include "MakoWireframeMtl.h"
#include "MakoGraphicsDevice.h"

MAKO_BEGIN_NAMESPACE

void WireframeMtl::Bind(GraphicsDevice* gd) const
{
	gd->SetOption(GDO_WIREFRAME, true);
}

void WireframeMtl::UnBind(GraphicsDevice* gd) const
{
	gd->SetOption(GDO_WIREFRAME, false);
}

MAKO_END_NAMESPACE