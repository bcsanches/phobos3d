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

#include <boost/noncopyable.hpp>

#include <PH_GameCamera.h>
#include <PH_IGameController.h>

#include "PH_SpectatorCameraCommandProducer.h"

namespace Phobos
{
	class PH_GAME_ENGINE_API SpectatorCamera_c: public IGameController_c, boost::noncopyable
	{
		public:
			SpectatorCamera_c();

			void FixedUpdate();
			void Update();

			void EnableProducer();
			void DisableProducer();

			void EnableMouse();
			void DisableMouse();

			void Enable();
			void Disable();

			inline void SetMouseSensitivity(Float_t value);
			inline void SetMoveSpeed(Float_t value);
			inline void SetTurnSpeed(Float_t value);

			inline void SetCameraNearClipDistance(Float_t near);
			inline void SetCameraFarClipDistance(Float_t far);

			void SetTransform(const Transform_c &transform);		

		private:
			GameCamera_c						clCamera;
			SpectatorCameraCommandProducer_c	clCameraCommandProducer;

			Transform_c					clCurrentTransform;

			TransformInterpolator_c		clTransformInterpolator;

			float						fpAlpha;

			Ogre::Degree				dTurn;
			Ogre::Degree				dLook;	
	};

	inline void SpectatorCamera_c::SetMouseSensitivity(Float_t value)
	{
		clCameraCommandProducer.SetMouseSensitivity(value);
	}

	inline void SpectatorCamera_c::SetMoveSpeed(Float_t value)
	{
		clCameraCommandProducer.SetMoveSpeed(value);
	}

	inline void SpectatorCamera_c::SetTurnSpeed(Float_t value)
	{
		clCameraCommandProducer.SetTurnSpeed(value);
	}

	inline void SpectatorCamera_c::SetCameraNearClipDistance(Float_t near)
	{
		clCamera.SetNearClipDistance(near);
	}

	inline void SpectatorCamera_c::SetCameraFarClipDistance(Float_t far)
	{
		clCamera.SetFarClipDistance(far);
	}
}

#endif