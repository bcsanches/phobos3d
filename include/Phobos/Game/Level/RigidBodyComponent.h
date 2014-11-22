#ifndef PH_PHOBOS_GAME_LEVEL_RIGID_BODY_COMPONENT_H
#define PH_PHOBOS_GAME_LEVEL_RIGID_BODY_COMPONENT_H

#include "Phobos/Game/Level/MapObjectComponent.h"

#include "Phobos/Game/Physics/RigidBody.h"

#include <Phobos/Memory.h>
#include <Phobos/Register/TableFwd.h>

#define PH_DYNAMIC_BODY_COMPONENT_NAME "DynamicBody"
#define PH_STATIC_BODY_COMPONENT_NAME "StaticBody"

namespace Phobos
{
	namespace Game
	{
		class RigidBodyComponent : public MapObjectComponent
		{			
			public:
				virtual ~RigidBodyComponent() {};				

			protected:
				RigidBodyComponent(MapObject &owner, const Register::Table &table, Physics::RigidBody &&rigidBody);

			protected:
				Physics::RigidBody m_clRigidBody;
		};

		class DynamicBodyComponent : public RigidBodyComponent
		{
			public:
				PH_DECLARE_MEMORY_OPERATORS;

			public:
				DynamicBodyComponent(Game::MapObject &owner, const Register::Table &table);
				virtual ~DynamicBodyComponent();

				static MapObjectComponent::UniquePtr_t Create(Game::MapObject &owner, const Register::Table &table);

				static void SyncAllToPhysics();
		};

	}
}

#endif