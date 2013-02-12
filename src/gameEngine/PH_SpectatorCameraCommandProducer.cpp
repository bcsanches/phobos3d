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

#include "PH_SpectatorCameraCommandProducer.h"

#include <PH_Console.h>
#include <Phobos/Shell/Utils.h>
#include <PH_Core.h>
#include <PH_CoreTimer.h>
#include <Phobos/Exception.h>
#include <Phobos/Memory.h>

namespace Phobos
{
	SpectatorCameraCommandProducer_c::SpectatorCameraCommandProducer_c(Shell::Context *context):
		m_clMoveButton("-moveBwd", "+moveBwd", "=moveBwd", "-moveFwd", "+moveFwd", "=moveFwd", context),
		m_clMoveUpDown("-moveDown", "+moveDown", "=moveDown", "-moveUp", "+moveUp", "=moveUp", context),
		m_clStrafeButton("-strafeRight", "+strafeRight", "=strafeRight", "-strafeLeft", "+strafeLeft", "=strafeLeft", context),
		m_clTurnButton( "-turnRight", "+turnRight", "=turnRight", "-turnLeft", "+turnLeft", "=turnLeft", context),
		m_clLookButton("-lookUp", "+lookUp", "=lookUp", "-lookDown", "+lookDown", "=lookDown", context),
		m_clMouseThumb(PH_MOUSE_THUMB_CMD_NAME, context),
		m_fpMoveSpeed(8),
		m_fpTurnSpeed(70),
		m_fpMouseSensitivity(0.3f),		
		m_fMouseActive(false)
	{		
		this->Disable();
	}

	IPlayerCmdPtr_t SpectatorCameraCommandProducer_c::CreateCmd()
	{
		const Float_t ticks = Core::GetInstance().GetSimInfo().m_stTimers[CORE_SYS_TIMER].m_fpFrameTime;

		const Float_t fwd = m_clMoveButton.GetValue() * ticks * m_fpMoveSpeed;
		const Float_t strafe = m_clStrafeButton.GetValue() * ticks * m_fpMoveSpeed;
		const Float_t upDown = m_clMoveUpDown.GetValue() * ticks * m_fpMoveSpeed;

		const Float_t *thumb = m_clMouseThumb.GetPoint();

		const Float_t turnAngle(((m_clTurnButton.GetValue() * m_fpTurnSpeed) + (-thumb[0] * m_fpTurnSpeed * m_fpMouseSensitivity)) * ticks);
		const Float_t lookAngle(((m_clLookButton.GetValue() * m_fpTurnSpeed) + (thumb[1] * m_fpTurnSpeed * m_fpMouseSensitivity)) * ticks);

		return IPlayerCmdPtr_t(PH_NEW SpectatorCameraCmd(
			fwd,
			strafe,
			upDown,
			turnAngle,
			lookAngle
		));
	}

	void SpectatorCameraCommandProducer_c::Enable()
	{
		Console &console = Console::GetInstance();

		m_clMoveButton.Enable(console);
		m_clStrafeButton.Enable(console);
		m_clMoveUpDown.Enable(console);
		m_clTurnButton.Enable(console);
		m_clLookButton.Enable(console);	
		m_clMouseThumb.Enable(console);
	}

	void SpectatorCameraCommandProducer_c::Disable()
	{
		m_clMoveButton.Disable();
		m_clStrafeButton.Disable();
		m_clMoveUpDown.Disable();
		m_clTurnButton.Disable();
		m_clLookButton.Disable();		
		m_clMouseThumb.Disable();
	}

	bool SpectatorCameraCommandProducer_c::IsMouseClipped() const
	{
		return true;
	}
	
	void SpectatorCameraCommandProducer_c::SetMoveSpeed(Float_t v)
	{
		m_fpMoveSpeed = v;
	}

	void SpectatorCameraCommandProducer_c::SetTurnSpeed(Float_t v)
	{
		m_fpTurnSpeed = v;
	}

	void SpectatorCameraCommandProducer_c::SetMouseSensitivity(Float_t v)
	{
		if(v <= 0)
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "SpectatorCameraCommandProducer_c::SetMouseSensitivity", "Mouse sensitivity must be greater than zero");

		m_fpMouseSensitivity = v;
	}
}
