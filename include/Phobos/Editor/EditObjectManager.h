#ifndef PH_EDITOR_EDIT_OBJECT_MANAGER_H
#define PH_EDITOR_EDIT_OBJECT_MANAGER_H

#include <Phobos/DisableCopy.h>
#include <Phobos/Game/MapDefs.h>
#include <Phobos/Node.h>
#include <Phobos/OgreEngine/Math/TransformFwd.h>


#include <map>

namespace Phobos
{
	namespace Editor
	{
		class EditObject;

		class EditObjectManager: public Node
		{		
			PH_DISABLE_COPY(EditObjectManager);

			public:
				EditObjectManager();

				void CreateEditObject(const String_t &asset, Game::MapObjectTypes type, const Engine::Math::Transform &transform);

			private:
				String_t GenerateName(UInt64_t id) const;

			private:
				typedef std::map<String_t, EditObject *> EditObjectsMap_t;

				EditObjectsMap_t	m_mapEditObjects;

				UInt64_t			m_uNextId;
		};
	}
}

#endif
