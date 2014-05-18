#include "Phobos/Game/Level/MeshComponent.h"

#include <Phobos/OgreEngine/Render.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Game/Level/MapDefs.h"
#include "Phobos/Game/Level/MapObject.h"
#include "Phobos/Game/Level/MapObjectComponentAccess.h"
#include "Phobos/Game/Level/MapObjectComponentFactory.h"

#include <boost/pool/object_pool.hpp>

#include <OgreEntity.h>

namespace
{
	static boost::object_pool<Phobos::Game::MeshComponent> g_poolMeshes;
}

namespace Phobos
{
	namespace Game
	{
		PH_MAP_COMPONENT_FULL_CREATOR("Mesh", MeshComponent, g_poolMeshes);

		MeshComponent::MeshComponent(MapObject &owner, const Register::Table &table) :
			MapObjectComponent(owner),
			m_pclEntity(OgreEngine::Render::GetInstance().CreateEntity(table.GetString(PH_MAP_OBJECT_KEY_MESH)))
		{
			this->AccessMapObject().AttachOgreObject(*m_pclEntity);			
		}

		MeshComponent::~MeshComponent()
		{
			OgreEngine::Render::GetInstance().DestroyEntity(m_pclEntity);
		}

		void MeshComponent::Release()
		{
			g_poolMeshes.destroy(this);
		}
	}
}


