/*
Phobos 3d
  October 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#ifndef PH_CLIENT_H
#define PH_CLIENT_H

#include <PH_ContextCmd.h>
#include <PH_ContextVar.h>
#include <PH_CoreModule.h>
#include <PH_Singleton.h>
#include <PH_WorldManager.h>

#include "PH_SpectatorCamera.h"

namespace Phobos
{
	PH_DECLARE_NODE_PTR(Client);

	class Client_c: public CoreModule_c, private WorldManagerListener_c
	{
		PH_DECLARE_SINGLETON_METHODS(Client);

		public:

		protected:
			void OnFixedUpdate();
			void OnUpdate();
			void OnRenderReady();
			void OnPrepareToBoot();

		private:
			Client_c();

			void OnMapUnloaded();
			void OnMapLoaded();

			void ClipMouseCursor();
			void UnclipMouseCursor();
			
			void CmdToggleMouseCursorClip(const StringVector_t &args, Context_c &);
			void CmdNullMouseThumb(const StringVector_t &args, Context_c &);

			void VarSpectatorMoveSpeedChanged(const class ContextVar_c &var, const String_c &oldValue, const String_c &newValue);
			void VarSpectatorTurnSpeedChanged(const class ContextVar_c &var, const String_c &oldValue, const String_c &newValue);
			void VarMouseSensitivityChanged(const class ContextVar_c &var, const String_c &oldValue, const String_c &newValue);

			struct ConfigInfo_s GetConfig();

		private:					
			ContextCmd_c		cmdToggleMouseCursorClip;
			ContextCmd_c		cmdNullMouseThumb;
			ContextVar_c		varMouseSensitivity;

			ContextVar_c		varSpectatorMoveSpeed;
			ContextVar_c		varSpectatorTurnSpeed;

			SpectatorCamera_c	clSpectatorCamera;

			bool fMouseClipped;
			bool fMapLoaded;
	};
}

#endif
