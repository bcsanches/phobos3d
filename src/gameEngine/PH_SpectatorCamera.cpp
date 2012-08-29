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

#include "PH_SpectatorCamera.h"

#include <PH_Core.h>

namespace Phobos
{
	SpectatorCamera_c::SpectatorCamera_c()
	{
		clCamera.SetNearClipDistance(0.1f);		
		clCamera.SetFarClipDistance(100);
		
		clCamera.EnableViewport();		
	}

	void SpectatorCamera_c::FixedUpdate()
	{
		SpectatorCameraCmdPtr_t cmd = boost::static_pointer_cast<SpectatorCameraCmd_c>(clCameraCommandProducer.CreateCmd());

		using namespace Ogre;

		Degree turnAngle(cmd->GetTurn());
		Degree lookAngle(cmd->GetLook());

		Vector3 axis[3];
		
		const Quaternion &q = clCurrentTransform.GetRotation();

		q.ToAxes(axis[0], axis[1], axis[2]);	

		axis[2] *= cmd->GetMove();
		axis[0] *= cmd->GetStrafe();
		axis[1] = (Vector3::UNIT_Y * cmd->GetMoveUp());

		Vector3 movement = axis[0] + axis[1] + axis[2];
		clCurrentTransform.Translate(movement);	

		dTurn += turnAngle;
		dLook += lookAngle;

		Quaternion rot(dTurn, Vector3::UNIT_Y);
		rot = rot * Quaternion(dLook, Vector3::UNIT_X);	

		clCurrentTransform.SetRotation(rot);

		clTransformInterpolator.SetValue(clCurrentTransform);
	}

	void SpectatorCamera_c::Update()
	{
		clCamera.SetTransform(clTransformInterpolator.GetValue(Core_c::GetInstance()->GetSimInfo().stTimers[CORE_SYS_TIMER].fpDelta));
	}

	void SpectatorCamera_c::SetTransform(const Transform_c &transform)
	{
		clCurrentTransform = transform;

		dTurn = transform.GetRotation().getYaw().valueDegrees();
		dLook = transform.GetRotation().getPitch().valueDegrees();
	}

	void SpectatorCamera_c::EnableProducer()
	{
		clCameraCommandProducer.Enable();
	}
	
	void SpectatorCamera_c::DisableProducer()
	{
		clCameraCommandProducer.Disable();
	}

	void SpectatorCamera_c::EnableMouse()
	{
		clCameraCommandProducer.EnableMouse();
	}

	void SpectatorCamera_c::DisableMouse()
	{
		clCameraCommandProducer.DisableMouse();
	}

	void SpectatorCamera_c::Enable()
	{
		clCamera.Enable();
		clCameraCommandProducer.Enable();
	}

	void SpectatorCamera_c::Disable()
	{
		clCamera.Disable();
		clCameraCommandProducer.Disable();
	}
}