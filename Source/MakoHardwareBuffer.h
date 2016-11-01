#pragma once
#include "MakoCommon.h"

MAKO_BEGIN_NAMESPACE

//! A HardwareBuffer represents data contained in hardware.
//! A HardwareBuffer will always have a parent that contains
//! the data in system memory that it represents in hardware
//! memory. Although this duplication of data in two places
//! wastes memory, it allows for higher performance because
//! the HardwareBuffer can be created with write-only access.
//!
//! Therefore, to change the contents of this buffer you must
//! change the contents of it's parent, whatever it's parent
//! may be. For example, If you want to change the vertices'
//! positions in a MeshData, you must call the appropriate 
//! methods (such as GetVertices()) and modify that data. Then,
//! it is essential to call Update() on the HardwareBuffer to
//! have any real changes.
class HardwareBuffer
{
public:
	//! Reloads hardware buffer with data from parent
	virtual void Update() = 0;
};

MAKO_END_NAMESPACE