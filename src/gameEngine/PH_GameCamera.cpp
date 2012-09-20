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
		iViewportZOrder(-1),
		fpNearPlane(1)
	{
		Render_c &render = Render_c::GetInstance();
				
		pclRootNode = render.CreateSceneNode();

		this->CreateCamera();
	}

	GameCamera_c::~GameCamera_c()
	{
		Render_c &render = Render_c::GetInstance();

		if(iViewportZOrder >= 0)
			render.RemoveViewport(iViewportZOrder);

		if(pclCamera)
			render.DestroyCamera(pclCamera);

		if(pclRootNode)
			render.DestroySceneNode(pclRootNode);
	}

	void GameCamera_c::EnableViewport(int ZOrder)
	{
		Render_c &render = Render_c::GetInstance();

		if(iViewportZOrder >= 0)
			render.RemoveViewport(iViewportZOrder);

		iViewportZOrder = ZOrder;

		if(iViewportZOrder >= 0)
			render.AddViewport(pclCamera, ZOrder);
	}

	void GameCamera_c::SetTransform(const Transform_c &t)
	{		
		pclRootNode->setPosition(t.GetOrigin());
		pclRootNode->setOrientation(t.GetRotation());
	}

	void GameCamera_c::SetCameraTransform(const Transform_c &t)
	{
		pclCamera->setPosition(t.GetOrigin());
		pclCamera->setOrientation(t.GetRotation());
	}

	void GameCamera_c::EnableFixedYawAxis(const Ogre::Vector3 &axis)
	{
		pclCamera->setFixedYawAxis(true, axis);
	}

	void GameCamera_c::SetNearClipDistance(const Float_t distance)
	{
		fpNearPlane = distance;
		pclCamera->setNearClipDistance(distance);
	}

	void GameCamera_c::SetFarClipDistance(const Float_t distance)
	{
		fpFarPlane = distance;
		pclCamera->setFarClipDistance(distance);
	}

	void GameCamera_c::SetFov(Ogre::Degree angle)
	{
		pclCamera->setFOVy(Ogre::Radian(angle));
	}

	void GameCamera_c::Enable()
	{
		//PH_ASSERT(pclCamera == NULL);

		//this->CreateCamera();
		this->EnableViewport(iViewportZOrder);
	}
	
	void GameCamera_c::Disable()
	{
		Render_c &render = Render_c::GetInstance();
		/*
		pclRootNode->detachObject(pclCamera);
		
		if(iViewportZOrder >= 0)
			render->RemoveViewport(iViewportZOrder);

		if(pclCamera)
			render->DestroyCamera(pclCamera);

		pclCamera = NULL;
		*/
		if(iViewportZOrder >= 0)
			render.RemoveViewport(iViewportZOrder);
	}

	void GameCamera_c::AddCameraListener(Ogre::Camera::Listener &listener)
	{
		pclCamera->addListener(&listener);
	}

	void GameCamera_c::CreateCamera()
	{
		Render_c &render = Render_c::GetInstance();
		
		pclCamera = render.CreateCamera();	

		pclRootNode->attachObject(pclCamera);

		if(iViewportZOrder >= 0)
			render.AddViewport(pclCamera, iViewportZOrder);

		pclCamera->setOrientation(Ogre::Quaternion(Ogre::Angle(180), Ogre::Vector3::UNIT_Y));	
		pclCamera->setNearClipDistance(fpNearPlane);
		
		this->SetFov(Ogre::Degree(75));
	}
}
