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

#include "Phobos/Engine/PluginInstance.h"

#include <Phobos/Memory.h>

Phobos::Engine::PluginInstancePtr_t Phobos::Engine::PluginInstance::Create(const String_t &name)
{
	return PluginInstancePtr_t(PH_NEW PluginInstance(name));
}

Phobos::Engine::PluginInstance::PluginInstance(const String_t &name):
	Node(name)
{
	m_clLibrary.Load(name);

	PluginEntryPointProc_t proc = reinterpret_cast<PluginEntryPointProc_t>(m_clLibrary.GetSymbol("PH_PluginEntryPoint"));

	m_pclPlugin = proc();
}

void Phobos::Engine::PluginInstance::Init()
{
	if(m_pclPlugin != NULL)
		m_pclPlugin->Init();
}

Phobos::Engine::PluginInstance::~PluginInstance()
{
	if(m_pclPlugin != NULL)
		m_pclPlugin->Finalize();
}

