#pragma once
#include "MakoCommon.h"
#include "MakoVec3d.h"
#include "MakoReferenceCounted.h"
#include "MakoLinkedList.h"
#include "MakoMatrix4.h"

MAKO_BEGIN_NAMESPACE

// Forward declarations
class Material;
class GraphicsDevice;
class Application;
class Scene3d;
class Physics3dScene;

struct SingleMaterialCreationParams
{
	MAKO_INLINE SingleMaterialCreationParams() : material(nullptr) {}

	Material* material;
};

struct Physics3dSceneCreationParams
{
	MAKO_INLINE Physics3dSceneCreationParams() : p3ds(nullptr) {}

	Physics3dScene* p3ds;
};

struct Scene3dNodeCreationParams
{
	MAKO_INLINE Scene3dNodeCreationParams()
		: isDynamic(true), scale(1.f), pos(0.f), rot(0.f) {}

	Position3d pos;
	Rotation3d rot;
	Scale3d    scale;
	bool       isDynamic;
};

//! A scene node is a node in the hierarchical scene graph. Every scene
//! node may have children, which are also scene nodes. Children move
//! relative to their parent's position. If the parent of a node is not
//! visible, its children won't be visible either. In this way, it is for
//! example easily possible to attach a light to a moving car, or to place
//! a walking character on a moving platform on a moving ship.
//! Almost any method in a node is called on it's children as well.
class Scene3dNode : public ReferenceCounted
{
	friend class Scene3d;

	//! The scene that this node belongs to. This can be a nullptr.
	//! However, if this is not a nullptr, and parent is a nullptr, 
	//! this node is the root.
	Scene3d* scene;
	
	//! The parent of the Scene3d node. Can be a nullptr.
	//! However, if this is a nullptr, and scene is not a nullptr, 
	//! this node is the root.
	Scene3dNode* parent;
	
	//! Controls whether the node can be scaled/moved/rotated
	bool isDynamic;

	//! The children of this node
	LinkedList<Scene3dNode*> children;
	
	//! Absolute transformation of the node.
	Matrix4f absTransformation;

	//! Relative translation of the scene node.
	Position3d relPos;

	//! Relative rotation of the scene node.
	Rotation3d relRot;

	//! Relative scale of the scene node.
	Scale3d relScale;
public:
	Scene3dNode(const Position3d& pos = Pos3d(0.f),
				const Rotation3d& rot = Rot3d(0.f),
				const Scale3d& scale  = Scale3d(1.f),
				bool isDynamic = true);
	virtual ~Scene3dNode();

	//! The scene node draws stuff in this function. It is
	//! Can be implemented in sub classes of Scene3dnode
	virtual void Draw(GraphicsDevice* gd) {}

	//! The scene node updates in this function. It is
	//! Can be implemented in sub classes of Scene3dnode
	virtual void Update() {}

	//! The scene node can do stuff that relies on other
	//! scene nodes already updated.
	//! Can be implemented in sub classes of Scene3dnode
	virtual void PostUpdate() {}

	virtual void PreDraw() {}
	virtual void PostDraw() {}

	//! Updates the absolute position based on the relative and the parents position,
	//! recursively calls UpdateAbsoluteTransformation() on children.
	MAKO_API void UpdateAbsoluteTransformation();

	//! Gets the children of this node
	//! \return The children
	MAKO_INLINE const LinkedList<Scene3dNode*>& GetChildren() const
	{ return children; }

	MAKO_INLINE bool IsDynamic() const
	{ return isDynamic; }

	//! Adds a child
	//! \param[in] node The node to add as a child
	MAKO_INLINE void AddChild(Scene3dNode* node)
	{
		node->Hold();
		node->parent = this;
		node->scene  = scene;
		children.push_back(node);
	}

	//! Removes a child
	//! \param[in] node The node to remove from the list of
	//! this node's children
	void RemoveChild(Scene3dNode* node);

	//! Get the parent of this node
	//! \return The parent
	MAKO_INLINE Scene3dNode* GetParent() const
	{ return parent; }

	//! Set the relative position of this node
	//! \param[in] pos The new relative position
	MAKO_INLINE virtual void SetPosition(const Position3d& pos)
	{ if (isDynamic) relPos = pos; }

	//! Set the relative position of this node
	//! \param[in] x The new relative x position
	//! \param[in] y The new relative y position
	//! \param[in] z The new relative z position
	MAKO_INLINE virtual void SetPosition(Float32 x, Float32 y, Float32 z)
	{ if (isDynamic) SetPosition(Pos3d(x, y, z)); }

	//! Add to the current position
	//! \param[in] vec The vector to add to the position
	MAKO_INLINE void Move(const Vec3df& vec)
	{ if (isDynamic) relPos += vec; }

