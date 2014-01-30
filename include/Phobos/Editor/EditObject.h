#ifndef PH_EDITOR_EDIT_OBJECT_H
#define PH_EDITOR_EDIT_OBJECT_H

#include <Phobos/Node.h>

namespace Phobos
{
	namespace Editor
	{
		class EditObject : public Node
		{
			public:
				EditObject(UInt_t id, const char *name);
				EditObject(UInt_t id, String_t &&name);

				private:
					UInt_t m_uId;
		};
	}
}

#endif