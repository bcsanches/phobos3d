#ifndef PH_PHOBOS_GAME_LEVEL_RIGID_BODY_COMPONENT_H
#define PH_PHOBOS_GAME_LEVEL_RIGID_BODY_COMPONENT_H

#include "Phobos/Game/Level/MapObjectComponent.h"

#include "Phobos/Game/Physics/RigidBody.h"

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

				//static MapObjectComponent::UniquePtr_t Create(MapObject &owner, const Register::Table &table);

			protected:
				//virtual void Release() override;

				RigidBodyComponent(const char *type, MapObject &owner, const Register::Table &table, Physics::RigidBody &&rigidBody);

			protected:
				Physics::RigidBody m_clRigidBody;
		};

		class DynamicBodyComponent : public RigidBodyComponent
		{
			public:
				DynamicBodyComponent(Game::MapObject &owner, const Register::Table &table);

				static MapObjectComponent::UniquePtr_t Create(Game::MapObject &owner, const Register::Table &table);

				static void SyncAllToPhysics();

			protected:
				virtual void Release() override;
		};

	}
}

#endif