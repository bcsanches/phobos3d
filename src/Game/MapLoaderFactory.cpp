/*
Phobos 3d
April 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "Phobos/Game/MapLoaderFactory.h"

#include <Phobos/Register/Hive.h>
#include <Phobos/Register/Manager.h>
#include <Phobos/Register/Table.h>

namespace Phobos
{
	namespace Game
	{
		MapLoaderFactory &MapLoaderFactory::GetInstance()
		{
			static MapLoaderFactory clInstance_gl;

			return clInstance_gl;
		}	

		MapLoaderFactory::MapLoaderFactory()
		{
			//empty
		}

		MapLoaderPtr_t MapLoaderFactory::Create(const String_t &type)
		{
			auto &table = Register::GetTable("MapLoader", type);

			return MapLoaderPtr_t(m_clFactory.Create(table.GetString("loader"), table));
		}

		std::list<String_t> MapLoaderFactory::CreateMapFileExtensionsList() const
		{
			const auto &hive = Register::GetHive("MapLoader");

			std::list<String_t> extensions;		

			for(auto it : hive)		
			{
				extensions.push_back(it.second->GetName());
			}

			return extensions;
		}
	}
}
