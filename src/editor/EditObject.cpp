#include "Phobos/Editor/EditObject.h"

namespace Phobos
{
	namespace Editor
	{
		EditObject::EditObject(UInt_t id, const char *name) :
			Node(name),
			m_uId(id)
		{
			//empty
		}

		EditObject::EditObject(UInt_t id, String_t &&name) :
			Node(name),
			m_uId(id)
		{

		}
	}
}

