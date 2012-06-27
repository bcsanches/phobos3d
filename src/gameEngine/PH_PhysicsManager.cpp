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

#include "PH_BaseCollisionShape.h"
#include "PH_CollisionShapes.h"
#include "PH_PhysicsManager.h"
#include "PH_PhysicsUtil.h"
#include "PH_RigidBodyComponent.h"

namespace Phobos
{
	namespace Physics
	{
		PH_DEFINE_DEFAULT_SINGLETON(PhysicsManager);

		PhysicsManager_c::PhysicsManager_c():
			GenericComponentManager_c("PhysicsManager", PRIVATE_CHILDREN)
		{
		}

		PhysicsManager_c::~PhysicsManager_c()
		{
			//empty
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

		btRigidBody *PhysicsManager_c::CreateRigidBody(const Transform_c &transform, BaseCollisionShape_c &shape, Float_t mass)
		{
			bool dynamic = mass != 0;

			btVector3 localInertia(0, 0, 0);

			btCollisionShape &btShape = shape.GetCollisionShape();

			if(dynamic)
				btShape.calculateLocalInertia(mass, localInertia);

			btDefaultMotionState *motionState = new btDefaultMotionState(MakeTransform(transform));

			btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, &btShape, localInertia);

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

		
		bool PhysicsManager_c::RetrieveCollisionShape(CollisionShapesSet_t::iterator &retIt, const BaseCollisionShape_c::Key_s &key)
		{			
			retIt = setCollisionShapesCache.lower_bound(key, BaseCollisionShape_c::KeyComparator_s());

			//not found the element?
			if((retIt != setCollisionShapesCache.end()) && (!BaseCollisionShape_c::KeyComparator_s()(key, *retIt)))
			{
				return true;
			}			
			else
			{
				return false;
			}						
		}		

		BaseCollisionShapePtr_t PhysicsManager_c::CreateBoxShape(Float_t x, Float_t y, Float_t z)
		{
			const BaseCollisionShape_c::BoxShapeInfo_s box = {x, y, z};
			BaseCollisionShape_c::Key_s key(box);
			
			
			CollisionShapesSet_t::iterator it;
			if(!this->RetrieveCollisionShape(it, key))
			{			
				//create it and store on the map
				BaseCollisionShapePtr_t ptr = boost::make_shared<BoxCollisionShape_c>(Ogre::Vector3(x, y, z));

				setCollisionShapesCache.insert(it, *ptr);

				return ptr;
			}
			else
			{
				return it->shared_from_this();
			}			
		}

		CollisionMeshPtr_t PhysicsManager_c::RetrieveCollisionMesh(const Ogre::Mesh &mesh)
		{			
			CollisionMeshesSet_t::iterator it = setCollisionMeshesCache.lower_bound(mesh.getName(), CollisionMesh_c::Comparator_s());

			if((it != setCollisionMeshesCache.end()) && (!CollisionMesh_c::Comparator_s()(mesh.getName(), *it)))
			{						
				return it->shared_from_this();
			}
			else
			{
				//not found, create a new one
				CollisionMeshPtr_t ptr = boost::make_shared<CollisionMesh_c>(mesh);

				setCollisionMeshesCache.insert(it, *ptr);

				return ptr;
			}			
		}
		
		
		BaseCollisionShapePtr_t PhysicsManager_c::CreateMeshShape(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale)
		{
			BaseCollisionShape_c::Key_s key(mesh, scale);

			CollisionShapesSet_t::iterator it;			

			//Element does not exists?
			if(!this->RetrieveCollisionShape(it, key))
			{
				CollisionMeshPtr_t collisionMesh = this->RetrieveCollisionMesh(mesh);

				BaseCollisionShapePtr_t ptr = boost::make_shared<ScaledMeshCollissionShape_c>(collisionMesh, scale);

				setCollisionShapesCache.insert(it, *ptr);				

				return ptr;
			}			

			return it->shared_from_this();	
		}				
	}
}
