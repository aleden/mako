#pragma once
#include "MakoCommon.h"
#include "MakoFileSystem.h"
#include "MakoOSDevice.h"
#include "MakoDirectoryPath.h"
#include "MakoFilePath.h"
#include "MakoApplication.h"

MAKO_BEGIN_NAMESPACE

FilePath FileSystem::FindFile(const String& fileName)
{
	// Check if fileName already exists
	if (os->DoesFileExist(fileName))
		return fileName;

	// Check if it is relative to the exe directory
	if (exeDir.GetFull().IsEmpty())
		exeDir = FilePath(APP()->GetCmdLnArgs()[0]).GetDir();


	String str;
	typedef LinkedList<DirectoryPath>::iterator DirsIt;
	for (DirsIt it = dirs.begin(); it != dirs.end(); ++it)
	{
		str = (*it).GetFull() + '/' + fileName;
		if (os->DoesFileExist(str))
			return str;

		str = exeDir.GetFull() + '/' + (*it).GetFull() + '/' + fileName;
		if (os->DoesFileExist(str))
			return str;
	}
	return Text("");
}

MAKO_END_NAMESPACE