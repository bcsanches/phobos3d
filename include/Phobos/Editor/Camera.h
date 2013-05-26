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

			private:
				Game::Camera m_clCamera;
		};
	}
}


#endif
