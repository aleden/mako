#pragma once
#include "MakoMeshExporter.h"

class MakoMeshExporterClassDesc : public ClassDesc2 
{
public:
	virtual int IsPublic()
	{ return TRUE; }
	
	virtual void* Create(BOOL /*loading = FALSE*/)
	{ return new MakoMeshExporter(); }
	
	virtual const TCHAR* ClassName()
	{ return GetString(IDS_CLASS_NAME); }
	
	virtual SClass_ID SuperClassID()
	{ return SCENE_EXPORT_CLASS_ID; }
	
	virtual Class_ID ClassID()
	{ return MakoMeshExporter_CLASS_ID; }

	virtual const TCHAR* Category()
	{ return GetString(IDS_CATEGORY); }

	// returns fixed parsable name (scripter-visible name)
	virtual const TCHAR* InternalName()
	{ return _T("MakoMeshExporter"); }
	
	// returns owning module handle
	virtual HINSTANCE HInstance()
	{ return hInstance; }
};