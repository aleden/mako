#include "MakoMeshExporter.h"
#include "GatherMeshInfoProc.h"

////////////////////////////////////////////////////////////////////////////////////
// Constructor(s)/Deconstructor
MakoMeshExporter::MakoMeshExporter() : exportCanceled(false), copyTextures(false) {}

MakoMeshExporter::~MakoMeshExporter() {}

////////////////////////////////////////////////////////////////////////////////////
// Methods

int	MakoMeshExporter::DoExport(const TCHAR *filePath,
							   ExpInterface *ei,
							   Interface *ip,
							   BOOL suppressPrompts,
							   DWORD options)
{
	if(!suppressPrompts)
		DialogBoxParam(hInstance,
		MAKEINTRESOURCE(IDD_PANEL),
		GetActiveWindow(),
		MakoMeshExporterOptionsDlgProc, (LPARAM)this);
 
	if (exportCanceled)
		return TRUE;

	MeshInfo meshinfo;

	//FillOutName(meshinfo, ip);
	
	ei->theScene->EnumTree(&GatherMeshInfoProc(ip, meshinfo));

	Export(meshinfo, filePath, copyTextures);

	typedef std::vector<SubMeshInfo*>::iterator SubMeshesIterator;
	for (SubMeshesIterator it = meshinfo.subMeshes.begin(); it != meshinfo.subMeshes.end(); ++it)
		delete (*it);

	// Implement the actual file Export here and
	// return TRUE If the file is exported properly
	return TRUE;
}

template <typename T>
inline void __Write(const T& data, FILE* file)
{ fwrite(&data, sizeof(T), 1, file); }

inline void __WriteData(const void* address, size_t cBytes, FILE* file)
{ fwrite(address, cBytes, 1, file); }

#define Write(data) __Write((data), file)
#define WriteData(data, cBytes) __WriteData((data), (cBytes), file)

void MakoMeshExporter::Export(MeshInfo& meshinfo, const TCHAR* filepath, bool copyTextures)
{
	FILE* file = fopen(filepath, "wb");
	
	// Write the main header
	MakoMeshHeader mmh;
	mmh.numSubMeshes = (Mako::UInt32)meshinfo.subMeshes.size();
	mmh.numStrings   = (Mako::UInt32)meshinfo.stringTable.size();
	Write(mmh);
	
	// Write the strings
	typedef std::vector<Mako::String>::iterator StringIt;
	for (StringIt it = meshinfo.stringTable.begin(); it != meshinfo.stringTable.end(); ++it)
		WriteData((*it).GetData(), ((*it).GetLength() + 1) * sizeof(Mako::StringChar));
	
	typedef std::vector<SubMeshInfo*>::iterator SubMeshIterator;
	for (SubMeshIterator it = meshinfo.subMeshes.begin(); it != meshinfo.subMeshes.end(); ++it)
	{
		// Write the sub mesh header
		MakoSubMeshHeader smh;
		smh.numVerts = (Mako::UInt32)(*it)->vertices.size();
		smh.numFaces = (Mako::UInt32)(*it)->faces.size();
		smh.numTCoordChannels = (Mako::UInt32)(*it)->numtcoords.size();
		Write(smh);

		// Write out all the number of tcoords in every channel
		Mako::UInt32 total = 0;
		for (Mako::UInt32 i = 0; i < (*it)->numtcoords.size(); ++i)
			total += (*it)->numtcoords[i];
		Write(total);

		// Write out vertices
		WriteData(&(*it)->vertices[0], (*it)->vertices.size() * sizeof(Vec3df));

		// Write out tcoords
		for (Mako::UInt32 i = 0; i < (*it)->tcoords.size(); ++i)
			WriteData(&((*it)->tcoords[i][0]), (*it)->tcoords[i].size() * sizeof(Vec2df));

		typedef std::map<Mako::UInt32, MakoSubMeshMtl*>::iterator SubMtlsIt;
		SubMtlsIt subMtlIt = (*it)->subTextures.begin();

		// Write out faces
		for (Mako::UInt32 i = 0; i < (*it)->faces.size(); ++i)
		{
			if (subMtlIt != (*it)->subTextures.end() && (*subMtlIt).first == i)
			{
				switch ((*subMtlIt).second->GetType())
				{
				case MMMT_DIFFTEXTURE:
					{
						MakoSubMeshDiffTextureMtl* diffmtl = (MakoSubMeshDiffTextureMtl*)((*subMtlIt).second);
						Mako::UInt8 entrytype = MMET_DIFFTEXTURE;
						Write(entrytype);
						Write(diffmtl->textureFilePathIndex);
						++subMtlIt;
						break;
					}
				case MMMT_COLOR:
					{
						MakoSubMeshColorMtl* colormtl = (MakoSubMeshColorMtl*)((*subMtlIt).second);
						Write(MMET_COLOR);
						Write(colormtl->c);
						++subMtlIt;
						break;
					}
				}
			}
			Mako::UInt8 entrytype = MMET_FACE;
			WriteData(&entrytype, sizeof(Mako::UInt8));

			Write((*it)->faces[i].v1);
			WriteData(&((*it)->faces[i].tcoords1[0]), sizeof(Mako::UInt32) * (*it)->faces[i].tcoords1.size());

			Write((*it)->faces[i].v2);
			WriteData(&((*it)->faces[i].tcoords2[0]), sizeof(Mako::UInt32) * (*it)->faces[i].tcoords2.size());

			Write((*it)->faces[i].v3);
			WriteData(&((*it)->faces[i].tcoords3[0]), sizeof(Mako::UInt32) * (*it)->faces[i].tcoords3.size());
		}
	}
#define ABS(a) ((a) < 0 ? -(a) : (a))
	if (copyTextures)
	{
		Mako::String& fp = Mako::ToString(filepath);
		
		Mako::Int32 fpnamePos;
		for (fpnamePos = fp.GetLength() - 1; fpnamePos >= 0 && (fp[fpnamePos] != '\\' && fp[fpnamePos] != '/'); --fpnamePos);

		Mako::String dir = fp.GetSubString(0, fpnamePos + 1);

		typedef std::vector<Mako::String>::iterator TexIt;
		for (TexIt it = meshinfo.texturesToCopy.begin(); it != meshinfo.texturesToCopy.end(); ++it)
		{
			const Mako::String& texfp = *it;
			Mako::Int32 namePos;
			for (namePos = texfp.GetLength(); namePos >= 0 && (texfp[namePos] != '\\' && texfp[namePos] != '/'); --namePos);

			Mako::String texName = texfp.GetSubString(namePos + 1 /*+1 to get rid of '\\'*/, texfp.GetLength() - namePos);

			Mako::String copypath = dir + texName;
			CopyFileW(texfp.ToWStringData(), copypath.ToWStringData(), FALSE);
		}
	}
#undef ABS

	fclose(file);
}

