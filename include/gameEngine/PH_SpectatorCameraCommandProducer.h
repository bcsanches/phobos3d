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

#include <Phobos/Shell/ContextFwd.h>
#include <Phobos/Shell/Variable.h>
#include <Phobos/Types.h>

#include <Phobos/System/AxisButton.h>
#include <Phobos/System/Button.h>
#include <Phobos/System/Thumb.h>

#include <Phobos/Engine/IPlayerCommandProducer.h>

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class SpectatorCameraCmd;
	
	typedef std::shared_ptr<SpectatorCameraCmd> SpectatorCameraCmdPtr_t;

	class PH_GAME_ENGINE_API SpectatorCameraCmd: public Engine::IPlayerCmd
	{
		public:
			inline SpectatorCameraCmd(Float_t move, Float_t strafe, Float_t moveUp, Float_t turn, Float_t look)
			{
				m_fpMove = move;
				m_fpMoveUp = moveUp;
				m_fpStrafe = strafe;
				m_fpTurn = turn;
				m_fpLook = look;			
			}		

			inline Float_t GetMove() const;
			inline Float_t GetMoveUp() const;
			inline Float_t GetStrafe() const;
			inline Float_t GetTurn() const;
			inline Float_t GetLook() const;

		private:
			Float_t m_fpMove;
			Float_t m_fpMoveUp;
			Float_t m_fpStrafe;
			Float_t m_fpTurn;
			Float_t m_fpLook;	
	};

	inline Float_t SpectatorCameraCmd::GetMove() const
	{
		return m_fpMove;
	}

	inline Float_t SpectatorCameraCmd::GetMoveUp() const
	{
		return m_fpMoveUp;
	}

	inline Float_t SpectatorCameraCmd::GetStrafe() const
	{
		return m_fpStrafe;
	}

	inline Float_t SpectatorCameraCmd::GetTurn() const
	{
		return m_fpTurn;
	}

	inline Float_t SpectatorCameraCmd::GetLook() const
	{
		return m_fpLook;
	}

	#define PH_MOUSE_THUMB_CMD_NAME "mouseThumb"

	class PH_GAME_ENGINE_API SpectatorCameraCommandProducer_c: public Engine::IPlayerCommandProducer
	{
		public:
			SpectatorCameraCommandProducer_c(Shell::Context *context = NULL);

			virtual Engine::IPlayerCmdPtr_t CreateCmd();

			virtual void Enable();
			virtual void Disable();

			virtual bool IsMouseClipped() const;	

			void SetMoveSpeed(Float_t v);
			void SetTurnSpeed(Float_t v);
			void SetMouseSensitivity(Float_t v);		

		private:
			System::AxisButton	m_clMoveButton;
			System::AxisButton	m_clMoveUpDown;
			System::AxisButton	m_clStrafeButton;
			System::AxisButton	m_clTurnButton;
			System::AxisButton	m_clLookButton;		
			System::Thumb		m_clMouseThumb;			

			Float_t			m_fpMoveSpeed;
			Float_t			m_fpTurnSpeed;
			Float_t			m_fpMouseSensitivity;

			bool			m_fMouseActive;
	};
}

#endif
