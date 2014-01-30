#include "Phobos/Editor/EditObjectManager.h"

#include "Phobos/Editor/EditObject.h"

#include "Phobos/Game/WorldManager.h"

namespace Phobos
{
	namespace Editor
	{
		EditObjectManager::EditObjectManager() :
			Node("EditObjectManager", Phobos::PRIVATE_CHILDREN),
			m_uNextId(0)
		{
			//empty
		}

		void EditObjectManager::CreateEditObject(const String_t &asset, Game::MapObjectTypes type, const Engine::Math::Transform &transform)
		{
			auto nextId = ++m_uNextId;		

			auto editObject = std::make_unique<EditObject>(nextId, this->GenerateName(nextId));

			auto mapObjectData = Game::WorldManager::GetInstance().MakeMapObject(
				editObject->GetName(),
				asset,
				type,
				transform
			);

			this->AddPrivateChild(std::move(editObject));
		}

		String_t EditObjectManager::GenerateName(UInt64_t id) const
		{
			std::stringstream name;

			for (;;)
			{
				name << "New Object " << id;

				if (m_mapEditObjects.find(name.str()) == m_mapEditObjects.end())
				{
					return name.str();
				}
				else
				{
					++id;
					name = std::stringstream();
				}
			}			
		}
	}
}

