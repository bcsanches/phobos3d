#ifndef PH_MODULE_MANAGER_H
#define PH_MODULE_MANAGER_H

#include <map>
#include <vector>

#include <Phobos/DisableCopy.h>
#include <Phobos/DynamicLibrary.h>
#include <Phobos/Register/HiveFwd.h>
#include <Phobos/Register/TableFwd.h>

namespace Phobos
{
	namespace Engine
	{
		class Module;

		template <typename T>
		struct GenericModuleInfo
		{
			typedef std::unique_ptr<T> UniquePtr_t;

			inline GenericModuleInfo(UniquePtr_t &&module, unsigned int priority):
				m_upModule(std::move(module)),
				m_uPriority(priority)
			{
				//empty
			}

			inline GenericModuleInfo(GenericModuleInfo<T> &&info) :
				m_upModule(std::move(info.m_upModule)),
				m_uPriority(info.m_uPriority)
			{
				//empty
			}

			template <typename Y>
			inline GenericModuleInfo(GenericModuleInfo<Y> &&info) :
				m_upModule(StaticUniquePtrCast<T>(std::move(info.m_upModule))),
				m_uPriority(info.m_uPriority)
			{
				//empty
			}

			GenericModuleInfo(const GenericModuleInfo &) = delete;
			GenericModuleInfo &operator=(const GenericModuleInfo &) = delete;

			std::unique_ptr<T>	m_upModule;
			unsigned int		m_uPriority;
		};

		typedef GenericModuleInfo<Module> ModuleInfo;
		typedef std::unique_ptr<Module>(*CreateModuleProc_t)(const Phobos::String_t &name);

		struct LocalModuleClass
		{
			CreateModuleProc_t	m_pfnCreateProc;
			unsigned int		m_uPriority;			
		};

		typedef std::tuple<String_t, CreateModuleProc_t, unsigned int> ModuleInfoTuple_t;

		inline ModuleInfoTuple_t MakeLocalModuleClass(const String_t &name, CreateModuleProc_t proc, unsigned int priority)
		{
			return std::make_tuple(name, proc, priority);
		}

		class ModuleManager
		{
			PH_DISABLE_MOVE_COPY(ModuleManager);

			public:				
				typedef std::initializer_list <ModuleInfoTuple_t> InitializerList_t;

			public:
				ModuleManager(InitializerList_t ignoreList = {});

				ModuleInfo CreateModule(const String_t &name);

				template <typename T>
				GenericModuleInfo<T> CreateModuleEx(const String_t &name)
				{
					auto info = this->CreateModule(name);

					return GenericModuleInfo<T>(std::move(info));
				}

				std::vector<ModuleInfo> CreateAllModules(std::initializer_list<const char *> list);

			private:
				ModuleInfo CreateModule(const Register::Table &table);

			private:
				std::map<String_t, DynamicLibrary>		m_mapLibs;
				std::map<String_t, LocalModuleClass>	m_mapLocalModules;

				const Register::Hive				&m_rclModulesHive;
		};
	}
}

#endif
