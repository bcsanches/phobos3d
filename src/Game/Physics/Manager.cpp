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

#include <memory>

#include <Phobos/Engine/Console.h>
#include <Phobos/Engine/Core.h>

#include "Phobos/Game/Physics/CharacterBodyComponent.h"
#include "Phobos/Game/Physics/CollisionShapes.h"
#include "Phobos/Game/Physics/CollisionTag.h"
#include "Phobos/Game/Physics/GhostCharacterBody.h"
#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/Conv.h"
#include "Phobos/Game/Physics/RigidBody.h"
#include "Phobos/Game/Physics/RigidBodyComponent.h"
#include "Phobos/Game/Physics/SweepCharacterBody.h"

namespace Phobos
{
	namespace Game
	{
		namespace Physics
		{
			PH_DEFINE_DEFAULT_SINGLETON2(Manager, Engine::Console &);

			Manager::Manager(Engine::Console &console) :
				Module("PhysicsManager", NodeFlags::PRIVATE_CHILDREN),
				m_fpScale(1),
				m_varPhysicsScale("dvPhysicsScale", "1"),
				m_clBroadphase(btVector3(-1000, -1000, -1000), btVector3(1000, 1000, 1000))
			{
				console.AddContextVariable(m_varPhysicsScale);
			}

			Manager::~Manager()
			{
				//empty
			}			

			void Manager::OnInit()
			{
				m_upConstraintSolver.reset(new btSequentialImpulseConstraintSolver());

				m_upCollisionDispatcher.reset(PH_NEW btCollisionDispatcher(&m_clCollisionConfig));
				m_upWorld.reset(new btDiscreteDynamicsWorld(m_upCollisionDispatcher.get(), &m_clBroadphase, m_upConstraintSolver.get(), &m_clCollisionConfig));

				m_upWorld->getDispatchInfo().m_allowedCcdPenetration=0.0001f;
				m_clBroadphase.getOverlappingPairCache()->setInternalGhostPairCallback(&m_clGhostPairCallback); 

				m_fpScale = std::stof(m_varPhysicsScale.GetValue());
			}

			void Manager::OnFixedUpdate()
			{
				if(!m_upWorld)
					return;

				auto &timer = Engine::Core::GetInstance().GetGameTimer();
				if(timer.IsPaused())
					return;

				m_clRigidBodyComponents.CallForAll(&RigidBodyComponent::SaveTransform);
				m_clCharacterBodyComponents.CallForAll1(&CharacterBodyComponent::PreparePhysicsFrame, timer.m_fpFrameTime);

				m_upWorld->stepSimulation(timer.m_fpFrameTime, 32);

				m_clCharacterBodyComponents.CallForAll(&CharacterBodyComponent::FinishPhysicsFrame);
			}

			void Manager::OnUpdate()
			{
				//No world, no reason to update
				if(!m_upWorld)
					return;
			
				//No pause check, to allow client interpolation
				m_clRigidBodyComponents.CallForAll1(&RigidBodyComponent::UpdateTransform, Engine::Core::GetInstance().GetGameTimer().m_fpDelta);
			}

			void Manager::SetGravity(const Ogre::Vector3 &gravity)
			{
				m_upWorld->setGravity(MakeVector3(gravity, m_fpScale));
			}

			const btVector3 Manager::GetPhysicsGravity() const
			{
				return m_upWorld->getGravity();
			}

			CharacterBodyPtr_t Manager::CreateCharacterBody(const Ogre::Vector3 &startPosition, const CollisionTag &collisionTag, Float_t stepHeight, Float_t radius, Float_t height)
			{	
	#if 0
				RigidBodyPtr_t body = this->CreateBoxRigidBody(RBT_KINEMATIC, Transform(startPosition), 0, radius, height, radius);
				CharacterBodyPtr_t ptr =  std::make_shared<SweepCharacterBody>(body, stepHeight);
	#else
			
				CollisionShapePtr_t shape = this->CreateCapsuleShape(radius, height);			
				CharacterBodyPtr_t ptr = std::make_shared<GhostCharacterBody>(stepHeight, shape, collisionTag);
				ptr->Teleport(startPosition);
	#endif

				return ptr;
			}

			RigidBody Manager::CreateMeshRigidBody(RigidBodyTypes_e type, const Engine::Math::Transform &transform, Float_t mass, const CollisionTag &collisionTag, const Ogre::Mesh &mesh, const Ogre::Vector3 &scale)
			{
				CollisionShapePtr_t collisionShape = this->CreateMeshShape(mesh, scale);

				return this->CreateRigidBody(type, transform, mass, collisionTag, collisionShape);
			}

