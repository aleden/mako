#pragma once

#include "MakoMesh.h"
#include "MakoMeshData.h"
#include "MakoArrayList.h"

MAKO_BEGIN_NAMESPACE

class SimpleMesh : public Mesh
{
private:
	// ArrayList-MeshData-Iterator
	typedef ArrayList<MeshData*>::iterator almbit;

	//! MeshDatas
	ArrayList<MeshData*> mbs;

	////! MeshData Materials
	//ArrayList<MeshData*> mbms;
public:
	MAKO_INLINE SimpleMesh() {}

	MAKO_INLINE virtual ~SimpleMesh()
	{ for (almbit it = mbs.begin(); it != mbs.end(); ++it) (*it)->Drop(); }

	virtual MAKO_INLINE bool IsAnimated() const
	{ return false; }

	virtual MAKO_INLINE UInt32 GetNumSubMeshes() const
	{ return mbs.size(); }
	
	virtual MAKO_INLINE const MeshData* GetSubMesh(UInt32 index) const
	{ return mbs[index]; }

	virtual MAKO_INLINE MeshData* GetSubMesh(UInt32 index)
	{ return mbs[index]; }

	virtual void AddSubMesh(MeshData* mb)
	{ mb->Hold(); mbs.push_back(mb); }

	//virtual void AddSubMesh(MeshData* mb)
	//{
	//	mb->Hold();
	//	mbs.push_back(mb);
	//	if (mbms.size() != mbs.size())
	//	{
	//		UInt32 oldSize = mbms.size();
	//		mbms.resize(mbs.size());

	//		for (UInt i = oldSize; i < mbms.size(); ++i)
	//			mbms[i] = nullptr;
	//	}
	//}

	//virtual MAKO_INLINE void SetMaterial(Material* mat, const UInt32 index)
	//{ mbms[index] = mat; }
	//
	//virtual MAKO_INLINE Material* GetMaterial(UInt32 index)
	//{ return materials[index]; }
};

MAKO_END_NAMESPACE