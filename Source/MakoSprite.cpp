#include "MakoSprite.h"
#include "MakoApplication.h"
#include "MakoGraphicsDevice.h"
#include "MakoTexture.h"

MAKO_BEGIN_NAMESPACE

Sprite::Sprite(Texture* t,
			   const Position2d& pos,
			   const Rotation2d& rot)
: Scene2dNode(pos, rot)
{ tex = t; tex->Hold(); }

Sprite::~Sprite()
{ tex->Drop(); }

void Sprite::Draw()
{ APP()->GetGraphicsDevice()->Draw2dTexture(GetPosition(), tex, GetRotation()); }

MAKO_END_NAMESPACE