			RigidBody Manager::CreateBoxRigidBody(RigidBodyTypes_e type, const Engine::Math::Transform &transform, Float_t mass, const CollisionTag &collisionTag, Float_t dimx, Float_t dimy, Float_t dimz)
			{
				CollisionShapePtr_t collisionShape = this->CreateBoxShape(dimx, dimy, dimz);

				return this->CreateRigidBody(type, transform, mass, collisionTag, collisionShape);
			}

			RigidBody Manager::CreateCapsuleRigidBody(RigidBodyTypes_e type, const Engine::Math::Transform &transform, Float_t mass, const CollisionTag &collisionTag, Float_t radius, Float_t height)
			{
				CollisionShapePtr_t collisionShape = this->CreateCapsuleShape(radius, height);

				return this->CreateRigidBody(type, transform, mass, collisionTag, collisionShape);
			}

			RigidBody Manager::CreateRigidBody(RigidBodyTypes_e type, const Engine::Math::Transform &transform, Float_t mass, const CollisionTag &collisionTag, CollisionShapePtr_t shape)
			{
				bool dynamic = mass != 0;

				btVector3 localInertia(0, 0, 0);

				btCollisionShape &btShape = shape->GetCollisionShape();

				if(dynamic)
					btShape.calculateLocalInertia(mass, localInertia);			
			
				std::unique_ptr<btDefaultMotionState> motionState(new btDefaultMotionState(MakeTransform(transform, m_fpScale)));

				btRigidBody::btRigidBodyConstructionInfo info(mass, motionState.get(), &btShape, localInertia);

				return RigidBody(type, info, std::move(motionState), shape, collisionTag);			
			}

			void Manager::RegisterRigidBody(btRigidBody &body, const CollisionTag &collisionTag)
			{
				m_upWorld->addRigidBody(&body, collisionTag.GetGroup(), collisionTag.GetFilter());
			}

			void Manager::UnregisterRigidBody(btRigidBody &body)
			{
				m_upWorld->removeRigidBody(&body);
			}

			void Manager::AddCollisionObject(btCollisionObject &collisionObject,const CollisionTag &collisionTag)
			{
				m_upWorld->addCollisionObject(&collisionObject, collisionTag.GetGroup(), collisionTag.GetFilter());
			}

			void Manager::RemoveCollisionObject(btCollisionObject &collisionObject)
			{
				m_upWorld->removeCollisionObject(&collisionObject);
			}

			void Manager::AddAction(btActionInterface &action)
			{
				m_upWorld->addAction(&action);
			}

			void Manager::RemoveAction(btActionInterface &action)
			{
				m_upWorld->removeAction(&action);
			}

		
			bool Manager::RetrieveCollisionShape(CollisionShapesSet_t::iterator &retIt, const CollisionShape::Key_s &key)
			{			
				retIt = m_setCollisionShapesCache.lower_bound(key, CollisionShape::KeyComparator_s());

				//not found the element?
				if((retIt != m_setCollisionShapesCache.end()) && (!CollisionShape::KeyComparator_s()(key, *retIt)))
				{
					return true;
				}			
				else
				{
					return false;
				}						
			}		

			CollisionShapePtr_t Manager::CreateBoxShape(Float_t x, Float_t y, Float_t z)
			{
				const CollisionShape::BoxShapeInfo_s box = {x, y, z};
				CollisionShape::Key_s key(box, this->GetScale());
			
			
				CollisionShapesSet_t::iterator it;
				if(!this->RetrieveCollisionShape(it, key))
				{			
					//create it and store on the map
					CollisionShapePtr_t ptr = std::make_shared<BoxCollisionShape>(Ogre::Vector3(x, y, z), m_fpScale);

					m_setCollisionShapesCache.insert(it, *ptr);

					return ptr;
				}
				else
				{
					return it->shared_from_this();
				}			
			}

			CollisionShapePtr_t Manager::CreateCapsuleShape(Float_t radius, Float_t height)
			{
				const CollisionShape::CylinderShapeInfo_s capsule = {radius, height};
				CollisionShape::Key_s key(CollisionShapeTypes::CAPSULE, capsule, this->GetScale());
			
			
				CollisionShapesSet_t::iterator it;
				if(!this->RetrieveCollisionShape(it, key))
				{			
					//create it and store on the map
					CollisionShapePtr_t ptr = std::make_shared<CapsuleCollisionShape>(radius, height, m_fpScale);

					m_setCollisionShapesCache.insert(it, *ptr);

					return ptr;
				}
				else
				{
					return it->shared_from_this();
				}			
			}

