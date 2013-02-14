/*
Phobos 3d
October 2011
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose,
including commercial applications, and to alter it and redistribute it freely,
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_COLLISION_MESH_H
#define PH_COLLISION_MESH_H

#include <vector>

#include <boost/intrusive/set.hpp>

#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>

#include <OgrePrerequisites.h>

#include <Phobos/DisableCopy.h>
#include <Phobos/Types.h>
#include <Phobos/String.h>

#include "PH_CollisionMeshFwd.h"

class btBvhTriangleMeshShape;

namespace Phobos
{
	namespace Physics
	{
		class CollisionMeshData
		{
			PH_DISABLE_COPY(CollisionMeshData);

			public:
				struct Vertex_s
				{
					Float32_t m_fp32Point[3];
				};

			public:			
				CollisionMeshData(const Ogre::Mesh &mesh);

				void AddMesh(const Ogre::Mesh &mesh);

				inline UInt_t GetNumVertices(void) const;
				inline UInt_t GetNumIndices(void) const;
				inline UInt_t GetNumTriangles(void) const;
				inline const Vertex_s *GetVertices(void) const;
				inline const UInt32_t *GetIndices(void) const;

				inline btStridingMeshInterface &GetMeshInterface(void);

			private:	
				void AddVertexData(const Ogre::VertexData &vertex);
				void AddIndexData(const Ogre::IndexData &index, UInt_t offset = 0);

			private:
				std::vector<Vertex_s> m_vecVertices;				
				std::vector<UInt32_t> m_vecIndices;	

				btTriangleIndexVertexArray	m_clIndexVertexArray;
		};		

		inline UInt_t CollisionMeshData::GetNumVertices(void) const
		{
			return m_vecVertices.size();
		}

		inline UInt_t CollisionMeshData::GetNumIndices(void) const
		{
			return m_vecIndices.size();
		}

		inline UInt_t CollisionMeshData::GetNumTriangles(void) const
		{
			return m_vecIndices.size() / 3;
		}

		inline const CollisionMeshData::Vertex_s *CollisionMeshData::GetVertices(void) const
		{
			return m_vecVertices.empty() ? NULL : &m_vecVertices[0];
		}

		inline const UInt32_t *CollisionMeshData::GetIndices(void) const
		{
			return m_vecIndices.empty() ? NULL : &m_vecIndices[0];
		}

		inline btStridingMeshInterface &CollisionMeshData::GetMeshInterface(void)
		{
			return(m_clIndexVertexArray);
		}

		//
		//
		//
		//
		//		
		class CollisionMesh: 
			public std::enable_shared_from_this<CollisionMesh>,			
			public boost::intrusive::set_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink> >
		{
			public:
				CollisionMesh(const Ogre::Mesh &mesh);

				inline const String_t &GetName() const;

				inline btBvhTriangleMeshShape &GetTriangleMeshShape() const;

				struct Comparator_s
				{
					bool operator()(const String_t &name, const CollisionMesh &collisionMesh) const;
					bool operator()(const CollisionMesh &collisionMesh, const String_t &name) const;
				};

				inline bool operator<(const CollisionMesh &) const;

			private:				
				String_t			m_strName;

				std::unique_ptr<CollisionMeshData> m_upMeshData;
				std::unique_ptr<btBvhTriangleMeshShape> m_upMeshShape;
		};

		inline const String_t &CollisionMesh::GetName() const
		{
			return m_strName;
		}

		inline btBvhTriangleMeshShape &CollisionMesh::GetTriangleMeshShape() const
		{
			return *m_upMeshShape;
		}

		inline bool CollisionMesh::operator<(const CollisionMesh &rhs) const
		{
			return m_strName < rhs.m_strName;
		}

		inline bool CollisionMesh::Comparator_s::operator()(const String_t &name, const CollisionMesh &collisionMesh) const
		{
			return name < collisionMesh.GetName();
		}

		inline bool CollisionMesh::Comparator_s::operator()(const CollisionMesh &collisionMesh, const String_t &name) const
		{
			return collisionMesh.GetName() < name;
		}
	}
}


#endif