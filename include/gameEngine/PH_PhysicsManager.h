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

#include <OgreMesh.h>

#include <PH_ContextVar.h>
#include <PH_Singleton.h>

#include "PH_CollisionShape.h"
#include "PH_CollisionMesh.h"
#include "PH_GameEngineAPI.h"
#include "PH_GenericComponentManager.h"
#include "PH_RigidBodyFwd.h"

namespace Phobos
{
	class Transform_c;

	namespace Physics
	{		
		class RigidBodyComponent_c;

		PH_DECLARE_SINGLETON_PTR(PhysicsManager);

		class PH_GAME_ENGINE_API PhysicsManager_c: public GenericComponentManager_c<RigidBodyComponent_c>
		{
			PH_DECLARE_SINGLETON_METHODS(PhysicsManager);

			public:
				enum CollisionShapeTypes_e
				{
					CST_BOX,					
					CST_SPHERE,
					CST_MESH
				};				

				~PhysicsManager_c();				

				RigidBodyPtr_t CreateBoxRigidBody(const Transform_c &transform, Float_t mass, Float_t dimx, Float_t dimy, Float_t dimz);
				RigidBodyPtr_t CreateCapsuleRigidBody(const Transform_c &transform, Float_t mass, Float_t radius, Float_t height);
				RigidBodyPtr_t CreateMeshRigidBody(const Transform_c &transform, Float_t mass, const Ogre::Mesh &mesh, const Ogre::Vector3 &scale);							

				void SetGravity(const Ogre::Vector3 &gravity);

				inline Float_t GetScale() const;
				inline Float_t GetPhysicsToGameScale() const;

			protected:
				virtual void OnPrepareToBoot();
				virtual void OnBoot();

				virtual void OnFixedUpdate();
				virtual void OnUpdate();

			private:								
				typedef boost::intrusive::set<CollisionShape_c, boost::intrusive::constant_time_size<false> > CollisionShapesSet_t;
				typedef boost::intrusive::set<CollisionMesh_c, boost::intrusive::constant_time_size<false> > CollisionMeshesSet_t;				

			private:

				PhysicsManager_c();											

				bool RetrieveCollisionShape(CollisionShapesSet_t::iterator &retIt, const CollisionShape_c::Key_s &key);

				CollisionMeshPtr_t RetrieveCollisionMesh(const Ogre::Mesh &mesh);		

				CollisionShapePtr_t CreateBoxShape(Float_t x, Float_t y, Float_t z);
				CollisionShapePtr_t CreateCapsuleShape(Float_t radius, Float_t height);
				CollisionShapePtr_t CreateMeshShape(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale);

				RigidBodyPtr_t CreateRigidBody(const Transform_c &transform, CollisionShapePtr_t shape, Float_t mass);

				//
				//
				//RigidBody class interface
				void RegisterRigidBody(btRigidBody &body);
				void UnregisterRigidBody(btRigidBody &body);				

			private:
				boost::scoped_ptr<btDiscreteDynamicsWorld> spWorld;
				boost::scoped_ptr<btCollisionDispatcher> spCollisionDispatcher;

				btDefaultCollisionConfiguration			clCollisionConfig;
				btDbvtBroadphase						clBroadphase;
				btSequentialImpulseConstraintSolver		clConstraintSolver;		

				Float_t									fpScale;

				ContextVar_c							varPhysicsScale;

				
				CollisionShapesSet_t setCollisionShapesCache;

				//Because scaled meshes references the original mesh we keep a cache of all meshes that were been loaded
				CollisionMeshesSet_t setCollisionMeshesCache;

				friend class RigidBody_c;

		};

		inline Float_t PhysicsManager_c::GetScale() const
		{
			return fpScale;
		}

		inline Float_t PhysicsManager_c::GetPhysicsToGameScale() const
		{
			return 1.0f / this->GetScale() ;
		}
	}
}

#endif