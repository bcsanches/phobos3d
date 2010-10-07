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
