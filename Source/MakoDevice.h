#pragma once
#include "MakoString.h"
#include "MakoCommon.h"

MAKO_BEGIN_NAMESPACE

class Application;

//! Devices are code that provide some large functionality; so
//! much that a device could be a whole seperate external library.
//! This abstract class just describes a small amount of general
//! behavior that all devices must fulfill.
class Device
{
public:
	//! Get the name of the device. For instance, if you wrote
	//! a direct 3d 8 device that inherited GraphicsDevice, you would
	//! return "Direct 3d 8"
	//! \return The name of the device
	virtual String GetName() const = 0;
};

MAKO_END_NAMESPACE
