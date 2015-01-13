#include "Phobos/Engine/ModuleManager.h"

#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Engine/Module.h"

namespace Phobos
{
	namespace Engine
	{
		ModuleManager::ModuleManager():
			m_rclModulesHive(Register::GetOrCreateHive("ModulesDef"))
		{
			//empty
		}

		std::unique_ptr<Module> ModuleManager::CreateModule(StringRef_t name)
		{
			const auto &table = m_rclModulesHive.GetTable(name.to_string());

			const String_t &libName = table.GetString("lib");

			auto it = m_mapLibs.find(libName);
			if (it == m_mapLibs.end())
			{
				DynamicLibrary lib;
				lib.Load(libName);

				it = m_mapLibs.insert(std::make_pair(libName, std::move(lib))).first;
			}

			auto &lib = it->second;

			return std::unique_ptr<Module>();
		}
	}
}
