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

#include "PH_GameCamera.h"

#include <PH_Render.h>
#include <PH_Transform.h>

namespace Phobos
{
	GameCamera_c::GameCamera_c():
		iViewportZOrder(-1)
	{
		RenderPtr_t render = Render_c::GetInstance();

		pclRootNode = render->CreateSceneNode();
		pclCamera = render->CreateCamera();	

		pclRootNode->attachObject(pclCamera);	
	
		pclCamera->setOrientation(Ogre::Quaternion(Ogre::Angle(180), Ogre::Vector3::UNIT_Y));	
	}

	GameCamera_c::~GameCamera_c()
	{
		RenderPtr_t render = Render_c::GetInstance();

		if(iViewportZOrder >= 0)
			render->RemoveViewport(iViewportZOrder);

		if(pclCamera)
			render->DestroyCamera(pclCamera);

		if(pclRootNode)
			render->DestroySceneNode(pclRootNode);
	}

	void GameCamera_c::EnableViewport(int ZOrder)
	{
		RenderPtr_t render = Render_c::GetInstance();

		if(iViewportZOrder >= 0)
			render->RemoveViewport(iViewportZOrder);

		iViewportZOrder = ZOrder;

		if(iViewportZOrder >= 0)
			Render_c::GetInstance()->AddViewport(pclCamera, ZOrder);
	}

	void GameCamera_c::SetTransform(const Transform_c &t)
	{		
		pclRootNode->setPosition(t.GetOrigin());
		pclRootNode->setOrientation(t.GetRotation());
	}

	void GameCamera_c::EnableFixedYawAxis(const Ogre::Vector3 &axis)
	{
		pclCamera->setFixedYawAxis(true, axis);
	}

	void GameCamera_c::SetNearClipDistance(const Float_t distance)
	{
		pclCamera->setNearClipDistance(distance);
	}
}
