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

#include <PH_Console.h>
#include <PH_Core.h>

#include "PH_CharacterBodyComponent.h"
#include "PH_CollisionShapes.h"
#include "PH_GhostCharacterBody.h"
#include "PH_PhysicsManager.h"
#include "PH_PhysicsUtils.h"
#include "PH_RigidBody.h"
#include "PH_RigidBodyComponent.h"
#include "PH_SweepCharacterBody.h"

namespace Phobos
{
	namespace Physics
	{
		PH_DEFINE_DEFAULT_SINGLETON(PhysicsManager);

		PhysicsManager_c::PhysicsManager_c():
			CoreModule_c("PhysicsManager", PRIVATE_CHILDREN),
			fpScale(1),
			varPhysicsScale("dvPhysicsScale", "1"),
			clBroadphase(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000))
		{
			//empty
		}

		PhysicsManager_c::~PhysicsManager_c()
		{
			//empty
		}

		void PhysicsManager_c::OnPrepareToBoot()
		{
			ConsolePtr_t console = Console_c::GetInstance();

			console->AddContextVar(varPhysicsScale);
		}

		void PhysicsManager_c::OnBoot()
		{
			spCollisionDispatcher.reset(new btCollisionDispatcher(&clCollisionConfig));
			spWorld.reset(new btDiscreteDynamicsWorld(spCollisionDispatcher.get(), &clBroadphase, &clConstraintSolver, &clCollisionConfig));

			spWorld->getDispatchInfo().m_allowedCcdPenetration=0.0001f;
			clBroadphase.getOverlappingPairCache()->setInternalGhostPairCallback(&clGhostPairCallback); 

			fpScale = StringToFloat(varPhysicsScale.GetValue());
		}

		void PhysicsManager_c::OnFixedUpdate()
		{
			if(!spWorld)
				return;

			const CoreTimer_s &timer = Core_c::GetInstance()->GetGameTimer();
			if(timer.IsPaused())
				return;

			clRigidBodyComponents.CallForAll(&RigidBodyComponent_c::SaveTransform);
			clCharacterBodyComponents.CallForAll1(&CharacterBodyComponent_c::PreparePhysicsFrame, timer.fpFrameTime);

			spWorld->stepSimulation(timer.fpFrameTime, 32);

			clCharacterBodyComponents.CallForAll(&CharacterBodyComponent_c::FinishPhysicsFrame);
		}

		void PhysicsManager_c::OnUpdate()
		{
			//No world, no reason to update
			if(!spWorld)
				return;
			
			//No pause check, to allow client interpolation
			clRigidBodyComponents.CallForAll1(&RigidBodyComponent_c::UpdateTransform, Core_c::GetInstance()->GetGameTimer().fpDelta);
		}

		void PhysicsManager_c::SetGravity(const Ogre::Vector3 &gravity)
		{
			spWorld->setGravity(MakeVector3(gravity, fpScale));
		}

		const btVector3 PhysicsManager_c::GetPhysicsGravity() const
		{
			return spWorld->getGravity();
		}

		CharacterBodyPtr_t PhysicsManager_c::CreateCharacterBody(const Ogre::Vector3 &startPosition, Float_t stepHeight, Float_t radius, Float_t height)
		{	
#if 0
			RigidBodyPtr_t body = this->CreateBoxRigidBody(RBT_KINEMATIC, Transform_c(startPosition), 0, radius, height, radius);
			CharacterBodyPtr_t ptr =  boost::make_shared<SweepCharacterBody_c>(body, stepHeight);
#else
			
			CollisionShapePtr_t shape = this->CreateCapsuleShape(radius, height);			
			CharacterBodyPtr_t ptr = boost::make_shared<GhostCharacterBody_c>(stepHeight, shape);
			ptr->Teleport(startPosition);
#endif

			return ptr;
		}

		RigidBodyPtr_t PhysicsManager_c::CreateMeshRigidBody(RigidBodyTypes_e type, const Transform_c &transform, Float_t mass, const Ogre::Mesh &mesh, const Ogre::Vector3 &scale)
		{
			CollisionShapePtr_t collisionShape = this->CreateMeshShape(mesh, scale);

			return this->CreateRigidBody(type, transform, collisionShape, mass);
		}

