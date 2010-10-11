/******************************************************************************
*
* COPYRIGHT BCSoftware LTDA 2009. ALL RIGHTS RESERVED.
*
* This software cannot be copied, stored, distributed without BCSoftware LTDA
* prior authorization.
*
******************************************************************************/

#include "PH_SpectatorCameraController.h"

#include <PH_Console.h>
#include <PH_Core.h>
#include <PH_CoreTimer.h>
#include <PH_Exception.h>

namespace Phobos
{
	SpectatorCameraController_c::SpectatorCameraController_c(Context_c *context):
		clMoveButton("-moveBwd", "+moveBwd", "=moveBwd", "-moveFwd", "+moveFwd", "=moveFwd", context),
		clStrafeButton("-strafeRight", "+strafeRight", "=strafeRight", "-strafeLeft", "+strafeLeft", "=strafeLeft", context),
		clTurnButton( "-turnRight", "+turnRight", "=turnRight", "-turnLeft", "+turnLeft", "=turnLeft", context),
		clLookButton("-lookUp", "+lookUp", "=lookUp", "-lookDown", "+lookDown", "=lookDown", context),
		clMoveUpDown("-moveDown", "+moveDown", "=moveDown", "-moveUp", "+moveUp", "=moveUp", context),	
		clMouseThumb(PH_MOUSE_THUMB_CMD_NAME, context),
		fpTurnSpeed(70),
		fpMoveSpeed(8),
		fpMouseSensitivity(0.3f)
	{
		clMouseThumb.Disable();
	}

	IPlayerCmdPtr_t SpectatorCameraController_c::CreateCmd()
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

	void SpectatorCameraController_c::EnableController()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		console->EnableAxisButton(clMoveButton);
		console->EnableAxisButton(clStrafeButton);
		console->EnableAxisButton(clMoveUpDown);
		console->EnableAxisButton(clTurnButton);
		console->EnableAxisButton(clLookButton);
	}

	void SpectatorCameraController_c::DisableController()
	{
		clMoveButton.Disable();
		clStrafeButton.Disable();
		clMoveUpDown.Disable();
		clTurnButton.Disable();
		clLookButton.Disable();	
	}

	void SpectatorCameraController_c::EnableMouse()
	{
		Console_c::GetInstance()->EnableThumb(clMouseThumb);		
	}

	void SpectatorCameraController_c::DisableMouse()
	{
		clMouseThumb.Disable();
	}

	void SpectatorCameraController_c::SetMoveSpeed(Float_t v)
	{
		fpMoveSpeed = v;
	}
	
	void SpectatorCameraController_c::SetTurnSpeed(Float_t v)
	{
		fpTurnSpeed = v;
	}
	
	void SpectatorCameraController_c::SetMouseSensitivity(Float_t v)
	{
		if(v <= 0)
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "SpectatorCameraController_c::SetMouseSensitivity", "Mouse sensitivity must be greater than zero");

		fpMouseSensitivity = v;
	}
}
