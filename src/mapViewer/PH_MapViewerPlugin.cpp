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
#include "PH_MapViewerPlugin.h"

#include <PH_Core.h>
#include <PH_CoreModuleManager.h>
#include <PH_Plugin.h>
#include <PH_ProcVector.h>

#include "PH_Client.h"

namespace Phobos
{
	class MapViewerPlugin_c: public IPluginInstance_c
	{
		public:
			void Init();
			void Finalize();

		private:
			ProcVector_c clProcs;
			CoreModuleManagerPtr_t ipManager;
	};

	void MapViewerPlugin_c::Init()
	{
		ipManager = CoreModuleManager_c::Create("MapViewer");

		Core_c::GetInstance()->AddModule(ipManager);

		ClientPtr_t client = Client_c::CreateInstance();
		clProcs.AddProc(Client_c::ReleaseInstance);
		ipManager->AddModule(client);		
	}

	void MapViewerPlugin_c::Finalize()
	{
		Core_c::GetInstance()->RemoveModule(*ipManager);		

		clProcs.CallAll();
		clProcs.Clear();

		ipManager.reset();
	}
}

Phobos::IPluginInstance_c *PH_PluginEntryPoint(void)
{
	using namespace Phobos;

	static MapViewerPlugin_c plugin;

	return &plugin;
}
