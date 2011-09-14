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
#include <PH_ContextUtils.h>
#include <PH_Core.h>
#include <PH_CoreTimer.h>
#include <PH_Exception.h>

namespace Phobos
{
	SpectatorCameraCommandProducer_c::SpectatorCameraCommandProducer_c(Context_c *context):
		clMoveButton("-moveBwd", "+moveBwd", "=moveBwd", "-moveFwd", "+moveFwd", "=moveFwd", context),
		clMoveUpDown("-moveDown", "+moveDown", "=moveDown", "-moveUp", "+moveUp", "=moveUp", context),
		clStrafeButton("-strafeRight", "+strafeRight", "=strafeRight", "-strafeLeft", "+strafeLeft", "=strafeLeft", context),
		clTurnButton( "-turnRight", "+turnRight", "=turnRight", "-turnLeft", "+turnLeft", "=turnLeft", context),
		clLookButton("-lookUp", "+lookUp", "=lookUp", "-lookDown", "+lookDown", "=lookDown", context),
		clMouseThumb(PH_MOUSE_THUMB_CMD_NAME, context),
		fpMoveSpeed(8),
		fpTurnSpeed(70),
		fpMouseSensitivity(0.3f),
		cmdNullMouseThumb(PH_MOUSE_THUMB_CMD_NAME),
		fMouseActive(false)
	{
		cmdNullMouseThumb.SetProc(PH_CONTEXT_CMD_BIND(&SpectatorCameraCommandProducer_c::CmdNullMouseThumb, this));
		clMouseThumb.Disable();
	}

	IPlayerCmdPtr_t SpectatorCameraCommandProducer_c::CreateCmd()
	{
		const Float_t ticks = Core_c::GetInstance()->GetSimInfo().stTimers[CORE_SYS_TIMER].fpFrameTime;

		const Float_t fwd = clMoveButton.GetValue() * ticks * fpMoveSpeed;
		const Float_t strafe = clStrafeButton.GetValue() * ticks * fpMoveSpeed;
		const Float_t upDown = clMoveUpDown.GetValue() * ticks * fpMoveSpeed;

		const Float_t *thumb = clMouseThumb.GetPoint();

		const Float_t turnAngle(((clTurnButton.GetValue() * fpTurnSpeed) + (-thumb[0] * fpTurnSpeed * fpMouseSensitivity)) * ticks);
		const Float_t lookAngle(((clLookButton.GetValue() * fpTurnSpeed) + (thumb[1] * fpTurnSpeed * fpMouseSensitivity)) * ticks);

		return IPlayerCmdPtr_t(new SpectatorCameraCmd_c(
			fwd,
			strafe,
			upDown,
			turnAngle,
			lookAngle
			));
	}

	void SpectatorCameraCommandProducer_c::Enable()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		clMoveButton.Enable(*console);
		clStrafeButton.Enable(*console);
		clMoveUpDown.Enable(*console);
		clTurnButton.Enable(*console);
		clLookButton.Enable(*console);
		
		ContextVar_c *var = console->TryGetContextVar(PH_PLAYER_CMD_MOUSE_CLIPPED_VAR);
		if(var != NULL)
		{
			if(var->GetBoolean())
				this->EnableMouse();

			var->AddListener(*this);
		}
	}

	void SpectatorCameraCommandProducer_c::Disable()
	{
		clMoveButton.Disable();
		clStrafeButton.Disable();
		clMoveUpDown.Disable();
		clTurnButton.Disable();
		clLookButton.Disable();		

		ContextVar_c *var = Console_c::GetInstance()->TryGetContextVar(PH_PLAYER_CMD_MOUSE_CLIPPED_VAR);
		if (var != NULL)
			var->RemoveListener(*this);

		this->DisableMouse();
		cmdNullMouseThumb.Unlink();
	}

	void SpectatorCameraCommandProducer_c::OnVariableValueChanged(const ContextVar_c &var)
	{
		if(var.GetBoolean())
			this->EnableMouse();		
		else
			this->DisableMouse();
	}

	void SpectatorCameraCommandProducer_c::DisableMouse()
	{
		if(!fMouseActive)
			return;

		clMouseThumb.Disable();
		Console_c::GetInstance()->AddContextCmd(cmdNullMouseThumb);

		fMouseActive = false;
	}

	void SpectatorCameraCommandProducer_c::EnableMouse()
	{
		if(fMouseActive)
			return;

		cmdNullMouseThumb.Unlink();
		clMouseThumb.Enable(*Console_c::GetInstance());

		fMouseActive = true;
	}

	void SpectatorCameraCommandProducer_c::SetMoveSpeed(Float_t v)
	{
		fpMoveSpeed = v;
	}

	void SpectatorCameraCommandProducer_c::SetTurnSpeed(Float_t v)
	{
		fpTurnSpeed = v;
	}

	void SpectatorCameraCommandProducer_c::SetMouseSensitivity(Float_t v)
	{
		if(v <= 0)
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "SpectatorCameraCommandProducer_c::SetMouseSensitivity", "Mouse sensitivity must be greater than zero");

		fpMouseSensitivity = v;
	}

	void SpectatorCameraCommandProducer_c::CmdNullMouseThumb(const StringVector_t &args, Context_c &)
	{
		//empty
	}
}