	//! Add to the current position
	//! \param[in] x The x pos to add to the pos
	//! \param[in] y The y pos to add to the pos
	//! \param[in] z The z pos to add to the pos
	MAKO_INLINE void Move(Float32 x = 0.f, Float32 y = 0.f, Float32 z = 0.f)
	{
		if (isDynamic)
		{
			relPos.x += x;
			relPos.y += y;
			relPos.z += z;
		}
	}
	
	//! Set the relative rotation of this node
	//! \param[in] rot The new relative rotation
	MAKO_INLINE virtual void SetRotation(const Rotation3d& rot)
	{ if (isDynamic) relRot = rot; }

	//! Set the relative rotation of this node
	//! \param[in] x The new relative x rotation
	//! \param[in] y The new relative y rotation
	//! \param[in] z The new relative z rotation
	MAKO_INLINE virtual void SetRotation(Float32 x, Float32 y, Float32 z)
	{ if (isDynamic) SetRotation(Rot3d(x, y, z)); }

	//! Add to the current rotation
	//! \param[in] vec The vector to add to the rotation
	MAKO_INLINE void Rotate(const Vec3df& vec)
	{ if (isDynamic) relRot += vec; }

	//! Add to the current rotation
	//! \param[in] x The x rot to add to the rot
	//! \param[in] y The y rot to add to the rot
	//! \param[in] z The z rot to add to the rot
	MAKO_INLINE void Rotate(Float32 x = 0.f, Float32 y = 0.f, Float32 z = 0.f)
	{
		if (isDynamic)
		{
			relRot.x += x;
			relRot.y += y;
			relRot.z += z;
		}
	}

	//! Set the relative scale of this node
	//! \param[in] scale The new relative scale
	MAKO_INLINE virtual void SetScale(const Size3d& scale)
	{ if (isDynamic) relScale = scale; }
	
	//! Set the relative scale of this node
	//! \param[in] x The new relative x scale
	//! \param[in] y The new relative y scale
	//! \param[in] z The new relative z scale
	MAKO_INLINE virtual void SetScale(Float32 x, Float32 y, Float32 z)
	{ if (isDynamic) SetScale(Size3d(x, y, z)); }

	//! Add to the current scale
	//! \param[in] vec The vector to add to the scale
	MAKO_INLINE void Scale(const Vec3df& vec)
	{ if (isDynamic) relScale += vec; }

	//! Add to the current scale
	//! \param[in] x The x scale to add to the scale
	//! \param[in] y The y scale to add to the scale
	//! \param[in] z The z scale to add to the scale
	MAKO_INLINE void Scale(Float32 x = 0.f, Float32 y = 0.f, Float32 z = 0.f)
	{
		if (isDynamic)
		{
			relScale.x += x;
			relScale.y += y;
			relScale.z += z;
		}
	}

	
	//! Get the relative position of this node
	//! \return The relative position
	MAKO_INLINE const Position3d& GetPosition() const
	{ return relPos; }
	
	//! Get the relative rotation of this node
	//! \return The relative rotation
	MAKO_INLINE const Rotation3d& GetRotation() const
	{ return relRot; }

	//! Get the relative scale of this node
	//! \return The new rotation
	MAKO_INLINE const Scale3d& GetScale() const
	{ return relScale; }
	
	//! Gets the relative transformation of the scene node.
	//! \return the relative transformation of the scene node.
	//! The relative transformation is stored internally as 3
	//! vectors: translation, rotation and scale. To get the relative
	//! transformation matrix, it is calculated from these values.
	MAKO_API Matrix4f GetTransformation() const;

	//! Gets the abolute position of the node. This will not be
	//! correct if any of it's parents' orientations have been
	//! modified. If this is the case, call UpdateAbsolutePosition()
	//! to get the correct absolute position.
	//! \return The current absolute position of the scene node.
	MAKO_INLINE Vec3df GetAbsolutePosition() const
	{ return absTransformation.GetTranslation(); }

	//! Gets the abolute position of the node.
	//! \return The current absolute position of the scene node.
	MAKO_INLINE Vec3df GetAbsoluteRotation() const
	{ return absTransformation.GetRotationDegrees(); }

	//! Gets the abolute position of the node.
	//! \return The current absolute position of the scene node.
	MAKO_INLINE Vec3df GetAbsoluteScale() const
	{ return absTransformation.GetScale(); }
	
	//! Get the absolute transformation of the node. Is recalculated before every 
	//! frame.
	//! \return The absolute transformation matrix.
	MAKO_INLINE const Matrix4f& GetAbsoluteTransformation() const
	{ return absTransformation; }

	//! Get the scene that this node is in
	//! \return The scene
	MAKO_INLINE Scene3d* GetScene() const
	{ return scene; }
};

MAKO_END_NAMESPACE