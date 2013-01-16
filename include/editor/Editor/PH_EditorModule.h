#ifndef PH_EDITOR_MODULE_H
#define PH_EDITOR_MODULE_H

#include <rapidjson/document.h>

#include <JsonCreator/StringWriterFwd.h>

#include <PH_CoreModule.h>
#include <PH_GamePlugin.h>
#include <PH_IClient.h>
#include <PH_Singleton.h>

#include "PH_NetworkService.h"

namespace Phobos
{
	namespace Editor
	{
		PH_DECLARE_NODE_PTR(EditorModule)

		class EditorModule_c: public CoreModule_c, public IClient_c
		{			
			public:
				PH_GAME_PLUGIN_CREATE_MODULE_PROC_DECL

				PH_DECLARE_SINGLETON_METHODS(EditorModule);

			public:				
				virtual EscAction::Enum HandleEsc(Gui::Form_c *&outForm);
				virtual void SetPlayerCmd(IPlayerCmdPtr_t cmd);

				virtual void OnFixedUpdate();

			protected:
				virtual void OnBoot();				

			private:
				void ExecuteJsonCommand(const rapidjson::Value &obj, JsonCreator::StringWriter &response);

				EditorModule_c();

			private:
				NetworkService_c clNetworkService;
		};
	}
}

#endif