		RigidBodyPtr_t PhysicsManager_c::CreateBoxRigidBody(RigidBodyTypes_e type, const Transform_c &transform, Float_t mass, Float_t dimx, Float_t dimy, Float_t dimz)
		{
			CollisionShapePtr_t collisionShape = this->CreateBoxShape(dimx, dimy, dimz);

			return this->CreateRigidBody(type, transform, collisionShape, mass);
		}

		RigidBodyPtr_t PhysicsManager_c::CreateCapsuleRigidBody(RigidBodyTypes_e type, const Transform_c &transform, Float_t mass, Float_t radius, Float_t height)
		{
			CollisionShapePtr_t collisionShape = this->CreateCapsuleShape(radius, height);

			return this->CreateRigidBody(type, transform, collisionShape, mass);
		}

		RigidBodyPtr_t PhysicsManager_c::CreateRigidBody(RigidBodyTypes_e type, const Transform_c &transform, CollisionShapePtr_t shape, Float_t mass)
		{
			bool dynamic = mass != 0;

			btVector3 localInertia(0, 0, 0);

			btCollisionShape &btShape = shape->GetCollisionShape();

			if(dynamic)
				btShape.calculateLocalInertia(mass, localInertia);			

			btDefaultMotionState *motionState = new btDefaultMotionState(MakeTransform(transform, fpScale));

			btRigidBody::btRigidBodyConstructionInfo info(mass, motionState, &btShape, localInertia);

			return boost::make_shared<RigidBody_c>(type, info, motionState, shape);			
		}				

		void PhysicsManager_c::RegisterRigidBody(btRigidBody &body, short group, short mask)
		{
			spWorld->addRigidBody(&body, group, mask);
		}

		void PhysicsManager_c::UnregisterRigidBody(btRigidBody &body)
		{
			spWorld->removeRigidBody(&body);
		}

		void PhysicsManager_c::AddCollisionObject(btCollisionObject &collisionObject,short int collisionFilterGroup,short int collisionFilterMask)
		{
			spWorld->addCollisionObject(&collisionObject, collisionFilterGroup, collisionFilterMask);
		}

		void PhysicsManager_c::RemoveCollisionObject(btCollisionObject &collisionObject)
		{
			spWorld->removeCollisionObject(&collisionObject);
		}

		void PhysicsManager_c::AddAction(btActionInterface &action)
		{
			spWorld->addAction(&action);
		}

		void PhysicsManager_c::RemoveAction(btActionInterface &action)
		{
			spWorld->removeAction(&action);
		}

		
		bool PhysicsManager_c::RetrieveCollisionShape(CollisionShapesSet_t::iterator &retIt, const CollisionShape_c::Key_s &key)
		{			
			retIt = setCollisionShapesCache.lower_bound(key, CollisionShape_c::KeyComparator_s());

			//not found the element?
			if((retIt != setCollisionShapesCache.end()) && (!CollisionShape_c::KeyComparator_s()(key, *retIt)))
			{
				return true;
			}			
			else
			{
				return false;
			}						
		}		

		CollisionShapePtr_t PhysicsManager_c::CreateBoxShape(Float_t x, Float_t y, Float_t z)
		{
			const CollisionShape_c::BoxShapeInfo_s box = {x, y, z};
			CollisionShape_c::Key_s key(box, this->GetScale());
			
			
			CollisionShapesSet_t::iterator it;
			if(!this->RetrieveCollisionShape(it, key))
			{			
				//create it and store on the map
				CollisionShapePtr_t ptr = boost::make_shared<BoxCollisionShape_c>(Ogre::Vector3(x, y, z), fpScale);

				setCollisionShapesCache.insert(it, *ptr);

				return ptr;
			}
			else
			{
				return it->shared_from_this();
			}			
		}

