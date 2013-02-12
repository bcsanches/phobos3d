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

#ifndef PH_PHYSICS_MANAGER_H
#define PH_PHYSICS_MANAGER_H

#include <map>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <OgreMesh.h>

#include <Phobos/Shell/Variable.h>
#include <Phobos/Singleton.h>

#include "PH_CollisionMesh.h"
#include "PH_CollisionShape.h"
#include "PH_CollisionTagFwd.h"
#include "PH_GameEngineAPI.h"
#include "PH_GenericComponentManagerModule.h"
#include "PH_ICharacterBodyFwd.h"
#include "PH_RigidBodyFwd.h"

namespace Phobos
{
	class Transform;

	namespace Physics
	{						
		PH_DECLARE_SINGLETON_PTR(Manager);

		struct SweepCollisionResult_s
		{	
			btVector3				m_v3HitPointWorld;
			btVector3				m_v3HitNormalWorld;

			Float_t					m_fpFraction;

			bool					m_fHasHit;

			//RigidBody				*pclContact;
		};				

		class CharacterBodyComponent;
		class RigidBodyComponent;

		class PH_GAME_ENGINE_API Manager: public CoreModule
		{
			PH_DECLARE_SINGLETON_METHODS(Manager);

			public:
				~Manager();				

				/**

					For quick and easy rigid body creation.

				*/
				RigidBodyPtr_t CreateBoxRigidBody(RigidBodyTypes_e type, const Transform &transform, Float_t mass, const CollisionTag &collisionTag, Float_t dimx, Float_t dimy, Float_t dimz);
				RigidBodyPtr_t CreateCapsuleRigidBody(RigidBodyTypes_e type, const Transform &transform, Float_t mass, const CollisionTag &collisionTag, Float_t radius, Float_t height);
				RigidBodyPtr_t CreateMeshRigidBody(RigidBodyTypes_e type, const Transform &transform, Float_t mass, const CollisionTag &collisionTag, const Ogre::Mesh &mesh, const Ogre::Vector3 &scale);

				/**
					Methods for manually creating a collision shape

				*/
				CollisionShapePtr_t CreateBoxShape(Float_t x, Float_t y, Float_t z);
				CollisionShapePtr_t CreateCapsuleShape(Float_t radius, Float_t height);
				CollisionShapePtr_t CreateMeshShape(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale);

				CollisionShapePtr_t CreateCylinderShapeX(Float_t radius, Float_t height);
				CollisionShapePtr_t CreateCylinderShapeY(Float_t radius, Float_t height);
				CollisionShapePtr_t CreateCylinderShapeZ(Float_t radius, Float_t height);

				/**

					Create a rigid body from an existing collision shape.

				*/
				RigidBodyPtr_t CreateRigidBody(RigidBodyTypes_e type, const Transform &transform, Float_t mass, const CollisionTag &collisionTag, CollisionShapePtr_t shape);

				CharacterBodyPtr_t CreateCharacterBody(const Ogre::Vector3 &startPosition, const CollisionTag &collisionTag, Float_t stepHeight, Float_t radius, Float_t height);

				void SetGravity(const Ogre::Vector3 &gravity);

				inline Float_t GetScale() const;
				inline Float_t GetPhysicsToGameScale() const;

				//
				//
				//Components
				void RegisterRigidBodyComponent(RigidBodyComponent &comp);
				void UnregisterRigidBodyComponent(RigidBodyComponent &comp);

				void RegisterCharacterBodyComponent(CharacterBodyComponent &comp);
				void UnregisterCharacterBodyComponent(CharacterBodyComponent &comp);

			protected:
				virtual void OnPrepareToBoot();
				virtual void OnBoot();

				virtual void OnFixedUpdate();
				virtual void OnUpdate();				

			private:
				template<typename T>
				CollisionShapePtr_t CreateGenericCylinderShape(Float_t radius, Float_t height, CollisionShapeTypes_t type);

			private:								
				typedef boost::intrusive::set<CollisionShape, boost::intrusive::constant_time_size<false> > CollisionShapesSet_t;
				typedef boost::intrusive::set<CollisionMesh, boost::intrusive::constant_time_size<false> > CollisionMeshesSet_t;				

			private:

				Manager();											

				bool RetrieveCollisionShape(CollisionShapesSet_t::iterator &retIt, const CollisionShape::Key_s &key);

				CollisionMeshPtr_t RetrieveCollisionMesh(const Ogre::Mesh &mesh);						

				const btVector3 GetPhysicsGravity() const;

				//
				//
				//RigidBody class interface
				void RegisterRigidBody(btRigidBody &body, const CollisionTag &collisionTag);
				void UnregisterRigidBody(btRigidBody &body);

				void AddCollisionObject(btCollisionObject &collisionObject,const CollisionTag &collisionTag);
				void RemoveCollisionObject(btCollisionObject &collisionObject);				

				//
				//
				//Character body interface

				void AddAction(btActionInterface &action);
				void RemoveAction(btActionInterface &action);

				//
				//
				//Collision interface
				void ConvexSweepTest(SweepCollisionResult_s &result, const btRigidBody &body, const btTransform &start, const btTransform &end);

			private:
				std::unique_ptr<btDiscreteDynamicsWorld> m_upWorld;
				std::unique_ptr<btCollisionDispatcher> m_upCollisionDispatcher;

				btDefaultCollisionConfiguration			m_clCollisionConfig;
				btAxisSweep3							m_clBroadphase;
				btSequentialImpulseConstraintSolver		m_clConstraintSolver;		

				btGhostPairCallback						m_clGhostPairCallback;

				Float_t									m_fpScale;

				Shell::Variable							m_varPhysicsScale;

				
				CollisionShapesSet_t m_setCollisionShapesCache;

				//Because scaled meshes references the original mesh we keep a cache of all meshes that were been loaded
				CollisionMeshesSet_t m_setCollisionMeshesCache;

				GenericComponentManager<RigidBodyComponent>			m_clRigidBodyComponents;
				GenericComponentManager<CharacterBodyComponent>		m_clCharacterBodyComponents;

				friend class RigidBody;
				friend class SweepCharacterBody;
				friend class GhostCharacterBody;

		};

		inline Float_t Manager::GetScale() const
		{
			return m_fpScale;
		}

		inline Float_t Manager::GetPhysicsToGameScale() const
		{
			return 1.0f / this->GetScale() ;
		}
	}
}

#endif
