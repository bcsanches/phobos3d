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

#include <boost/noncopyable.hpp>

#include <OgrePrerequisites.h>

#include <PH_Transform.h>

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class PH_GAME_ENGINE_API GameCamera_c: boost::noncopyable
	{
		public:
			GameCamera_c();
			~GameCamera_c();

			void EnableViewport(int ZOrder = 0);

			void SetTransform(const Transform_c &t);

			void EnableFixedYawAxis(const Ogre::Vector3 &axis);

			void SetNearClipDistance(const Float_t distance);

			void Enable();
			void Disable();

		private:
			Ogre::SceneNode	*pclRootNode;
			Ogre::Camera	*pclCamera;

			int iViewportZOrder;
	};
}

#endif

