#ifndef PH_ENGINE_MODULE_FACTORY_H
#define PH_ENGINE_MODULE_FACTORY_H

#include <Phobos/GenericFactory.h>

#include "Phobos/Engine/Module.h"

namespace Phobos
{
	namespace Engine
	{
		class ModuleFactory : public Phobos::GenericFactoryEx < Module, std::unique_ptr<Module>, const Phobos::String_t & >
		{
			PH_DISABLE_COPY(ModuleFactory);

			public:
				static ModuleFactory &GetInstance();

			private:
				inline ModuleFactory() {};
		};

		typedef void (*ModuleFactoryCreateModuleProc_t)(std::unique_ptr<Phobos::Engine::Module> &out, const Phobos::String_t &name);
	}
}

#define PH_MODULE_CREATE_INSTANCE(TYPE)  														\
	std::unique_ptr<Phobos::Engine::Module> TYPE::CreateInstance(const Phobos::String_t &name)	\
{																								\
	return std::unique_ptr<Phobos::Engine::Module>(new TYPE(name));								\
}

#define PH_MODULE_CREATOR(NAME, PROC)											\
	static Phobos::Engine::ModuleFactory::ObjectCreator_t TYPE_##CreatorObject_gl(NAME, PROC, Phobos::Engine::ModuleFactory::GetInstance())	

#define PH_MODULE_FULL_CREATOR(NAME, TYPE)			\
	PH_MODULE_CREATE_INSTANCE(TYPE)					\
	PH_MODULE_CREATOR(NAME, TYPE::CreateInstance);


#endif