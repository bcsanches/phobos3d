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

#include <PH_Core.h>

#include "PH_PhysicsManager.h"
#include "PH_PhysicsUtil.h"
#include "PH_RigidBodyComponent.h"

namespace Phobos
{
	namespace Physics
	{
		PH_DEFINE_DEFAULT_SINGLETON(PhysicsManager);

		inline PhysicsManager_c::CollisionShapeComparator_s::CollisionShapeComparator_s(BoxShapeInfo_s &info, CompareShapeProc_t proc):
			eType(CST_BOX),
			pfnLessThanProc(proc)
		{
			uShapeInfo.stBox = info;
		}

		inline PhysicsManager_c::CollisionShapeComparator_s::CollisionShapeComparator_s(const Ogre::Mesh &mesh, CompareShapeProc_t proc):
			eType(CST_MESH),
			pfnLessThanProc(proc),
			strMeshName(mesh.getName()),
			spCollisionMesh(new CollisionMesh_c(mesh))
		{

		}

		bool PhysicsManager_c::CollisionShapeComparator_s::operator<(const CollisionShapeComparator_s &rhs) const
		{
			if(eType == rhs.eType)
			{				
				return pfnLessThanProc(*this, rhs);
			}
			else
			{
				return eType < rhs.eType;
			}
		}

		PhysicsManager_c::PhysicsManager_c():
			GenericComponentManager_c("PhysicsManager", PRIVATE_CHILDREN)
		{
		}

		PhysicsManager_c::~PhysicsManager_c()
		{

		}

		void PhysicsManager_c::OnBoot()
		{
			spCollisionDispatcher.reset(new btCollisionDispatcher(&clCollisionConfig));
			spWorld.reset(new btDiscreteDynamicsWorld(spCollisionDispatcher.get(), &clBroadphase, &clConstraintSolver, &clCollisionConfig));
		}

		void PhysicsManager_c::OnFixedUpdate()
		{
			if(!spWorld)
				return;

			const CoreTimer_s &timer = Core_c::GetInstance()->GetGameTimer();
			if(timer.IsPaused())
				return;

			GenericComponentManager_c::CallForAll(&RigidBodyComponent_c::SaveTransform);
			spWorld->stepSimulation(timer.fpFrameTime, 32);
		}

		void PhysicsManager_c::OnUpdate()
		{
			//No world, no reason to update
			if(!spWorld)
				return;
			
			//No pause check, to allow client interpolation
			GenericComponentManager_c::CallForAll1(&RigidBodyComponent_c::UpdateTransform, Core_c::GetInstance()->GetGameTimer().fpDelta);
		}

		btRigidBody *PhysicsManager_c::CreateRigidBody(const Transform_c &transform, btCollisionShape &shape, Float_t mass)
		{
			bool dynamic = mass != 0;

			btVector3 localInertia(0, 0, 0);

			if(dynamic)
				shape.calculateLocalInertia(mass, localInertia);

			btDefaultMotionState *motionState = new btDefaultMotionState(MakeTransform(transform));

			btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, &shape, localInertia);

			return new btRigidBody(info);
		}

		void PhysicsManager_c::DestroyRigidBody(btRigidBody *body)
		{
			if(body->isInWorld())
				spWorld->removeRigidBody(body);

			delete body->getMotionState();
			delete body;
		}

		void PhysicsManager_c::RegisterRigidBody(btRigidBody &body)
		{
			spWorld->addRigidBody(&body);
		}

		void PhysicsManager_c::UnregisterRigidBody(btRigidBody &body)
		{
			spWorld->removeRigidBody(&body);
		}

		PhysicsManager_c::CollisionShapeSharedPtr_t PhysicsManager_c::RetrieveCollisionShape(CollisionShapesMap_t::iterator &retIt, const CollisionShapeComparator_s &comparator)
		{
			retIt = mapCollisionShapes.lower_bound(comparator);

			//found the element?
			if((retIt == mapCollisionShapes.end()) || (mapCollisionShapes.key_comp()(comparator, retIt->first)))
			{
				//Inser the new shape in the map
				retIt = mapCollisionShapes.insert(retIt, std::make_pair(comparator, CollisionShapeSharedPtr_t()));
			}			

			return retIt->second.lock();
		}

		inline PhysicsManager_c::CollisionShapeComparator_s PhysicsManager_c::CreateBoxComparator(Float_t x, Float_t y, Float_t z) const
		{
			BoxShapeInfo_s box = {x, y, z};
			return CollisionShapeComparator_s(box, PhysicsManager_c::CompareBoxShape);
		}				

		PhysicsManager_c::CollisionShapeSharedPtr_t PhysicsManager_c::CreateBoxShape(Float_t x, Float_t y, Float_t z)
		{
			CollisionShapeComparator_s comparator = this->CreateBoxComparator(x, y, z);

			CollisionShapesMap_t::iterator it;
			CollisionShapeSharedPtr_t ptr = this->RetrieveCollisionShape(it, comparator);			

			//Element does not exists?
			if(ptr.get() == NULL)
			{
				//create it and store on the map
				ptr.reset(new btBoxShape(btVector3(x * 0.5f, y * 0.5f, z * 0.5f)));
				it->second = ptr;
			}

			return ptr;			
		}

		bool PhysicsManager_c::CompareBoxShape(const CollisionShapeComparator_s &lhs, const CollisionShapeComparator_s &rhs)
		{
			for(int i = 0;i < 3; ++i)
			{
				if(lhs.uShapeInfo.stBox.v3Dimension[i] == rhs.uShapeInfo.stBox.v3Dimension[i])
					continue;

				return lhs.uShapeInfo.stBox.v3Dimension[i] < rhs.uShapeInfo.stBox.v3Dimension[i];
			}

			return false;
		}

		PhysicsManager_c::CollisionShapeComparator_s PhysicsManager_c::CreateMeshComparator(const Ogre::Mesh &mesh) const
		{
			return CollisionShapeComparator_s(mesh, PhysicsManager_c::CompareMeshShape);
		}

		PhysicsManager_c::CollisionShapeSharedPtr_t PhysicsManager_c::CreateMeshShape(const Ogre::Mesh &mesh)
		{
			CollisionShapeComparator_s comparator = this->CreateMeshComparator(mesh);

			CollisionShapesMap_t::iterator it;
			CollisionShapeSharedPtr_t ptr = this->RetrieveCollisionShape(it, comparator);			

			//Element does not exists?
			if(ptr.get() == NULL)
			{
				//create it and store on the map
				ptr.reset(new btBvhTriangleMeshShape(&it->first.spCollisionMesh->GetMeshInterface(), true));
				it->second = ptr;
			}

			return ptr;	
		}

		bool PhysicsManager_c::CompareMeshShape(const CollisionShapeComparator_s &lhs, const CollisionShapeComparator_s &rhs)
		{
			return lhs.strMeshName < rhs.strMeshName;
		}
	}
}
