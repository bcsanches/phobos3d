/*
Phobos 3d
April 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_BOOT_MODULE_H
#define PH_BOOT_MODULE_H

#include "PH_CoreModule.h"

#include <vector>
#include <string>

namespace Phobos
{
	class BootModule;
	class CoreModuleManager;

	PH_DECLARE_NODE_PTR(BootModule);	

	class BootModule: public CoreModule
	{
		public:			
			BootModule(const String_t &cfgName, int argc, char *const argv[], CoreModuleManager &manager);

			void OnUpdate();			
			void OnFixedUpdate();

		private:
			String_t m_strCfgName;
			std::vector<std::string> m_vecArgs;

			int		m_iFixedUpdateCount;
			bool	m_fUpdateDone;
			bool	m_fPrepareFired;
			bool	m_fBootFired;

			CoreModuleManager &m_rclManager;
	};
}


#endif