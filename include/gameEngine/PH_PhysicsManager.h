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

#include <PH_CoreModule.h>
#include <PH_Singleton.h>

#include "PH_CollisionMesh.h"
#include "PH_GameEngineAPI.h"


namespace Phobos
{
	class Transform_c;

	namespace Physics
	{		
		PH_DECLARE_SINGLETON_PTR(PhysicsManager);

		class PH_GAME_ENGINE_API PhysicsManager_c: public CoreModule_c
		{
			PH_DECLARE_SINGLETON_METHODS(PhysicsManager);

			public:
				enum CollisionShapeTypes_e
				{
					CST_BOX,					
					CST_SPHERE,
					CST_MESH
				};

				typedef boost::shared_ptr<btCollisionShape> CollisionShapeSharedPtr_t;

				CollisionShapeSharedPtr_t CreateBoxShape(Float_t x, Float_t y, Float_t z);
				CollisionShapeSharedPtr_t CreateMeshShape(const Ogre::Mesh &mesh);

				btRigidBody *CreateRigidBody(const Transform_c &transform, btCollisionShape &shape, Float_t mass);
				void DestroyRigidBody(btRigidBody *body);				

			protected:
				virtual void OnBoot();

				virtual void OnFixedUpdate();

			private:
				typedef boost::weak_ptr<btCollisionShape> CollisionShapeWeakPtr_t;				

				struct CollisionShapeComparator_s;
				typedef bool (*CompareShapeProc_t)(const CollisionShapeComparator_s &, const CollisionShapeComparator_s &);

				struct BoxShapeInfo_s
				{
					Float_t v3Dimension[3];
				};

				struct SphereShapeInfo_s
				{
					Float_t fpRadius;
				};

				struct CollisionShapeComparator_s
				{
					inline CollisionShapeComparator_s(BoxShapeInfo_s &info, CompareShapeProc_t proc);
					inline CollisionShapeComparator_s(const Ogre::Mesh &mesh, CompareShapeProc_t proc);

					bool operator<(const CollisionShapeComparator_s &lhs) const; 

					CollisionShapeTypes_e eType;

					union 
					{
						BoxShapeInfo_s stBox;
						SphereShapeInfo_s stSphere;
					} uShapeInfo;

					//Valid only when eType == CST_MESH
					//We do not insert it on the union because it is an smart pointer (no POD)
					boost::shared_ptr<CollisionMesh_c> spCollisionMesh;
					String_c strMeshName;

					CompareShapeProc_t pfnLessThanProc;
				};

				typedef std::map<CollisionShapeComparator_s, CollisionShapeWeakPtr_t> CollisionShapesMap_t;

			private:

				PhysicsManager_c();
				~PhysicsManager_c();	
				
				inline CollisionShapeComparator_s CreateBoxComparator(Float_t x, Float_t y, Float_t z) const;

				inline CollisionShapeComparator_s CreateMeshComparator(const Ogre::Mesh &mesh) const;

				static bool CompareBoxShape(const CollisionShapeComparator_s &lhs, const CollisionShapeComparator_s &rhs);
				static bool CompareMeshShape(const CollisionShapeComparator_s &lhs, const CollisionShapeComparator_s &rhs);

				CollisionShapeSharedPtr_t RetrieveCollisionShape(CollisionShapesMap_t::iterator &retIt, const CollisionShapeComparator_s &comp);

			private:
				boost::scoped_ptr<btDiscreteDynamicsWorld> spWorld;
				boost::scoped_ptr<btCollisionDispatcher> spCollisionDispatcher;

				btDefaultCollisionConfiguration			clCollisionConfig;
				btDbvtBroadphase						clBroadphase;
				btSequentialImpulseConstraintSolver		clConstraintSolver;				

				//
				//Usar um std::map
				//Cada componente da fisica deve armazenar um weak_ptr para o elemento do mapa
				//Quando o componente for destruído, checar o weak_ptr, se der null, remover elemento do mapa
				//Ou entao fazer um flush quando for feito o reset do engine de fisica				
				CollisionShapesMap_t mapCollisionShapes;

		};
	}
}

#endif