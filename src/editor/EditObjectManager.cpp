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

		EditObject &EditObjectManager::CreateEditObject(UInt64_t parentId, const String_t &asset, Game::MapObjectTypes type, const Engine::Math::Transform &transform)
		{
			EditObject *parentObject = nullptr;
			if (parentId)
			{ 
				parentObject = m_mapEditObjectIndex.find(parentId)->second;
			}

			auto nextId = ++m_uNextId;		

			auto editObject = std::make_unique<EditObject>(nextId, this->GenerateName(nextId));			

			auto mapObjectData = Game::WorldManager::GetInstance().CreateMapObject(
				editObject->GetName(),
				asset,
				type,
				transform
			);

			auto &finalEditObject = static_cast<EditObject &>(this->AddPrivateChild(std::move(editObject)));

			m_mapEditObjectIndex.insert(std::make_pair(nextId, &finalEditObject));
			m_mapEditObjects.insert(std::make_pair(finalEditObject.GetName(), &finalEditObject));

			return finalEditObject;
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

