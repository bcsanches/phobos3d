/*
Phobos 3d
September 2011
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

#include <PH_ContextCmd.h>
#include <PH_ContextVar.h>
#include <PH_CoreModule.h>
#include <PH_MouseInputDevice.h>

#include "PH_GameEngineAPI.h"

namespace Phobos
{
	class PH_GAME_ENGINE_API BaseClient_c: public CoreModule_c
	{
		public:
			struct ConfigInfo_s
			{
				MouseInputDevicePtr_t ipMouse;
				void *pWindowHandler;
			};			

		protected:
			BaseClient_c(const char *name);

			virtual void OnPrepareToBoot();

			struct ConfigInfo_s GetConfig();

			void ClipMouseCursor();
			void UnclipMouseCursor();

			void RegisterToggleMouseCursorClipCmd();

			void RegisterNullMouseThumbCmd();
			void UnregisterNullMouseThumbCmd();

			bool IsMouseClipped();

			virtual void OnMouseClip() {};
			virtual void OnMouseUnclip() {};		

		private:
			void CmdToggleMouseCursorClip(const Phobos::StringVector_t &args, Phobos::Context_c &);
			void CmdNullMouseThumb(const Phobos::StringVector_t &args, Phobos::Context_c &);

		private:
			ContextCmd_c cmdToggleMouseCursorClip;
			ContextCmd_c cmdNullMouseThumb;

			ContextVar_c varMouseClipped;
	};
}