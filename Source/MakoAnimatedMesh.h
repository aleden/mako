#pragma once

#include "MakoMesh.h"

MAKO_BEGIN_NAMESPACE

//! Interface for an animated mesh.
class AnimatedMesh : public Mesh
{
public:
	//! Gets the frame count of the animated mesh.
	//! \return Returns the amount of frames. If the amount is 1,
	//! it is a static, non animated mesh.
	virtual UInt32 GetFrameCount() const = 0;

	//! Returns the Mesh interface for a frame.
	//! \param[in] frame Frame number as zero based index. The maximum
	//! frame number is getFrameCount() - 1;
	//! \param[in] detailLevel Level of detail. 0 is the lowest, 255 the
	//! highest level of detail. Most meshes will ignore the detail level.
	//! \param[in] startFrameLoop Because some animated meshes (.MD2) are
	//! blended between 2 static frames, and maybe animated in a loop,
	//! the startFrameLoop and the endFrameLoop have to be defined, to
	//! prevent the animation to be blended between frames which are
	//! outside of this loop.
	//! If startFrameLoop and endFrameLoop are both -1, they are ignored.
	//! \param[in] endFrameLoop see startFrameLoop.
	//! \return Returns the animated mesh based on a detail level.
	virtual Mesh* GetMeshAtFrame(Int32 frame,
		                         UInt8 detailLevel = 255,
		                         Int32 startFrameLoop = -1,
								 Int32 endFrameLoop = -1) = 0;
};

MAKO_END_NAMESPACE