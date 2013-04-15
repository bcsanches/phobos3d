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


#include "PH_Client.h"

#include <Phobos/Engine/Console.h>

#include <Phobos/Shell/Utils.h>
#include <Phobos/Error.h>
#include <Phobos/Exception.h>

#include <PH_PointEntity.h>
#include <Phobos/Engine/Session.h>
#include <PH_WorldManager.h>

#include "Gui/PH_LevelSelector.h"

PH_GAME_PLUGIN_ENTRY_POINT("MapViewerPlugin", "game.cfg");

namespace Phobos
{
	PH_DEFINE_DEFAULT_SINGLETON(Client);

	PH_GAME_PLUGIN_REGISTER_MODULE(Client);

	PH_GAME_PLUGIN_CREATE_MODULE_PROC_IMPL(Client);

	Client::Client():
		BaseClient("Client"),		
		m_varMouseSensitivity("dvMouseSensitivity", "0.1"),
		m_varSpectatorMoveSpeed("dvSpectatorMoveSpeed", "2.0"),
		m_varSpectatorTurnSpeed("dvSpectatorTurnSpeed", "45.0"),		
		m_fMapLoaded(false)
	{
		m_varSpectatorMoveSpeed.SetCallback(PH_CONTEXT_VAR_BIND(&Client::VarSpectatorMoveSpeedChanged, this));
		m_varSpectatorTurnSpeed.SetCallback(PH_CONTEXT_VAR_BIND(&Client::VarSpectatorTurnSpeedChanged, this));
		m_varMouseSensitivity.SetCallback(PH_CONTEXT_VAR_BIND(&Client::VarMouseSensitivityChanged, this));

		m_clSpectatorCamera.Disable();		
	}

	void Client::SetPlayerCmd(Engine::IPlayerCmdPtr_t cmd)
	{
		m_ipPlayerCmd = cmd;
	}

	Phobos::Engine::EscAction Client::HandleEsc(Engine::Gui::Form *&outForm)
	{
		Gui::LevelSelector &levelSelector = Gui::LevelSelector::GetInstance();
		levelSelector.Open();

		outForm = &levelSelector;

		return Engine::EscAction::SET_GUI;
	}

	void Client::OnFixedUpdate()
	{
		if(!m_fMapLoaded)
			return;

		m_clSpectatorCamera.FixedUpdate(m_ipPlayerCmd);
	}

	void Client::OnUpdate()
	{
		if(!m_fMapLoaded)
			return;

		m_clSpectatorCamera.Update();
	}

	void Client::OnPrepareToBoot()
	{
		BaseClient::OnPrepareToBoot();

		auto &console = Engine::Console::GetInstance();		

		console.AddContextVariable(m_varMouseSensitivity);
		console.AddContextVariable(m_varSpectatorMoveSpeed);
		console.AddContextVariable(m_varSpectatorTurnSpeed);

		WorldManager::GetInstance().AddListener(*this);
	}	

	void Client::OnBoot()
	{
		BaseClient::OnBoot();		

		Gui::LevelSelector &levelSelector = Gui::LevelSelector::GetInstance();
		levelSelector.Open();

		auto &session = Engine::Session::GetInstance();
		session.SetGuiForm(&levelSelector);
		session.CloseConsole();
	}

	void Client::OnMapUnloaded()
	{
		if(m_fMapLoaded)
		{
			m_fMapLoaded = false;
			m_clSpectatorCamera.Disable();
		}
	}

	void Client::OnMapLoaded()
	{
		WorldManager &worldManager = WorldManager::GetInstance();
		PointEntity *player = static_cast<PointEntity *>(worldManager.TryGetEntityByType("InfoPlayerStart"));
		if(!player)
		{
			LogMessage("[CmdLoadMap] World does not contains InfoPlayerStart entity");
		}
		else
		{
			m_clSpectatorCamera.SetTransform(player->GetTransform());
		}

		Gui::LevelSelector::GetInstance().Close();

		m_fMapLoaded = true;
		m_clSpectatorCamera.Enable();
		Engine::Session::GetInstance().SetPlayerCommandProducer(&m_clSpectatorCameraCommandProducer);

		//make sure no gui is present
		Engine::Session::GetInstance().SetGuiForm(NULL);
	}	
	
	void Client::VarSpectatorMoveSpeedChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue)
	{
		m_clSpectatorCameraCommandProducer.SetMoveSpeed(std::stof(newValue));
	}

	void Client::VarSpectatorTurnSpeedChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue)
	{
		m_clSpectatorCameraCommandProducer.SetTurnSpeed(std::stof(newValue));
	}

	void Client::VarMouseSensitivityChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue)
	{
		m_clSpectatorCameraCommandProducer.SetMouseSensitivity(std::stof(newValue));
	}
}

