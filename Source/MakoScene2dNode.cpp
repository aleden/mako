#include "MakoScene2dNode.h"
#include "MakoVec2d.h"
#include "MakoApplication.h"
#include "MakoScene2d.h"
#include "MakoMaterial.h"

MAKO_BEGIN_NAMESPACE

/////////////////////////////////////////////////////////////////
// Constructor(s)/Deconstructor
Scene2dNode::Scene2dNode(const Position2d& pos,
						 const Rotation2d& rot,
						 Scene2dNode* parent,
						 bool isRootSceneNode)
{
	this->pos = pos;
	this->rot = rot;
	
	if (!isRootSceneNode)
	{
		if (parent)
			this->parent = parent;
		else
			this->parent = APP()->GetActive2dScene()->GetRootNode();
		
		this->parent->AddChild(this);
	}
	else
	{
		this->parent = nullptr;
	}
}

Scene2dNode::~Scene2dNode()
{
	for (LinkedList<Scene2dNode*>::iterator it = children.begin();
		it != children.end();
		++it)
	{
		(*it)->Drop();
	}
}

/////////////////////////////////////////////////////////////////
// Methods

UInt16           Scene2dNode::GetNumberOfChildren() const
{ return children.size(); }

Scene2dNode* Scene2dNode::GetChild(UInt16 index) const
{
	UInt16 c = 0;
	LinkedList<Scene2dNode*>::const_iterator it = children.begin();
	while (it != children.end())
	{
		if (c == index)
		{
			return *it;
		}
		++it; ++c;
	}

	return nullptr;
}

void          Scene2dNode::AddChild(Scene2dNode* node)
{
	node->Hold();
	children.push_back(node);
}

void          Scene2dNode::RemoveChild(UInt16 index)
{
	UInt16 c = 0;
	LinkedList<Scene2dNode*>::const_iterator it = children.begin();
	while (it != children.end())
	{
		if (c == index)
		{
			Scene2dNode* temp = *it;
			children.erase(it);
			temp->Drop();
			return ;
		}
		++it; ++c;
	}
}

void          Scene2dNode::RemoveChild(Scene2dNode* n)
{
	for (LinkedList<Scene2dNode*>::iterator it = children.begin();
		it != children.end();
		++it)
	{
		if (*it == n)
		{
			Scene2dNode* temp = *it;
			children.erase(it);
			temp->Drop();
			return;
		}
	}
}

Scene2dNode* Scene2dNode::GetParent() const
{ return parent; }

void          Scene2dNode::SetPosition(const Position2d& pos)
{ this->pos = pos; }

void          Scene2dNode::SetRotation(const Rotation2d& rot)
{ this->rot = rot; }

const Position2d& Scene2dNode::GetPosition() const
{ return pos; }

const Rotation2d& Scene2dNode::GetRotation() const
{ return rot; }

MAKO_END_NAMESPACE

