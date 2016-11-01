#include "MakoUtilities.h"

MAKO_BEGIN_NAMESPACE


UInt32 CalcVBIndexPosFromPrimCount(UInt32 numPrimitives, PRIMITIVE_TYPE type)
{
	if (numPrimitives == 0)
		return 0;

	switch (type)
	{
	case PT_POINTLIST:
		return numPrimitives;
	case PT_LINELIST:
		return numPrimitives * 2;
	case PT_LINESTRIP:
		return numPrimitives + 1;
	case PT_TRIANGLELIST:
		return numPrimitives * 3;
	case PT_TRIANGLESTRIP:
		return 2 + numPrimitives;
	case PT_TRIANGLEFAN:
		return numPrimitives + 1;
	default:
		return 0xFFFFFFFF;
	}
}

UInt32 CalcVertPosFromPrimCount(UInt32 numPrimitives, PRIMITIVE_TYPE type)
{
	if (numPrimitives == 0)
		return 0;
	switch (type)
	{
	case PT_POINTLIST:
		return numPrimitives;
	case PT_LINELIST:
		return numPrimitives*2;
	case PT_LINESTRIP:
		return numPrimitives + 1;
	case PT_TRIANGLELIST:
		return numPrimitives * 3;
	case PT_TRIANGLESTRIP:
		return 2 + numPrimitives;
	case PT_TRIANGLEFAN:
		return numPrimitives + 1;
	default:
		return 0xFFFFFFFF;
	}
}



MAKO_END_NAMESPACE