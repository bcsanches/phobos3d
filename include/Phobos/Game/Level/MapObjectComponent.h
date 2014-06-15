#ifndef PH_GAME_MAP_OBJECT_COMPONENT_H
#define PH_GAME_MAP_OBJECT_COMPONENT_H

#include <Phobos/DisableCopy.h>

#include <Phobos/Error.h>

#include <memory>

namespace Phobos
{
	namespace Game
	{
		class MapObject;
		class MapObjectComponentAccess;

		class MapObjectComponent
		{
			PH_DISABLE_COPY(MapObjectComponent);

			public:
				struct PointerTraits
				{
					typedef Phobos::Game::MapObjectComponent *pointer;

					void operator()(pointer ptr)
					{
						ptr->Release();
					}
				};

				typedef std::unique_ptr<MapObjectComponent, PointerTraits> UniquePtr_t;

			public:				
				virtual void Release() = 0;

				const char *GetType() const
				{
					return m_pszType;
				}

			protected:
				MapObjectComponent(const char *type, MapObject &owner) :
					m_rclOwner(owner),
					m_pszType(type)
				{
					PH_ASSERT_VALID(type);
				}

				virtual ~MapObjectComponent()
				{
					//empty
				}
				
				MapObjectComponentAccess AccessMapObject();

			private:
				MapObject	&m_rclOwner;
				const char	*m_pszType;
		};		
	}
}

#endif
