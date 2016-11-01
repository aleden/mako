#pragma once
#include "Max.h"
#include "resource.h"
#include "istdplug.h"
#include "iparamb2.h"
#include "iparamm2.h"
#include <string>
#include <sstream>
#include <stdio.h>
#include "../../Source/MakoString.h"

struct MeshInfo;
#define MakoMeshExporter_CLASS_ID Class_ID(0x6f9a1426, 0xa33ee85f)

class MakoMeshExporter : public SceneExport
{
	bool exportCanceled;
	bool copyTextures;
	static HWND hParams;

	static void Export(MeshInfo& meshinfo, const TCHAR* filepath, bool copyTextures);
	void FillOutName(MeshInfo& meshinfo, Interface* ip);
public:
	//Constructor/Destructor
	MakoMeshExporter();
	~MakoMeshExporter();

	// Number of extensions supported
	int	ExtCount();

	// Extension #n (i.e. "3DS")
	const TCHAR* Ext(int n);

	// Long ASCII description (i.e. "Autodesk 3D Studio File")
	const TCHAR* LongDesc();

	// Short ASCII description (i.e. "3D Studio")
	const TCHAR* ShortDesc();

	// ASCII Author name
	const TCHAR* AuthorName();

	// ASCII Copyright message
	const TCHAR* CopyrightMessage();

	// Other message #1
	const TCHAR* OtherMessage1();

	// Other message #2
	const TCHAR* OtherMessage2();

	// Version number * 100 (i.e. v3.01 = 301)
	unsigned int Version();

	// Show DLL's "About..." box
	void ShowAbout(HWND hWnd);

	BOOL SupportsOptions(int ext, DWORD options);
	int DoExport(const TCHAR *name, ExpInterface *ei,
		Interface *i,
		BOOL suppressPrompts = FALSE,
		DWORD options = 0);

	static INT_PTR CALLBACK MakoMeshExporterOptionsDlgProc(HWND hWnd,
		UINT message, WPARAM wParam, LPARAM lParam);
};

extern TCHAR *GetString(int id);
extern HINSTANCE hInstance;