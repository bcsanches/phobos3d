#include "Phobos/Game/Level/RigidBodyComponent.h"

#include "Phobos/Game/Level/ComponentPool.h"
#include "Phobos/Game/Level/MapObject.h"
#include "Phobos/Game/Level/MapObjectComponentFactory.h"
#include "Phobos/Game/Level/MapObjectComponentAccess.h"

#include "Phobos/Game/Physics/Manager.h"
#include "Phobos/Game/Physics/Settings.h"
#include "Phobos/Game/Physics/PhysicsUtils.h"

#include <Phobos/Path.h>

#include <Phobos/Register/Table.h>
#include <Phobos/OgreEngine/Math/Transform.h>
#include <Phobos/OgreEngine/Utils.h>

#include <boost/pool/object_pool.hpp>

#include <OgreMeshManager.h>

namespace Phobos
{
	namespace Game
	{
		RigidBodyComponent::RigidBodyComponent(MapObject &owner, const Register::Table &table, Physics::RigidBody &&rigidBody) :
			MapObjectComponent(owner),
			m_clRigidBody(std::move(rigidBody))
		{
			m_clRigidBody.Register();
		}		

		//
		//Dynamic Bodies
		//

		static std::vector<DynamicBodyComponent *>		g_vecDynamicBodies;	
		
		PH_GAME_DEFINE_COMPONENT_POOL(DynamicBodyComponent, g_poolDynamicBodies);

		PH_MAP_COMPONENT_FULL_CREATOR(PH_DYNAMIC_BODY_COMPONENT_NAME, DynamicBodyComponent);
	
		DynamicBodyComponent::DynamicBodyComponent(Game::MapObject &owner, const Register::Table &table):
			RigidBodyComponent(
				owner, 
				table,
				Game::Physics::Manager::GetInstance().CreateRigidBody(
					Game::Physics::RBT_DYNAMIC,
					owner.MakeWorldTransform(),
					table.GetFloat("mass"),
					Game::Physics::Settings::LoadCollisionTag(table),
					Game::Physics::Utils::CreateCollisionShape(table, Ogre::Vector3(1, 1, 1))
				)
			)
		{
			g_vecDynamicBodies.push_back(this);
		}	

		DynamicBodyComponent::~DynamicBodyComponent()
		{			
			g_vecDynamicBodies.erase(std::remove_if(g_vecDynamicBodies.begin(), g_vecDynamicBodies.end(), [this](DynamicBodyComponent *current){return current == this; }), g_vecDynamicBodies.end());
		}

		void DynamicBodyComponent::SyncAllToPhysics()
		{
			for (auto comp : g_vecDynamicBodies)
			{
				const auto transform = comp->m_clRigidBody.GetTransform();

				comp->AccessMapObject().SetTransform(transform);				
			}
		}
	}
}

namespace
{
	using namespace Phobos;		

	static Game::Physics::RigidBody CreateStaticObjectRigidBody(const String_t &meshName, const Phobos::Engine::Math::Transform &transform, const Ogre::Vector3 &scale, const Phobos::Game::Physics::CollisionTag &collisionTag)
	{		
		auto pMesh = Ogre::MeshManager::getSingleton().load(meshName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME);

		Path path(meshName);
		path.StripExtension();

		auto &physicsManager = Game::Physics::Manager::GetInstance();

		auto collisionDef = Game::Physics::Settings::TryGetStaticMeshCollisionShapeDef(path.GetStr());

		auto body = collisionDef != NULL ?
			physicsManager.CreateRigidBody(Game::Physics::RBT_STATIC, transform, 0, collisionTag, Game::Physics::Utils::CreateCollisionShape(*collisionDef, scale)) :
			physicsManager.CreateMeshRigidBody(Game::Physics::RBT_STATIC, transform, 0, collisionTag, *pMesh, scale)
		;

		return body;
	}	

	class StaticBodyComponent : public Game::RigidBodyComponent
	{
		public:
			PH_DECLARE_MEMORY_OPERATORS;

		public:
			StaticBodyComponent::StaticBodyComponent(Game::MapObject &owner, const Register::Table &table);

			static MapObjectComponent::UniquePtr_t Create(Game::MapObject &owner, const Register::Table &table);		
	};
	
	PH_GAME_DEFINE_COMPONENT_POOL(StaticBodyComponent, g_poolStaticBodies);

	PH_MAP_COMPONENT_FULL_CREATOR(PH_STATIC_BODY_COMPONENT_NAME, StaticBodyComponent);

	StaticBodyComponent::StaticBodyComponent(Game::MapObject &owner, const Register::Table &table) :
		RigidBodyComponent(
			owner,
			table,
			CreateStaticObjectRigidBody(
				Phobos::OgreEngine::FixMeshName(table.GetString(PH_MAP_OBJECT_KEY_MESH)),
				owner.MakeWorldTransform(),
				owner.GetWorldScale(),
				Game::Physics::Settings::CreateStaticWorldCollisionTag()
			)
		)		
	{
		//empty
	}	
}

