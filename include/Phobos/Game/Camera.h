/*
Phobos 3d
October 2011
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/


#ifndef PH_GAME_CAMERA_H
#define PH_GAME_CAMERA_H

#include <OgrePrerequisites.h>
#include <OgreCamera.h>

#include <Phobos/DisableCopy.h>
#include <Phobos/OgreEngine/Render.h>
#include <Phobos/OgreEngine/Math/Transform.h>

#include "Phobos/Game/GameAPI.h"

namespace Phobos
{
	namespace Game
	{
		class PH_GAME_API Camera
		{
			public:
				Camera();
				~Camera();

				void EnableViewport(int ZOrder = OgreEngine::DefaultViewportZOrder::GAME);

				void SetTransform(const Engine::Math::Transform &t);
				void SetCameraTransform(const Engine::Math::Transform &t);

				Engine::Math::Transform GetTransform() const;

				void EnableFixedYawAxis(const Ogre::Vector3 &axis);

				void SetNearClipDistance(const Float_t distance);
				void SetFarClipDistance(const Float_t distance);

				void Enable();
				void Disable();

				void AddCameraListener(Ogre::Camera::Listener &listener);

				void SetFov(Ogre::Degree angle);

			private:
				void CreateCamera();

			private:
				OgreEngine::SceneNodeUniquePtr_t	m_upRootNode;
				Ogre::Camera						*m_pclCamera;

				int m_iViewportZOrder;

				Float_t m_fpNearPlane;
				Float_t m_fpFarPlane;

				PH_DISABLE_COPY(Camera);
		};
	}
}

#endif

