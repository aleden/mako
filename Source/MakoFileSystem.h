#pragma once
#include "MakoCommon.h"
#include "MakoString.h"
#include "MakoFilePath.h"
#include "MakoDirectoryPath.h"
#include "MakoLinkedList.h"
#include "MakoApplication.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class OSDevice;

class FileSystem
{
	LinkedList<DirectoryPath> dirs;
	OSDevice* os;

	DirectoryPath exeDir;
public:
	MAKO_INLINE FileSystem() : os(APP()->OS()) {}
	
	MAKO_INLINE ~FileSystem() {}

	void AddDirectory(const DirectoryPath& dir)
	{ dirs.push_back(dir); }

	FilePath FindFile(const String& fileName);
};

MAKO_END_NAMESPACE