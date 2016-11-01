#pragma once
#include "MakoCommon.h"
#include "MakoString.h"

MAKO_BEGIN_NAMESPACE

// Forward declaration
class Application;

//! Specifies the importance of the log for something that
//! happened.
enum LOG_LEVEL
{ LL_LOW, LL_MEDIUM, LL_HIGH};

//! This class represents the console of a Mako application.
//! It can be opened during the running of a game usually by
//! the "~" key. It acts as displaying warning/error messages,
//! and also to execute actions that affect the Mako application's
//! state.
class Console
{
private:
	String contents;
	Application* app;
	bool logToFile;
public:
	MAKO_INLINE Console(bool logToFile = false)
		: logToFile(logToFile) {}
	MAKO_INLINE ~Console() {}
	
	//! Print text onto the contents of the Console
	//! \param[in] s The text
	MAKO_INLINE void Print(const String& s)
	{ contents += s; }

	//! Print a line of text onto the contents of the console.
	//! This simply appends a new line character to String s
	//! and then prints it to the console's contents.
	//! \param[in] s The text
	MAKO_INLINE void PrintLn(const String& s)
	{ contents += (s + StringChar('\n')); }
	
	//! Get the contents of the console.
	//! \return The contents of the console
	MAKO_INLINE const String& GetContents() const
	{ return contents; }

	//! Log that SOMETHING happened. This differs from Print()
	//! and PrintLn() because it is specialized for recording
	//! something specific that occured.
	//! \param[in] ll The log level; the importance
	//! \param[in] message The message describing what occured.
	MAKO_INLINE void Log(LOG_LEVEL ll, const String& message)
	{ PrintLn(message); }
};
MAKO_END_NAMESPACE
