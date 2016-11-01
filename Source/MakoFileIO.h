#pragma once
#include "MakoCommon.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

class MemoryInputStream;

//! Read a text file.
//! \param[in] filePath The file path of the text file to read.
//! \param[out] contents The entire text file is transferred to contents.
void ReadTextFile(const String& filePath, String& contents);

//! Reads a binary file
//! \param[in] filePath The file path of the file to read in.
//! \return The MemoryInputStream containing the contents of the file
//! at filePath
MemoryInputStream* ReadBinaryFile(const String& filePath);

MAKO_END_NAMESPACE