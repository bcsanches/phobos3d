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

#include "PH_CollisionMesh.h"

#include <OgreMesh.h>
#include <OgreSubMesh.h>

#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>

#include <PH_Error.h>
#include <PH_Memory.h>

namespace Phobos
{
	namespace Physics
	{
		CollisionMeshData_c::CollisionMeshData_c(const Ogre::Mesh &mesh)
		{
			this->AddMesh(mesh);
		}

		void CollisionMeshData_c::AddMesh(const Ogre::Mesh &mesh)
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

					this->AddIndexData(*subMesh.indexData, vecVertices.size());
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
			indexedMesh.m_vertexStride = sizeof(CollisionMeshData_c::Vertex_s);
			indexedMesh.m_vertexBase = (const unsigned char *) this->GetVertices();

			clIndexVertexArray.addIndexedMesh(indexedMesh);
		}

		void CollisionMeshData_c::AddVertexData(const Ogre::VertexData &vertexData)
		{
			const size_t prevNumVertices = vecVertices.size();
			vecVertices.resize(prevNumVertices + vertexData.vertexCount);

			const Ogre::VertexElement* posElem = vertexData.vertexDeclaration->findElementBySemantic(Ogre::VES_POSITION);		

			PH_ASSERT_VALID(posElem);

			Ogre::HardwareVertexBufferSharedPtr vbuf = vertexData.vertexBufferBinding->getBuffer(posElem->getSource());
			const unsigned int vSize = (unsigned int)vbuf->getVertexSize();

			unsigned char	*vertex = static_cast<unsigned char*>(vbuf->lock(Ogre::HardwareBuffer::HBL_READ_ONLY));
			float			*pReal;
			Vertex_s		*curVertices = &vecVertices[prevNumVertices];

			const unsigned int vertexCount = (unsigned int)vertexData.vertexCount;
			for(unsigned int j = 0; j < vertexCount; ++j)
			{
				posElem->baseVertexPointerToElement(vertex, &pReal);
				vertex += vSize;

				curVertices->fp32Point[0] = (*pReal++);
				curVertices->fp32Point[1] = (*pReal++);
				curVertices->fp32Point[2] = (*pReal++);		
		            
				curVertices++;
			}
			vbuf->unlock();
		}

		void CollisionMeshData_c::AddIndexData(const Ogre::IndexData &index, UInt_t offset)
		{
			using namespace Ogre;

			const UInt_t prevNumIndices = vecIndices.size();
			vecIndices.resize(prevNumIndices + index.indexCount);
			
			const UInt_t numTris = (UInt_t) index.indexCount / 3;
			HardwareIndexBufferSharedPtr ibuf = index.indexBuffer;	
			const bool use32bitindexes = (ibuf->getType() == HardwareIndexBuffer::IT_32BIT);
			unsigned int indexOffset = prevNumIndices;

			if (use32bitindexes) 
			{
				const UInt32_t* pInt = static_cast<UInt32_t*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
				for(unsigned int k = 0; k < numTris; ++k)
				{
					vecIndices[indexOffset ++] = offset + *pInt++;
					vecIndices[indexOffset ++] = offset + *pInt++;
					vecIndices[indexOffset ++] = offset + *pInt++;
				}
				ibuf->unlock();
			}
			else 
			{
				const UInt16_t* pShort = static_cast<UInt16_t*>(ibuf->lock(HardwareBuffer::HBL_READ_ONLY));
				for(unsigned int k = 0; k < numTris; ++k)
				{		
					vecIndices[indexOffset ++] = offset + static_cast<UInt32_t> (*pShort++);
					vecIndices[indexOffset ++] = offset + static_cast<UInt32_t> (*pShort++);
					vecIndices[indexOffset ++] = offset + static_cast<UInt32_t> (*pShort++);
				}
				ibuf->unlock();
			}
		}

		CollisionMesh_c::CollisionMesh_c(const Ogre::Mesh &mesh):
			spMeshData(PH_NEW CollisionMeshData_c(mesh)),
			strName(mesh.getName())			
		{			
			spMeshShape.reset(new btBvhTriangleMeshShape(&spMeshData->GetMeshInterface(), true, true));
		}
	}
}
