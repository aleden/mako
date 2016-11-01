#include "MakoReferenceCounted.h"

MAKO_BEGIN_NAMESPACE

bool ReferenceCounted::Drop() const
{
	--referenceCounter;
	
	if (referenceCounter == 0)
	{
		delete this;
		return true;
	}
	if (referenceCounter < 0)
	{
		MAKO_DEBUG_BREAK;
		delete this;
		return true;
	}
	return false;
}

MAKO_END_NAMESPACE