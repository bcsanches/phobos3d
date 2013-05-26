/*
Phobos 3d
October 2010
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

#ifndef PH_SPECTATOR_CAMERA_H
#define PH_SPECTATOR_CAMERA_H

#include <Phobos/DisableCopy.h>

#include "Phobos/Game/Camera.h"
#include "Phobos/Game/IController.h"

#include "Phobos/Game/SpectatorCameraCommandProducer.h"

namespace Phobos
{
	namespace Game
	{
		class PH_GAME_API SpectatorCamera: public IController
		{
			PH_DISABLE_COPY(SpectatorCamera);

			public:
				SpectatorCamera();

				void FixedUpdate(Engine::IPlayerCmdPtr_t cmd);
				void Update();

				void EnableProducer();
				void DisableProducer();

				void EnableMouse();
				void DisableMouse();

				void Enable();
				void Disable();			

				inline void SetCameraNearClipDistance(Float_t near);
				inline void SetCameraFarClipDistance(Float_t far);

				void SetTransform(const Engine::Math::Transform &transform);		

			private:
				Camera						m_clCamera;			

				Engine::Math::Transform		m_clCurrentTransform;

				Engine::Math::TransformInterpolator	m_clTransformInterpolator;

				float						m_fpAlpha;

				Ogre::Degree				m_dTurn;
				Ogre::Degree				m_dLook;	
		};
	
		inline void SpectatorCamera::SetCameraNearClipDistance(Float_t nearDist)
		{
			m_clCamera.SetNearClipDistance(nearDist);
		}

		inline void SpectatorCamera::SetCameraFarClipDistance(Float_t farDist)
		{
			m_clCamera.SetFarClipDistance(farDist);
		}
	}
}

#endif
