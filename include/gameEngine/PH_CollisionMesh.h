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

#include <vector>

#include <BulletCollision/CollisionShapes/btTriangleIndexVertexArray.h>

#include <OgrePrerequisites.h>

#include <PH_Types.h>

namespace Phobos
{
	namespace Physics
	{
		class CollisionMesh_c
		{
			public:
				struct Vertex_s
				{
					Float32_t fp32Point[3];
				};

			public:			
				CollisionMesh_c(const Ogre::Mesh &mesh);

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
				std::vector<Vertex_s> vecVertices;				
				std::vector<UInt32_t> vecIndices;	

				btTriangleIndexVertexArray	clIndexVertexArray;
		};		

		inline UInt_t CollisionMesh_c::GetNumVertices(void) const
		{
			return vecVertices.size();
		}

		inline UInt_t CollisionMesh_c::GetNumIndices(void) const
		{
			return vecIndices.size();
		}

		inline UInt_t CollisionMesh_c::GetNumTriangles(void) const
		{
			return vecIndices.size() / 3;
		}

		inline const CollisionMesh_c::Vertex_s *CollisionMesh_c::GetVertices(void) const
		{
			return vecVertices.empty() ? NULL : &vecVertices[0];
		}

		inline const UInt32_t *CollisionMesh_c::GetIndices(void) const
		{
			return vecIndices.empty() ? NULL : &vecIndices[0];
		}

		inline btStridingMeshInterface &CollisionMesh_c::GetMeshInterface(void)
		{
			return(clIndexVertexArray);
		}
	}
}
