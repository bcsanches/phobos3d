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

#include "Phobos/Game/SpectatorCamera.h"

#include <Phobos/Engine/Clocks.h>

namespace Phobos
{
	namespace Game
	{
		SpectatorCamera::SpectatorCamera()
		{
			m_clCamera.SetNearClipDistance(0.1f);		
			m_clCamera.SetFarClipDistance(100);
		
			m_clCamera.EnableViewport();		
		}

		void SpectatorCamera::FixedUpdate(Engine::IPlayerCmdPtr_t playerCmd)
		{
			auto cmd = std::static_pointer_cast<SpectatorCameraCmd>(playerCmd);

			using namespace Ogre;

			Degree turnAngle(cmd->GetTurn());
			Degree lookAngle(cmd->GetLook());

			Vector3 axis[3];
		
			const Quaternion &q = m_clCurrentTransform.GetRotation();

			q.ToAxes(axis[0], axis[1], axis[2]);	

			axis[2] *= cmd->GetMove();
			axis[0] *= cmd->GetStrafe();
			axis[1] = (Vector3::UNIT_Y * cmd->GetMoveUp());

			Vector3 movement = axis[0] + axis[1] + axis[2];
			m_clCurrentTransform.Translate(movement);	

			m_dTurn += turnAngle;
			m_dLook += lookAngle;

			Quaternion rot(m_dTurn, Vector3::UNIT_Y);
			rot = rot * Quaternion(m_dLook, Vector3::UNIT_X);	

			m_clCurrentTransform.SetRotation(rot);

			m_clTransformInterpolator.SetValue(m_clCurrentTransform);
		}

		void SpectatorCamera::Update()
		{
			m_clCamera.SetTransform(m_clTransformInterpolator.GetValue(Engine::RenderClock::GetDelta()));
		}

		void SpectatorCamera::SetTransform(const Engine::Math::Transform &transform)
		{
			m_clCurrentTransform = transform;

			m_dTurn = transform.GetRotation().getYaw().valueDegrees();
			m_dLook = transform.GetRotation().getPitch().valueDegrees();
		}
	
		void SpectatorCamera::Enable()
		{
			m_clCamera.Enable();		
		}

		void SpectatorCamera::Disable()
		{
			m_clCamera.Disable();		
		}
	}
}