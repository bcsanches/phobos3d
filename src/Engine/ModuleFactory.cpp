#include "Phobos/Engine/ModuleFactory.h"

namespace Phobos
{
	namespace Engine
	{		
		ModuleFactory &ModuleFactory::GetInstance()
		{
			static ModuleFactory gModule;

			return gModule;
		}
	}
}


#ifdef PH_WIN32
#define PH_MODULE_FACTORY_EXPORT __declspec(dllexport)
#elif defined PH_LINUX
#define PH_MODULE_FACTORY_EXPORT
#endif

extern "C" PH_MODULE_FACTORY_EXPORT void PH_ModuleFactory_Create(std::unique_ptr<Phobos::Engine::Module> &out, const Phobos::String_t &name)
{
	out = Phobos::Engine::ModuleFactory::GetInstance().Create(name, name);
}
