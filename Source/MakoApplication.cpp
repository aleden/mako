#include "MakoApplication.h"

MAKO_BEGIN_NAMESPACE

Application* appinst;

Application::Application()
{
	appinst = this;
}

MAKO_END_NAMESPACE