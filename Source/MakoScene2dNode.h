#pragma once

#include "MakoCommon.h"
#include "MakoVec2d.h"
#include "MakoReferenceCounted.h"
#include "MakoLinkedList.h"

MAKO_BEGIN_NAMESPACE

class Application;

class Scene2dNode : public ReferenceCounted
{
private:
	Position2d pos;
	Rotation2d rot;
	Scene2dNode* parent;
	LinkedList<Scene2dNode*> children;
public:
	MAKO_API Scene2dNode
		(
			const Position2d& pos = Pos2d(0), 
			const Rotation2d& rot = Rot2d(0), 
			Scene2dNode* parent = nullptr, 
			bool isRootSceneNode = false
		);
	MAKO_API virtual ~Scene2dNode();

	virtual void Draw() {}
	virtual void Update() {}

	MAKO_API UInt16 GetNumberOfChildren() const;
	MAKO_API Scene2dNode* GetChild(UInt16 index) const;
	MAKO_API void AddChild(Scene2dNode* node);
	MAKO_API void RemoveChild(UInt16 index);
	MAKO_API void RemoveChild(Scene2dNode* n);

	MAKO_API Scene2dNode* GetParent() const;

	MAKO_API void SetPosition(const Position2d& pos);
	MAKO_API void SetRotation(const Rotation2d& rot);
	
	MAKO_API const Position2d& GetPosition() const;
	MAKO_API const Rotation2d& GetRotation() const;
};

MAKO_END_NAMESPACE
