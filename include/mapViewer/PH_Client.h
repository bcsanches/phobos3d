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


#ifndef PH_CLIENT_H
#define PH_CLIENT_H

#include <Phobos/Shell/Command.h>
#include <Phobos/Shell/Variable.h>
#include <PH_GamePlugin.h>
#include <Phobos/Singleton.h>
#include <PH_WorldManager.h>

#include "PH_BaseClient.h"
#include "PH_SpectatorCamera.h"

namespace Phobos
{
	PH_DECLARE_NODE_PTR(Client);

	class Client: public BaseClient, private WorldManagerListener
	{
		PH_DECLARE_SINGLETON_METHODS(Client);

		public:
			PH_GAME_PLUGIN_CREATE_MODULE_PROC_DECL;

			virtual void SetPlayerCmd(IPlayerCmdPtr_t cmd) override;

			virtual EscAction HandleEsc(Gui::Form *&outForm) override;

		protected:
			virtual void OnFixedUpdate() override;
			virtual void OnUpdate() override;			
			virtual void OnPrepareToBoot() override;
			virtual void OnBoot() override;			

		private:
			Client();

			void OnMapUnloaded();
			void OnMapLoaded();
			
			void CmdToggleMouseCursorClip(const Shell::StringVector_t &args, Shell::Context &);			

			void VarSpectatorMoveSpeedChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue);
			void VarSpectatorTurnSpeedChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue);
			void VarMouseSensitivityChanged(const class Shell::Variable &var, const String_t &oldValue, const String_t &newValue);

			struct ConfigInfo_s GetConfig();

		private:								
			Shell::Variable		m_varMouseSensitivity;

			Shell::Variable		m_varSpectatorMoveSpeed;
			Shell::Variable		m_varSpectatorTurnSpeed;

			SpectatorCamera	m_clSpectatorCamera;
			SpectatorCameraCommandProducer_c m_clSpectatorCameraCommandProducer;

			IPlayerCmdPtr_t		m_ipPlayerCmd;
			
			bool m_fMapLoaded;
	};
}

#endif
