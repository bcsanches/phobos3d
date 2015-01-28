#include "Phobos/Engine/ModuleManager.h"

#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>

#include "Phobos/Engine/Module.h"

#include "Phobos/Engine/ModuleFactory.h"

namespace
{
	using namespace Phobos;

	static String_t FixLibName(const String_t &libName)
	{
#ifdef PH_DEBUG
		return libName + "_d";
#else
		return libName;
#endif
	}
}

namespace Phobos
{
	namespace Engine
	{
		ModuleManager::ModuleManager():
			m_rclModulesHive(Register::GetOrCreateHive("ModuleDef"))
		{
			//empty
		}

		ModuleInfo ModuleManager::CreateModule(const Register::Table &table)
		{
			String_t libName = FixLibName(table.GetString("lib"));

			auto it = m_mapLibs.find(libName);
			if (it == m_mapLibs.end())
			{
				DynamicLibrary lib;
				lib.Load(libName);

				it = m_mapLibs.insert(std::make_pair(libName, std::move(lib))).first;
			}

			auto &lib = it->second;

			ModuleFactoryCreateModuleProc_t createProc = static_cast<ModuleFactoryCreateModuleProc_t>(lib.GetSymbol("PH_ModuleFactory_Create"));

			std::unique_ptr<Module> module;

			createProc(module, table.GetName());

			return ModuleInfo(std::move(module), table.GetInt("priority"));
		}

		ModuleInfo ModuleManager::CreateModule(const String_t &name)
		{
			const auto &table = m_rclModulesHive.GetTable(name);

			return this->CreateModule(table);
		}

		std::vector<ModuleInfo> ModuleManager::CreateAllModules(std::initializer_list<const char *> list)
		{
			std::vector<ModuleInfo> vecModules;

			for (auto it : m_rclModulesHive)
			{
				//if on ignore list, ignore  it :)
				if (std::find_if(list.begin(), list.end(), [&it](const char *value) { return it.first.compare(value) == 0; }) != list.end())
					continue;

				auto &table = *static_cast<const Register::Table *>(it.second);

				vecModules.push_back(this->CreateModule(table));
			}

			return vecModules;
		}
	}
}
