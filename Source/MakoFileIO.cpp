#include "MakoFileIO.h"
#include "MakoMemoryStream.h"
#include "MakoException.h"
#include <cstdio>
#include <fstream>
#include <string>

using namespace std;

MAKO_BEGIN_NAMESPACE

/////////////////////////////////////////////////////////////////////////////////
// ReadTextFile
void ReadTextFile(const String& filePath,
				  String& contents)
{
	wfstream file(filePath.ToWStringData(), ios::in);
	std::wstring line;
	std::wstring all;

	while (getline(file, line, L'\n'))
		all += line;

	if (!file.eof())
		throw Exception(Text("There was an error reading the text file ") + filePath);

	contents = ToString(all.c_str());
}

/////////////////////////////////////////////////////////////////////////////////
// ReadBinaryFile
MemoryInputStream* ReadBinaryFile(const String& filePath)
{
	FILE* file;
	void* data;
	UInt32 filesize;
	
	// Open file
	file = _wfopen(filePath.ToWStringData(), L"rb");
	
	// Get file size
	fseek(file, 0, SEEK_END);
	filesize = ftell(file);
	rewind(file);
	
	// Get the data
	data = new Byte[filesize];
	fread(data, filesize, 1, file);
	
	return new MemoryInputStream(data, filesize);
}

MAKO_END_NAMESPACE
