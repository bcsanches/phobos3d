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

#include <PH_Singleton.h>

#include "PH_BaseCollisionShape.h"
#include "PH_CollisionMesh.h"
#include "PH_GameEngineAPI.h"
#include "PH_GenericComponentManager.h"

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

				BaseCollisionShapePtr_t CreateBoxShape(Float_t x, Float_t y, Float_t z);
				BaseCollisionShapePtr_t CreateMeshShape(const Ogre::Mesh &mesh, const Ogre::Vector3 &scale);

				btRigidBody *CreateRigidBody(const Transform_c &transform, BaseCollisionShape_c &shape, Float_t mass);
				void DestroyRigidBody(btRigidBody *body);

				void RegisterRigidBody(btRigidBody &body);
				void UnregisterRigidBody(btRigidBody &body);

			protected:
				virtual void OnBoot();

				virtual void OnFixedUpdate();
				virtual void OnUpdate();

			private:								
				typedef boost::intrusive::set<BaseCollisionShape_c, boost::intrusive::constant_time_size<false> > CollisionShapesSet_t;
				typedef boost::intrusive::set<CollisionMesh_c, boost::intrusive::constant_time_size<false> > CollisionMeshesSet_t;				

			private:

				PhysicsManager_c();											

				bool RetrieveCollisionShape(CollisionShapesSet_t::iterator &retIt, const BaseCollisionShape_c::Key_s &key);

				CollisionMeshPtr_t RetrieveCollisionMesh(const Ogre::Mesh &mesh);				

			private:
				boost::scoped_ptr<btDiscreteDynamicsWorld> spWorld;
				boost::scoped_ptr<btCollisionDispatcher> spCollisionDispatcher;

				btDefaultCollisionConfiguration			clCollisionConfig;
				btDbvtBroadphase						clBroadphase;
				btSequentialImpulseConstraintSolver		clConstraintSolver;				

				
				CollisionShapesSet_t setCollisionShapesCache;

				//Because scaled meshes references the original mesh we keep a cache of all meshes that were been loaded
				CollisionMeshesSet_t setCollisionMeshesCache;

		};
	}
}

#endif