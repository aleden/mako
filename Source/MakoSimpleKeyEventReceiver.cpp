#include "MakoSimpleKeyEventReceiver.h"
#include "MakoApplication.h"

MAKO_BEGIN_NAMESPACE

SimpleKeyEventReceiver::SimpleKeyEventReceiver(Application* app)
{
	this->app = app;


	for (UInt8 i = 0; i < 26; i++)
		keys[KEY_A+i] = false;
	for (UInt8 i = 0; i < 12; i++)
		keys[KEY_F1+i] = false;
	for (UInt8 i = 0; i < 10; i++)
		keys[KEY_0+i] = false;

	keys[KEY_ESCAPE] = false;
}

MAKO_END_NAMESPACE