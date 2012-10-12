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

#include <boost/scoped_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <btBulletDynamicsCommon.h>
#include <BulletCollision/CollisionDispatch/btGhostObject.h>

#include <OgreMesh.h>

#include <PH_ContextVar.h>
#include <PH_Singleton.h>

#include "PH_CollisionMesh.h"
#include "PH_CollisionShape.h"
#include "PH_CollisionTagFwd.h"
#include "PH_GameEngineAPI.h"
#include "PH_GenericComponentManagerModule.h"
#include "PH_ICharacterBodyFwd.h"
#include "PH_RigidBodyFwd.h"

namespace Phobos
{
	class Transform_c;

	namespace Physics
	{						
		PH_DECLARE_SINGLETON_PTR(Manager);

		struct SweepCollisionResult_s
		{	
			btVector3				v3HitPointWorld;
			btVector3				v3HitNormalWorld;

			Float_t					fpFraction;

			bool					fHasHit;

			//RigidBody_c				*pclContact;
		};				

		class CharacterBodyComponent_c;
		class RigidBodyComponent_c;

		class PH_GAME_ENGINE_API Manager_c: public CoreModule_c
		{
			PH_DECLARE_SINGLETON_METHODS(Manager);

			public:
				~Manager_c();				

				/**

					For quick and easy rigid body creation.

				*/
				RigidBodyPtr_t CreateBoxRigidBody(RigidBodyTypes_e type, const Transform_c &transform, Float_t mass, const CollisionTag_c &collisionTag, Float_t dimx, Float_t dimy, Float_t dimz);
				RigidBodyPtr_t CreateCapsuleRigidBody(RigidBodyTypes_e type, const Transform_c &transform, Float_t mass, const CollisionTag_c &collisionTag, Float_t radius, Float_t height);
				RigidBodyPtr_t CreateMeshRigidBody(RigidBodyTypes_e type, const Transform_c &transform, Float_t mass, const CollisionTag_c &collisionTag, const Ogre::Mesh &mesh, const Ogre::Vector3 &scale);

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
				RigidBodyPtr_t CreateRigidBody(RigidBodyTypes_e type, const Transform_c &transform, Float_t mass, const CollisionTag_c &collisionTag, CollisionShapePtr_t shape);

				CharacterBodyPtr_t CreateCharacterBody(const Ogre::Vector3 &startPosition, const CollisionTag_c &collisionTag, Float_t stepHeight, Float_t radius, Float_t height);

				void SetGravity(const Ogre::Vector3 &gravity);

				inline Float_t GetScale() const;
				inline Float_t GetPhysicsToGameScale() const;

				//
				//
				//Components
				void RegisterRigidBodyComponent(RigidBodyComponent_c &comp);
				void UnregisterRigidBodyComponent(RigidBodyComponent_c &comp);

				void RegisterCharacterBodyComponent(CharacterBodyComponent_c &comp);
				void UnregisterCharacterBodyComponent(CharacterBodyComponent_c &comp);

			protected:
				virtual void OnPrepareToBoot();
				virtual void OnBoot();

				virtual void OnFixedUpdate();
				virtual void OnUpdate();				

			private:
				template<typename T>
				CollisionShapePtr_t CreateGenericCylinderShape(Float_t radius, Float_t height, CollisionShapeTypes_t type);

			private:								
				typedef boost::intrusive::set<CollisionShape_c, boost::intrusive::constant_time_size<false> > CollisionShapesSet_t;
				typedef boost::intrusive::set<CollisionMesh_c, boost::intrusive::constant_time_size<false> > CollisionMeshesSet_t;				

			private:

				Manager_c();											

				bool RetrieveCollisionShape(CollisionShapesSet_t::iterator &retIt, const CollisionShape_c::Key_s &key);

				CollisionMeshPtr_t RetrieveCollisionMesh(const Ogre::Mesh &mesh);						

				const btVector3 GetPhysicsGravity() const;

				//
				//
				//RigidBody class interface
				void RegisterRigidBody(btRigidBody &body, const CollisionTag_c &collisionTag);
				void UnregisterRigidBody(btRigidBody &body);

				void AddCollisionObject(btCollisionObject &collisionObject,const CollisionTag_c &collisionTag);
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
				boost::scoped_ptr<btDiscreteDynamicsWorld> spWorld;
				boost::scoped_ptr<btCollisionDispatcher> spCollisionDispatcher;

				btDefaultCollisionConfiguration			clCollisionConfig;
				btAxisSweep3							clBroadphase;
				btSequentialImpulseConstraintSolver		clConstraintSolver;		

				btGhostPairCallback						clGhostPairCallback;

				Float_t									fpScale;

				ContextVar_c							varPhysicsScale;

				
				CollisionShapesSet_t setCollisionShapesCache;

				//Because scaled meshes references the original mesh we keep a cache of all meshes that were been loaded
				CollisionMeshesSet_t setCollisionMeshesCache;

				GenericComponentManager_c<RigidBodyComponent_c>			clRigidBodyComponents;
				GenericComponentManager_c<CharacterBodyComponent_c>	clCharacterBodyComponents;

				friend class RigidBody_c;
				friend class SweepCharacterBody_c;
				friend class GhostCharacterBody_c;

		};

		inline Float_t Manager_c::GetScale() const
		{
			return fpScale;
		}

		inline Float_t Manager_c::GetPhysicsToGameScale() const
		{
			return 1.0f / this->GetScale() ;
		}
	}
}

#endif