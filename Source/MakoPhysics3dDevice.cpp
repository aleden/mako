#include "MakoPhysics3dDevice.h"

MAKO_BEGIN_NAMESPACE

////////////////////////////////////////////////////////////////////////////////////////////////////
// Physics3dActor
Physics3dActor::Physics3dActor(Physics3dScene* scene,
								 const Position3d& pos,
								 const Rotation3d& rot)
								 : pos(pos), rot(rot), scene(scene)
{}

Physics3dActor::~Physics3dActor()
{}

Physics3dScene* Physics3dActor::GetPhysics3dScene() const
{ return scene; }

const Position3d& Physics3dActor::GetPosition() const
{ return pos; }

const Rotation3d& Physics3dActor::GetRotation() const
{ return rot; }

void Physics3dActor::SetRotation(const Rotation3d& rot)
{ this->rot = rot; }

void Physics3dActor::SetPosition(const Position3d& pos)
{ this->pos = pos; }

MAKO_END_NAMESPACE