			template<typename T>
			CollisionShapePtr_t Manager::CreateGenericCylinderShape(Float_t radius, Float_t height, CollisionShapeTypes_t type)
			{
				const CollisionShape::CylinderShapeInfo_s cylinder = {radius, height};
				CollisionShape::Key_s key(type, cylinder, this->GetScale());			
			
				CollisionShapesSet_t::iterator it;
				if(!this->RetrieveCollisionShape(it, key))
				{			
					//create it and store on the map
					CollisionShapePtr_t ptr = std::make_shared<T>(radius, height, m_fpScale);

					m_setCollisionShapesCache.insert(it, *ptr);

					return ptr;
				}
				else
				{
					return it->shared_from_this();
				}			
			}

			CollisionShapePtr_t Manager::CreateCylinderShapeX(Float_t radius, Float_t height)
			{
				return CreateGenericCylinderShape<CylinderCollisionShapeX>(radius, height, CollisionShapeTypes::CYLINDER_X);
			}

			CollisionShapePtr_t Manager::CreateCylinderShapeY(Float_t radius, Float_t height)
			{
				return CreateGenericCylinderShape<CylinderCollisionShapeY>(radius, height, CollisionShapeTypes::CYLINDER_Y);
			}
		
			CollisionShapePtr_t Manager::CreateCylinderShapeZ(Float_t radius, Float_t height)
			{
				return CreateGenericCylinderShape<CylinderCollisionShapeZ>(radius, height, CollisionShapeTypes::CYLINDER_Z);
			}

			CollisionMeshPtr_t Manager::RetrieveCollisionMesh(const Ogre::Mesh &mesh)
			{			
				CollisionMeshesSet_t::iterator it = m_setCollisionMeshesCache.lower_bound(mesh.getName(), CollisionMesh::Comparator_s());

				if((it != m_setCollisionMeshesCache.end()) && (!CollisionMesh::Comparator_s()(mesh.getName(), *it)))
				{						
					return it->shared_from_this();
				}
				else
				{
					//not found, create a new one
					CollisionMeshPtr_t ptr = std::make_shared<CollisionMesh>(mesh);

					m_setCollisionMeshesCache.insert(it, *ptr);

					return ptr;
				}			
			}
		
		
			CollisionShapePtr_t Manager::CreateMeshShape(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale)
			{
				CollisionShape::Key_s key(mesh, scale, this->GetScale());

				CollisionShapesSet_t::iterator it;			

				//Element does not exists?
				if(!this->RetrieveCollisionShape(it, key))
				{
					CollisionMeshPtr_t collisionMesh = this->RetrieveCollisionMesh(mesh);

					CollisionShapePtr_t ptr = std::make_shared<ScaledMeshCollissionShape>(collisionMesh, scale, m_fpScale);

					m_setCollisionShapesCache.insert(it, *ptr);				

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

			void Manager::ConvexSweepTest(SweepCollisionResult_s &result, const btRigidBody &body, const btTransform &start, const btTransform &end)
			{			
				ClosestNotMeConvexResultCallback_c callback (body);

				callback.m_collisionFilterGroup = body.getBroadphaseHandle()->m_collisionFilterGroup;
				callback.m_collisionFilterMask = body.getBroadphaseHandle()->m_collisionFilterMask;
								
				m_upWorld->convexSweepTest (static_cast<const btConvexShape *>(body.getCollisionShape()), start, end, callback);			

				result.m_fpFraction = callback.m_closestHitFraction;
				result.m_fHasHit = callback.hasHit();
				result.m_v3HitPointWorld = callback.m_hitPointWorld;
				result.m_v3HitNormalWorld = callback.m_hitNormalWorld;

				//result.pclContact = static_cast<IM_BtRigidBody_c *>(callback.m_hitCollisionObject ? callback.m_hitCollisionObject->getUserPointer() : NULL);
			}
		
			//
			//
			//
			//
			//COMPONENTS

			void Manager::RegisterRigidBodyComponent(RigidBodyComponent &comp)
			{
				m_clRigidBodyComponents.Register(comp);
			}
		
			void Manager::UnregisterRigidBodyComponent(RigidBodyComponent &comp)
			{
				m_clRigidBodyComponents.Unregister(comp);
			}

			void Manager::RegisterCharacterBodyComponent(CharacterBodyComponent &comp)
			{
				m_clCharacterBodyComponents.Register(comp);
			}

			void Manager::UnregisterCharacterBodyComponent(CharacterBodyComponent &comp)
			{
				m_clCharacterBodyComponents.Unregister(comp);
			}
		}
	}
}