void MakoMeshExporter::FillOutName(MeshInfo& meshinfo, Interface* ip)
{
	TCHAR name[256];
	GetWindowText(ip->GetMAXHWnd(), name, 256);

	Mako::UInt32 strlen = 0;
	for (TCHAR* ptr = &(name[0]); ((char)*ptr) != '\0'; ++ptr)
		++strlen;
	
	Mako::Int16* utf16name = new Mako::Int16[strlen+1];
	for (Mako::UInt32 i = 0; i < strlen; ++i)
		utf16name[i] = (Mako::Int16)name[i];
	utf16name[strlen] = '\0';

	//meshinfo.name = utf16name;
}

INT_PTR CALLBACK MakoMeshExporter::MakoMeshExporterOptionsDlgProc(HWND hWnd,
																  UINT message,
																  WPARAM wParam,
																  LPARAM lParam)
{
	static MakoMeshExporter* app;

	switch(message)
	{
	case WM_INITDIALOG:
		{
			app = (MakoMeshExporter*)(lParam);
			CenterWindow(hWnd, GetParent(hWnd));
			return TRUE;
		}
	case WM_COMMAND:
		if (LOWORD(wParam) == IDC_BEXPORT)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
		}
		else if (LOWORD(wParam) == IDC_BCANCEL)
		{
			SendMessage(hWnd, WM_CLOSE, 0, 0);
			app->exportCanceled = true;
		}
		return TRUE;
	case WM_CLOSE:
		{
			LRESULT state = Button_GetCheck(GetDlgItem(hWnd, IDC_COPYTEX_CHECK));
			app->copyTextures = (state == BST_UNCHECKED ? false : true);
			EndDialog(hWnd, 0);
			return TRUE;
		}
	}
	return 0;
}

int MakoMeshExporter::ExtCount()
{
	return 1;
}

const TCHAR *MakoMeshExporter::Ext(int n)
{		
	return _T("makomesh");
}

const TCHAR *MakoMeshExporter::LongDesc()
{
	return _T("Mako Mesh");
}

const TCHAR *MakoMeshExporter::ShortDesc() 
{
	return _T("Mako Mesh");
}

const TCHAR *MakoMeshExporter::AuthorName()
{
	return _T("glass660@gmail.com");
}

const TCHAR *MakoMeshExporter::CopyrightMessage() 
{	
	//#pragma message(TODO("Return ASCII Copyright message"))
	return _T("");
}

const TCHAR *MakoMeshExporter::OtherMessage1() 
{		
	//TODO: Return Other message #1 if any
	return _T("");
}

const TCHAR *MakoMeshExporter::OtherMessage2() 
{		
	//TODO: Return other message #2 in any
	return _T("");
}

unsigned int MakoMeshExporter::Version()
{				
	return 100;
}

void MakoMeshExporter::ShowAbout(HWND hWnd)
{			
	// Optional
}

BOOL MakoMeshExporter::SupportsOptions(int ext, DWORD options)
{
	//#pragma message(TODO("Decide which options to support.  Simply return
	// true for each option supported by each Extension the exporter supports."))
	return TRUE;
}