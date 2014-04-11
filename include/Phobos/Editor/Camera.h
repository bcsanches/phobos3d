#ifndef PH_EDITOR_CAMERA_H
#define PH_EDITOR_CAMERA_H

#include <Phobos/Game/Camera.h>

namespace Phobos
{
	namespace Editor
	{
		class Camera
		{
			public:
				Camera();
				~Camera();

				void Enable()
				{
					m_clCamera.Enable();
				}

				void Disable()
				{
					m_clCamera.Disable();
				}

				inline Engine::Math::Transform GetTransform()
				{
					return m_clCamera.GetTransform();
				}

			private:
				Game::Camera m_clCamera;
		};
	}
}


#endif
