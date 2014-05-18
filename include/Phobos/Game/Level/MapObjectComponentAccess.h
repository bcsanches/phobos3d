#ifndef PH_MAP_OBJECT_COMPONENT_ACCESS_H
#define PH_MAP_OBJECT_COMPONENT_ACCESS_H

#include "Phobos/Game/Level/MapObject.h"

namespace Phobos
{
	namespace Game
	{
		class MapObjectComponentAccess
		{
			public:
				friend class MapObjectComponent;

				MapObjectComponentAccess(MapObjectComponentAccess &rhs) :
					m_rclObject(rhs.m_rclObject)
				{
					//empty
				}

				void AttachOgreObject(Ogre::MovableObject &object)
				{
					m_rclObject.AttachOgreObject(object);
				}

			private:
				MapObjectComponentAccess(MapObject &owner) :
					m_rclObject(owner)
				{
					//empty
				}

				MapObject &m_rclObject;
			};
	}
}

#endif