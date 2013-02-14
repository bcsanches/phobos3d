#ifndef PH_EDITOR_MODULE_H
#define PH_EDITOR_MODULE_H

#include <rapidjson/document.h>

#include <JsonCreator/StringWriterFwd.h>

#include <PH_CoreModule.h>
#include <PH_GamePlugin.h>
#include <PH_IClient.h>
#include <Phobos/Singleton.h>

#include "PH_NetworkService.h"

namespace Phobos
{
	namespace Editor
	{
		PH_DECLARE_NODE_PTR(EditorModule)

		class EditorModule: public Module, public IClient
		{			
			public:
				PH_GAME_PLUGIN_CREATE_MODULE_PROC_DECL

				PH_DECLARE_SINGLETON_METHODS(EditorModule);

			public:				
				virtual EscAction HandleEsc(Gui::Form *&outForm) override;
				virtual void SetPlayerCmd(IPlayerCmdPtr_t cmd) override;

				virtual void OnFixedUpdate() override;

			protected:
				virtual void OnBoot() override;				

			private:
				void ExecuteJsonCommand(const rapidjson::Value &obj, JsonCreator::StringWriter &response);

				EditorModule();

			private:
				NetworkService m_clNetworkService;
		};
	}
}

#endif
