#ifndef PH_GAME_RENDER_WORLD_H
#define PH_GAME_RENDER_WORLD_H

#include <Phobos/Singleton.h>

#include <Phobos/Register/HiveFwd.h>

#include <OgrePrerequisites.h>

#include "Phobos/Game/GameAPI.h"

namespace Phobos
{
	namespace Game
	{
		PH_DECLARE_SINGLETON_PTR(RenderWorld);

		class SceneNodeObject;

		class SceneNodeHandler
		{
			private:
				SceneNodeHandler(const SceneNodeHandler &rhs);
				SceneNodeHandler &operator=(const SceneNodeHandler &rhs);

			public:
				~SceneNodeHandler();
				SceneNodeHandler();
				SceneNodeHandler(SceneNodeObject &object, UInt16_t index, UInt16_t serial);
				SceneNodeHandler(SceneNodeHandler &&other);

				SceneNodeHandler &operator=(SceneNodeHandler &&other);

				void SetPosition(const Ogre::Vector3 &position);
				void SetOrientation(const Ogre::Quaternion &orientation);

			private:
				//The handler owns the object, not the memory
				//Must call RenderWorld::DestroyDynamicnode to destroy it
				SceneNodeObject		*m_pclSceneNode;

				UInt16_t			m_u16Index;
				UInt16_t			m_u16Serial;
		};

		class PH_GAME_API RenderWorld
		{
			PH_DECLARE_SINGLETON_METHODS(RenderWorld);

			friend class RenderWorldLoadAccess;

			public:
				//The returned handler will owns the object, when it dies, it destroy the object
				virtual SceneNodeHandler AcquireDynamicSceneNodeHandler(StringRef_t handler) = 0;

			protected:
				RenderWorld() {};
				virtual ~RenderWorld() {}

			protected:
				virtual void Load(const Register::Hive &hive) = 0;			
				virtual void Unload() = 0;
		};

		class RenderWorldLoadAccess
		{
			friend class MapLoader;

			private:
				static void Load(const Register::Hive &hive)
				{
					RenderWorld::GetInstance().Load(hive);
				}

				static void Unload()
				{
					RenderWorld::GetInstance().Unload();
				}
		};
	}
}

#endif