		CollisionShapePtr_t PhysicsManager_c::CreateCapsuleShape(Float_t radius, Float_t height)
		{
			const CollisionShape_c::CapsuleShapeInfo_s capsule = {radius, height};
			CollisionShape_c::Key_s key(capsule, this->GetScale());
			
			
			CollisionShapesSet_t::iterator it;
			if(!this->RetrieveCollisionShape(it, key))
			{			
				//create it and store on the map
				CollisionShapePtr_t ptr = boost::make_shared<CapsuleCollisionShape_c>(radius, height, fpScale);

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
		
		
		CollisionShapePtr_t PhysicsManager_c::CreateMeshShape(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale)
		{
			CollisionShape_c::Key_s key(mesh, scale, this->GetScale());

			CollisionShapesSet_t::iterator it;			

			//Element does not exists?
			if(!this->RetrieveCollisionShape(it, key))
			{
				CollisionMeshPtr_t collisionMesh = this->RetrieveCollisionMesh(mesh);

				CollisionShapePtr_t ptr = boost::make_shared<ScaledMeshCollissionShape_c>(collisionMesh, scale, fpScale);

				setCollisionShapesCache.insert(it, *ptr);				

				return ptr;
			}			

			return it->shared_from_this();	
		}

		//
		//
		// Collision Detection
		//
		//

		class ClosestNotMeConvexResultCallback_c: public btCollisionWorld::ClosestConvexResultCallback
		{
			public:
				ClosestNotMeConvexResultCallback_c (const btCollisionObject &me) : 
				  btCollisionWorld::ClosestConvexResultCallback(btVector3(0.0, 0.0, 0.0), btVector3(0.0, 0.0, 0.0)),
				  rclMe(me)
				{
					//empty
				}	

				virtual btScalar addSingleResult(btCollisionWorld::LocalConvexResult& convexResult,bool normalInWorldSpace)
				{
					if (convexResult.m_hitCollisionObject == &rclMe)
						return 1.0f;

					if(convexResult.m_hitCollisionObject->getCollisionFlags() & btCollisionObject::CF_NO_CONTACT_RESPONSE)
						return 1.0f;			

					return ClosestConvexResultCallback::addSingleResult (convexResult, normalInWorldSpace);
				}		
				
			protected:
				const btCollisionObject &rclMe;
		};

		void PhysicsManager_c::ConvexSweepTest(SweepCollisionResult_s &result, const btRigidBody &body, const btTransform &start, const btTransform &end)
		{			
			ClosestNotMeConvexResultCallback_c callback (body);

			callback.m_collisionFilterGroup = body.getBroadphaseHandle()->m_collisionFilterGroup;
			callback.m_collisionFilterMask = body.getBroadphaseHandle()->m_collisionFilterMask;
								
			spWorld->convexSweepTest (static_cast<const btConvexShape *>(body.getCollisionShape()), start, end, callback);			

			result.fpFraction = callback.m_closestHitFraction;
			result.fHasHit = callback.hasHit();
			result.v3HitPointWorld = callback.m_hitPointWorld;
			result.v3HitNormalWorld = callback.m_hitNormalWorld;

			//result.pclContact = static_cast<IM_BtRigidBody_c *>(callback.m_hitCollisionObject ? callback.m_hitCollisionObject->getUserPointer() : NULL);
		}
		
		//
		//
		//
		//
		//COMPONENTS

		void PhysicsManager_c::RegisterRigidBodyComponent(RigidBodyComponent_c &comp)
		{
			clRigidBodyComponents.Register(comp);
		}
		
		void PhysicsManager_c::UnregisterRigidBodyComponent(RigidBodyComponent_c &comp)
		{
			clRigidBodyComponents.Unregister(comp);
		}

		void PhysicsManager_c::RegisterCharacterBodyComponent(CharacterBodyComponent_c &comp)
		{
			clCharacterBodyComponents.Register(comp);
		}

		void PhysicsManager_c::UnregisterCharacterBodyComponent(CharacterBodyComponent_c &comp)
		{
			clCharacterBodyComponents.Unregister(comp);
		}
	}
}
