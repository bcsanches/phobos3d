#ifndef PH_MAP_OBJECT_COMPONENT_ACCESS_H
#define PH_MAP_OBJECT_COMPONENT_ACCESS_H

#include "Phobos/Game/Level/MapObject.h"

#include <Phobos/OgreEngine/Math/Transform.h>

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

				void SetTransform(const Engine::Math::Transform &transform)
				{
					m_rclObject.SetTransform(transform);					
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