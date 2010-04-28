#ifndef PH_BOOT_MODULE_H
#define PH_BOOT_MODULE_H

#include "PH_CoreModule.h"

namespace Phobos
{
	class BootModule_c;

	typedef boost::intrusive_ptr<BootModule_c> BootModulePtr_t;

	class PH_ENGINE_CORE_API BootModule_c: public CoreModule_c
	{
		public:
			static BootModulePtr_t Create();

			void OnUpdate();
			void OnFixedUpdate();

		protected:
			BootModule_c();

		private:
			int		iFixedUpdateCount;
			bool	fUpdateDone;
	};
}


#endif