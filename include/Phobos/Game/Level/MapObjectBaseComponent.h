#ifndef PH_GAME_MAP_OBJECT_BASE_COMPONENT_H
#define PH_GAME_MAP_OBJECT_BASE_COMPONENT_H

#include <Phobos/DisableCopy.h>

#include "Phobos/Game/Level/MapObject.h"

namespace Phobos
{
	namespace Game
	{
		class MapObjectBaseComponent
		{
			PH_DISABLE_COPY(MapObjectBaseComponent);

			public:
				struct PointerTraits
				{
					typedef Phobos::Game::MapObjectComponent *pointer;

					void operator()(pointer ptr)
					{
						ptr->Release();
					}
				};

		public:
			virtual void Release() = 0;

		protected:
			MapObjectComponent(MapObject &owner) :
				m_rclOwner(owner)
			{
				//empty
			}

			virtual ~MapObjectComponent()
			{
				//empty
			}


			MapObject::ComponentAccess AccessMapObject()
			{
				return MapObject::ComponentAccess(m_rclOwner);
			}

		private:
			MapObject &m_rclOwner;
		};
	}
}

#endif
