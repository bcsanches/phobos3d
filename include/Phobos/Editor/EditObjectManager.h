#ifndef PH_EDITOR_EDIT_OBJECT_MANAGER_H
#define PH_EDITOR_EDIT_OBJECT_MANAGER_H

#include <Phobos/Node.h>

#include <map>

namespace Phobos
{
	namespace Editor
	{
		class EditObject;

		class EditObjectManager: public Node
		{		
			public:
				EditObjectManager();

			private:
				typedef std::map<String_t, EditObject *> EditObjectsMap_t;

				EditObjectsMap_t m_mapEditObjects;
		};
	}
}

#endif
