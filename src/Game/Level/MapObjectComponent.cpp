#include "Phobos/Game/Level/MapObjectComponent.h"

#include "Phobos/Game/Level/MapObjectComponentAccess.h"

namespace Phobos
{
	namespace Game
	{ 
		MapObjectComponentAccess MapObjectComponent::AccessMapObject()
		{
			return MapObjectComponentAccess(m_rclOwner);
		}
	}
}
