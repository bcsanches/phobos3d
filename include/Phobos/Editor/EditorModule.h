#ifndef PH_EDITOR_MODULE_H
#define PH_EDITOR_MODULE_H

#include <rapidjson/document.h>

#include <JsonCreator/StringWriterFwd.h>

#include <Phobos/Engine/Module.h>
#include <Phobos/Engine/Plugin.h>
#include <Phobos/Engine/IClient.h>
#include <Phobos/Game/MapDefs.h>
#include <Phobos/Game/WorldManager.h>
#include <Phobos/Singleton.h>

#include "Phobos/Editor/Camera.h"
#include "Phobos/Editor/EditObjectManager.h"
#include "Phobos/Editor/NetworkService.h"

namespace Phobos
{
	namespace Editor
	{
		PH_DECLARE_NODE_PTR(EditorModule)

		class EditorModule : public Engine::Module, public Engine::IClient, public Game::WorldManagerListener
		{			
			public:
				PH_PLUGIN_CREATE_MODULE_PROC_DECL

				PH_DECLARE_SINGLETON_METHODS(EditorModule);

			public:				
				virtual Engine::EscAction HandleEsc(Engine::Gui::Form *&outForm) override;
				virtual void SetPlayerCmd(Engine::IPlayerCmdPtr_t cmd) override;

				virtual void OnFixedUpdate() override;

				void CreateMapObject(const String_t &asset, Game::MapObjectTypes type);

			protected:
				virtual void OnBoot() override;				

			private:
				void ExecuteJsonCommand(const rapidjson::Value &obj, JsonCreator::StringWriter &response);

				virtual void OnMapUnloaded() override;
				virtual void OnMapLoaded() override;

				EditorModule();

			private:
				NetworkService		m_clNetworkService;
				EditObjectManager	m_clEditObjectManager;
				Camera				m_clCamera;
		};
	}
}

#endif
