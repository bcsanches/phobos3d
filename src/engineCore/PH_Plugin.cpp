/*
Phobos 3d
September 2010
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

#include "PH_Plugin.h"

namespace Phobos
{
	PluginPtr_t Plugin_c::Create(const String_c &name)
	{
		return PluginPtr_t(new Plugin_c(name));
	}

	Plugin_c::Plugin_c(const String_c &name):
		Node_c(name)
	{
		clLibrary.Load(name);

		PluginEntryPointProc_t proc = reinterpret_cast<PluginEntryPointProc_t>(clLibrary.GetSymbol("PH_PluginEntryPoint"));

		pclPlugin = proc();
	}

	void Plugin_c::Init()
	{
		if(pclPlugin != NULL)
			pclPlugin->Init();
	}

	Plugin_c::~Plugin_c()
	{
		if(pclPlugin != NULL)
			pclPlugin->Finalize();
	}
}
