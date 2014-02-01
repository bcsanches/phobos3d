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


#include "ViewerClient.h"

#include <Phobos/Engine/Console.h>

#include <Phobos/Shell/Utils.h>
#include <Phobos/Error.h>
#include <Phobos/Exception.h>

#include <Phobos/Game/Things/PointEntity.h>
#include <Phobos/Engine/Session.h>
#include <Phobos/Game/WorldManager.h>

#include <Phobos/Game/Gui/LevelSelector.h>

PH_PLUGIN_ENTRY_POINT("MapViewerPlugin", "game.cfg");

namespace
{
	static Phobos::ViewerClient g_clViewerClient;
}

namespace Phobos
{	
	ViewerClient::ViewerClient():		
		m_varMouseSensitivity("dvMouseSensitivity", "0.1"),
		m_varSpectatorMoveSpeed("dvSpectatorMoveSpeed", "2.0"),
		m_varSpectatorTurnSpeed("dvSpectatorTurnSpeed", "45.0"),		
		m_fMapLoaded(false)
	{
		m_varSpectatorMoveSpeed.SetCallback(PH_CONTEXT_VAR_BIND(&ViewerClient::VarSpectatorMoveSpeedChanged, this));
		m_varSpectatorTurnSpeed.SetCallback(PH_CONTEXT_VAR_BIND(&ViewerClient::VarSpectatorTurnSpeedChanged, this));
		m_varMouseSensitivity.SetCallback(PH_CONTEXT_VAR_BIND(&ViewerClient::VarMouseSensitivityChanged, this));

		m_clSpectatorCamera.Disable();	

		auto &console = Engine::Console::GetInstance();

		console.AddContextVariable(m_varMouseSensitivity);
		console.AddContextVariable(m_varSpectatorMoveSpeed);
		console.AddContextVariable(m_varSpectatorTurnSpeed);		

		Engine::Session::GetInstance().SetClient(this);

		auto &levelSelector = Game::Gui::LevelSelector::GetInstance();
		levelSelector.Open();

		auto &session = Engine::Session::GetInstance();
		session.SetGuiForm(&levelSelector);
		session.CloseConsole();
	}

	void ViewerClient::OnConnect()
	{
		Game::WorldManager::GetInstance().AddListener(*this);
	}

	void ViewerClient::OnDisconnect()
	{
		this->DisableSpectatorCamera();

		Game::WorldManager::GetInstance().RemoveListener(*this);

		//As we loose listener, we forget about map status
		m_fMapLoaded = false;		

		//make sure no gui is present
		Game::Gui::LevelSelector::GetInstance().Close();
		Engine::Session::GetInstance().SetGuiForm(nullptr);
	}

	void ViewerClient::EnableSpectatorCamera()
	{
		if (m_fMapLoaded)
		{
			m_clSpectatorCamera.Enable();
			Engine::Session::GetInstance().SetPlayerCommandProducer(&m_clSpectatorCameraCommandProducer);
		}
	}

	void ViewerClient::DisableSpectatorCamera()
	{
		if (m_fMapLoaded)
		{
			m_clSpectatorCamera.Disable();
		}
	}

	void ViewerClient::DispatchCommand(Engine::IPlayerCmdPtr_t cmd)
	{
		m_ipPlayerCmd = cmd;

		m_clSpectatorCamera.FixedUpdate(m_ipPlayerCmd);
	}

	Phobos::Engine::EscAction ViewerClient::HandleEsc(Engine::Gui::Form *&outForm)
	{
		auto &levelSelector = Game::Gui::LevelSelector::GetInstance();
		levelSelector.Open();

		outForm = &levelSelector;

		return Engine::EscAction::SET_GUI;
	}

	void ViewerClient::OnUpdate()
	{
		if(!m_fMapLoaded)
			return;

		m_clSpectatorCamera.Update();
	}

	void ViewerClient::OnMapUnloaded()
	{
		if(m_fMapLoaded)
		{
			m_fMapLoaded = false;
			m_clSpectatorCamera.Disable();
		}
	}

	void ViewerClient::OnMapLoaded()
	{
		auto &worldManager = Game::WorldManager::GetInstance();
		auto *player = static_cast<Game::Things::PointEntity *>(worldManager.TryGetEntityByType("InfoPlayerStart"));
		if(!player)
		{
			LogMessage("[CmdLoadMap] World does not contains InfoPlayerStart entity");
		}
		else
		{
			m_clSpectatorCamera.SetTransform(player->GetTransform());
		}		

		m_fMapLoaded = true;
		
		this->EnableSpectatorCamera();

		Game::Gui::LevelSelector::GetInstance().Close();

		//make sure no gui is present
		Engine::Session::GetInstance().SetGuiForm(nullptr);
	}	
	
	void ViewerClient::VarSpectatorMoveSpeedChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue)
	{
		m_clSpectatorCameraCommandProducer.SetMoveSpeed(std::stof(newValue));
	}

	void ViewerClient::VarSpectatorTurnSpeedChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue)
	{
		m_clSpectatorCameraCommandProducer.SetTurnSpeed(std::stof(newValue));
	}

	void ViewerClient::VarMouseSensitivityChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue)
	{
		m_clSpectatorCameraCommandProducer.SetMouseSensitivity(std::stof(newValue));
	}
}

