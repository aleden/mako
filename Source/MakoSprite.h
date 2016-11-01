#pragma once

#include "MakoCommon.h"
#include "MakoScene2dNode.h"

MAKO_BEGIN_NAMESPACE

class Texture;

class Sprite : public Scene2dNode
{
private:
	Texture* tex;
	Float32 rot;
public:
	MAKO_API Sprite(Texture* t, const Position2d& pos,
		const Rotation2d& rot = 0.f);

	MAKO_INLINE Texture* GetTexture() const
	{ return tex; }

	MAKO_API virtual ~Sprite();
	MAKO_API virtual void Draw();
};

MAKO_END_NAMESPACE