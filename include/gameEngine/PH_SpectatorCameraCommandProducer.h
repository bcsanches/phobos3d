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

#ifndef PH_SPECTATOR_CAMERA_COMMAND_PRODUCER_H
#define PH_SPECTATOR_CAMERA_COMMAND_PRODUCER_H

#include <PH_AxisButton.h>
#include <PH_Button.h>
#include <PH_ContextVar.h>
#include <PH_Thumb.h>
#include <PH_Types.h>

#include "PH_IPlayerCommandProducer.h"
#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class Context_c;
	class SpectatorCameraCmd_c;
	
	typedef boost::shared_ptr<SpectatorCameraCmd_c> SpectatorCameraCmdPtr_t;

	class PH_GAME_ENGINE_API SpectatorCameraCmd_c: public IPlayerCmd_c
	{
		public:
			inline SpectatorCameraCmd_c(Float_t move, Float_t strafe, Float_t moveUp, Float_t turn, Float_t look)
			{
				fpMove = move;
				fpMoveUp = moveUp;
				fpStrafe = strafe;
				fpTurn = turn;
				fpLook = look;			
			}		

			inline Float_t GetMove() const;
			inline Float_t GetMoveUp() const;
			inline Float_t GetStrafe() const;
			inline Float_t GetTurn() const;
			inline Float_t GetLook() const;

		private:
			Float_t fpMove;
			Float_t fpMoveUp;
			Float_t fpStrafe;
			Float_t fpTurn;
			Float_t fpLook;	
	};

	inline Float_t SpectatorCameraCmd_c::GetMove() const
	{
		return fpMove;
	}

	inline Float_t SpectatorCameraCmd_c::GetMoveUp() const
	{
		return fpMoveUp;
	}

	inline Float_t SpectatorCameraCmd_c::GetStrafe() const
	{
		return fpStrafe;
	}

	inline Float_t SpectatorCameraCmd_c::GetTurn() const
	{
		return fpTurn;
	}

	inline Float_t SpectatorCameraCmd_c::GetLook() const
	{
		return fpLook;
	}

	#define PH_MOUSE_THUMB_CMD_NAME "mouseThumb"

	class PH_GAME_ENGINE_API SpectatorCameraCommandProducer_c: public IPlayerCommandProducer_c
	{
		public:
			SpectatorCameraCommandProducer_c(Context_c *context = NULL);

			virtual IPlayerCmdPtr_t CreateCmd();

			virtual void Enable();
			virtual void Disable();

			virtual bool IsMouseClipped() const;	

			void SetMoveSpeed(Float_t v);
			void SetTurnSpeed(Float_t v);
			void SetMouseSensitivity(Float_t v);		

		private:
			AxisButton_c	clMoveButton;
			AxisButton_c	clMoveUpDown;
			AxisButton_c	clStrafeButton;
			AxisButton_c	clTurnButton;
			AxisButton_c	clLookButton;		
			Thumb_c			clMouseThumb;			

			Float_t			fpMoveSpeed;
			Float_t			fpTurnSpeed;
			Float_t			fpMouseSensitivity;

			bool			fMouseActive;
	};
}

#endif
