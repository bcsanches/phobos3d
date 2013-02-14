#ifndef PH_EDITOR_CAMERA_H
#define PH_EDITOR_CAMERA_H

#include <PH_GameCamera.h>

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
				GameCamera_c m_clCamera;
		};
	}
}


#endif
