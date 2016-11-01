#pragma once

#ifndef MAKO_NO_HANDLE_EXC

#define RUN_MAKO_APPLICATION(MAKO_CLASS_NAME) \
int main(int argc, char** argv)\
{\
	MAKO_CLASS_NAME* app = nullptr;\
	try\
	{\
		app = new MAKO_CLASS_NAME;\
	}\
	catch (Mako::Exception& e)\
	{\
		if (app->GetOSDevice())\
		{\
			app->GetOSDevice()->SetMouseLocked(false);\
			app->GetOSDevice()->ShowMessageBox(Text("Mako Exception"), Text("Description: ") + e.description + Text("\nError Code: ") + String::From32BitInt(e.errorCode));\
		}\
		else\
		{\
			wprintf(L"Mako Exception\nDescription: %s\nError Code: %d", e.description.ToWStringData(), e.errorCode);\
		}\
		app->Quit();\
		goto deletion;\
	}\
	catch (...)\
	{\
		if (app->GetOSDevice())\
		{\
			app->GetOSDevice()->SetMouseLocked(false);\
			app->GetOSDevice()->ShowMessageBox(Text("Unknown Exception"), Text("An unknown exception occured in MAKO_CLASS_NAME::MAKO_CLASS_NAME()."));\
		}\
		else\
		{\
			wprintf(L"Unknown exception in MAKO_CLASS_NAME::MAKO_CLASS_NAME()\n");\
		}\
		return -1;\
	}\
	\
	try\
	{\
		app->SetCommandLineArguments(argc, argv);\
		app->Initialize();\
	}\
	catch (Mako::Exception& e)\
	{\
		if (app->GetOSDevice())\
		{\
			app->GetOSDevice()->SetMouseLocked(false);\
			app->GetOSDevice()->ShowMessageBox(Text("Mako Exception"), Text("Description: ") + e.description + Text("\nError Code: ") + String::From32BitInt(e.errorCode));\
		}\
		else\
		{\
			wprintf(L"Mako Exception\nDescription: %s\nError Code: %d", e.description.ToWStringData(), e.errorCode);\
		}\
		app->Quit();\
		goto deletion;\
	}\
	catch (...)\
	{\
		if (app->GetOSDevice())\
		{\
			app->GetOSDevice()->SetMouseLocked(false);\
			app->GetOSDevice()->ShowMessageBox(Text("Unknown Exception"), Text("An unknown exception occured in MAKO_CLASS_NAME::Initialize()."));\
		}\
		else\
		{\
			wprintf(L"Unknown exception in MAKO_CLASS_NAME::Initialize()\n");\
		}\
		app->Quit();\
		goto deletion;\
	}\
	\
	if (app->HasQuitted())\
		goto deletion;\
	\
	if (app->GetOSDevice())\
		app->GetOSDevice()->ResetElapsedTime();\
	\
	try\
	{\
		app->Run();\
	}\
	catch (Mako::Exception& e)\
	{\
		if (app->GetOSDevice())\
		{\
			app->GetOSDevice()->SetMouseLocked(false);\
			app->GetOSDevice()->ShowMessageBox(Text("Mako Exception"), Text("Description: ") + e.description + Text("\nError Code: ") + String::From32BitInt(e.errorCode));\
		}\
		app->Quit();\
		goto deletion;\
	}\
	catch (...)\
	{\
		if (app->GetOSDevice())\
		{\
			app->GetOSDevice()->SetMouseLocked(false);\
			app->GetOSDevice()->ShowMessageBox(Text("Mako Exception"), Text("An unknown exception occured in MAKO_CLASS_NAME::Run()."));\
		}\
		else\
		{\
			wprintf(L"Unknown exception in MAKO_CLASS_NAME::Run()\n");\
		}\
		app->Quit();\
		goto deletion;\
	}\
	\
deletion:\
	try\
	{\
		app->Shutdown();\
		delete app;\
	}\
	catch (Mako::Exception& e)\
	{\
		return e.errorCode;\
	}\
	catch (...)\
	{\
		return -1;\
	}\
	return 0;\
}

#else
#define RUN_MAKO_APPLICATION(MAKO_CLASS_NAME) \
int main(int argc, char** argv)\
{\
	MAKO_CLASS_NAME* app = new MAKO_CLASS_NAME;\
	app->SetCommandLineArguments(argc, argv);\
	app->Initialize();\
	\
	if (app->HasQuitted())\
		goto deletion;\
	\
	if (app->GetOSDevice())\
		app->GetOSDevice()->ResetElapsedTime();\
	\
	app->Run();\
	\
	\
deletion:\
	app->Shutdown();\
	delete app;\
	return 0;\
}

#endif