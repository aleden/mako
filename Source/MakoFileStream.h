#pragma once
#include "MakoStream.h"
#include "MakoFilePath.h"
#include <cstdio>

MAKO_BEGIN_NAMESPACE

class FileInputStream : public InputStream
{
private:
	FILE* file;
	UInt32 fileSize;
public:
	MAKO_INLINE FileInputStream(const FilePath& fp) : file(nullptr)
	{
		file = _wfopen(fp.GetAbs().ToWStringData(), L"rb");
		fseek(file, 0, SEEK_END);
		fileSize = ftell(file);
		rewind(file);
	}

	MAKO_INLINE ~FileInputStream()
	{ fclose(file); }

	MAKO_INLINE void ReadTo(void* buffer, UInt32 cBytes)
	{ fread(buffer, cBytes, 1, file); }

	MAKO_INLINE UInt32 Tell() const
	{ return ftell(file); }

	MAKO_INLINE void Seek(UInt32 byte)
	{ fseek(file, byte, SEEK_SET); }

	MAKO_INLINE void Skip(UInt32 cBytes)
	{ fseek(file, cBytes, SEEK_CUR); }

	MAKO_INLINE UInt32 GetSize() const
	{ return fileSize; }
};

class FileOutputStream : public OutputStream
{
private:
	FILE* file;
	UInt32 fileSize;
public:
	MAKO_INLINE FileOutputStream(const FilePath& filePath) : file(NULL)
	{
		file = _wfopen(filePath.GetAbs().ToWStringData(), L"wb");
		fseek(file , 0 , SEEK_END);
		fileSize = ftell(file);
		rewind(file);
	}

	MAKO_INLINE ~FileOutputStream()
	{ fclose(file); }

	MAKO_INLINE void WriteData(const void* buffer, UInt32 cBytes)
	{ fwrite(buffer, cBytes, 1, file); }

	MAKO_INLINE UInt32 Tell() const
	{ return ftell(file); }

	MAKO_INLINE void Seek(UInt32 byte)
	{ fseek(file, byte, SEEK_SET); }

	MAKO_INLINE UInt32 GetSize() const
	{ return fileSize; }
};

MAKO_END_NAMESPACE