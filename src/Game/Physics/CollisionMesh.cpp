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

#include "Phobos/Game/Physics/CollisionMesh.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>

#include <Phobos/Error.h>
#include <Phobos/Memory.h>

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{
			CollisionMeshData::CollisionMeshData(const Ogre::Mesh &mesh)
			{
				this->AddMesh(mesh);
			}

			void CollisionMeshData::AddMesh(const Ogre::Mesh &mesh)
			{	
				using namespace Ogre;

				if(mesh.sharedVertexData)
					this->AddVertexData(*mesh.sharedVertexData);

				for(UInt_t numSubMeshes = mesh.getNumSubMeshes(), i = 0;i < numSubMeshes; ++i)
				{
					const SubMesh &subMesh(*mesh.getSubMesh(i));

					PH_ASSERT_VALID(subMesh.indexData);
					if(!subMesh.useSharedVertices)
					{
						PH_ASSERT_VALID(subMesh.vertexData);

						this->AddIndexData(*subMesh.indexData, m_vecVertices.size());
						this->AddVertexData(*subMesh.vertexData);
					}
					else
					{
						this->AddIndexData(*subMesh.indexData);
					}
				}			

				btIndexedMesh indexedMesh;
				indexedMesh.m_numTriangles = this->GetNumTriangles();
				indexedMesh.m_triangleIndexBase = (const unsigned char *) this->GetIndices();
				indexedMesh.m_triangleIndexStride = sizeof(UInt32_t) * 3;
				indexedMesh.m_numVertices = this->GetNumVertices();
				indexedMesh.m_vertexStride = sizeof(CollisionMeshData::Vertex_s);
				indexedMesh.m_vertexBase = (const unsigned char *) this->GetVertices();

				m_clIndexVertexArray.addIndexedMesh(indexedMesh);
			}

			void CollisionMeshData::AddVertexData(const Ogre::VertexData &vertexData)
			{
				const size_t prevNumVertices = m_vecVertices.size();
				m_vecVertices.resize(prevNumVertices + vertexData.vertexCount);

				const Ogre::VertexElement* posElem = vertexData.vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);		

				PH_ASSERT_VALID(posElem);

				Ogre::HardwareVertexBufferSharedPtr vbuf = vertexData.vertexBufferBinding->getBuffer(posElem->getSource());
				const unsigned int vSize = (unsigned int)vbuf->getVertexSize();

				unsigned char	*vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
				float			*pReal;
				Vertex_s		*curVertices = &m_vecVertices[prevNumVertices];

				const unsigned int vertexCount = (unsigned int)vertexData.vertexCount;
				for(unsigned int j = 0; j < vertexCount; ++j)
				{
					posElem->baseVertexPointerToElement(vertex, &pReal);
					vertex += vSize;

					curVertices->m_fp32Point[0] = (*pReal++);
					curVertices->m_fp32Point[1] = (*pReal++);
					curVertices->m_fp32Point[2] = (*pReal++);		
		            
					curVertices++;
				}
				vbuf->unlock();
			}

			void CollisionMeshData::AddIndexData(const Ogre::IndexData &index, UInt_t offset)
			{
				using namespace Ogre;

				const UInt_t prevNumIndices = m_vecIndices.size();
				m_vecIndices.resize(prevNumIndices + index.indexCount);
			
				const UInt_t numTris = (UInt_t) index.indexCount / 3;
				HardwareIndexBufferSharedPtr ibuf = index.indexBuffer;	
				const bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);
				unsigned int indexOffset = prevNumIndices;

				if (use32bitindexes) 
				{
					const UInt32_t* pInt = static_cast<UInt32_t*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
					for(unsigned int k = 0; k < numTris; ++k)
					{
						m_vecIndices[indexOffset ++] = offset + *pInt++;
						m_vecIndices[indexOffset ++] = offset + *pInt++;
						m_vecIndices[indexOffset ++] = offset + *pInt++;
					}
					ibuf->unlock();
				}
				else 
				{
					const UInt16_t* pShort = static_cast<UInt16_t*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
					for(unsigned int k = 0; k < numTris; ++k)
					{		
						m_vecIndices[indexOffset ++] = offset + static_cast<UInt32_t> (*pShort++);
						m_vecIndices[indexOffset ++] = offset + static_cast<UInt32_t> (*pShort++);
						m_vecIndices[indexOffset ++] = offset + static_cast<UInt32_t> (*pShort++);
					}
					ibuf->unlock();
				}
			}

			CollisionMesh::CollisionMesh(const Ogre::Mesh &mesh):
				m_upMeshData(PH_NEW CollisionMeshData(mesh)),
				m_strName(mesh.getName())			
			{			
				m_upMeshShape.reset(new btBvhTriangleMeshShape(&m_upMeshData->GetMeshInterface(), true, true));
			}
		}
	}
}
