#ifndef PH_MODULE_MANAGER_H
#define PH_MODULE_MANAGER_H

#include <map>

#include <Phobos/DisableCopy.h>
#include <Phobos/DynamicLibrary.h>
#include <Phobos/Register/HiveFwd.h>

namespace Phobos
{
	namespace Engine
	{
		class Module;

		class ModuleManager
		{
			PH_DISABLE_COPY(ModuleManager);

			public:
				ModuleManager();

				std::unique_ptr<Module> CreateModule(StringRef_t name);

			private:
				std::map<String_t, DynamicLibrary>	m_mapLibs;

				Register::Hive						&m_rclModulesHive;
		};
	}
}

#endif
