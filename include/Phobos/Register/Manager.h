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
#ifndef PH_REGISTER_REGISTER_MANAGER_H
#define PH_REGISTER_REGISTER_MANAGER_H

#include <Phobos/Shell/Command.h>
#include <Phobos/Shell/IContext.h>

#include <Phobos/Node.h>

#include <istream>

#include "Phobos/Register/RegisterAPI.h"
#include "Phobos/Register/TableFwd.h"
#include "Phobos/Register/HiveFwd.h"

namespace Phobos
{	
	class Path;

	namespace Register
	{		
		PH_REGISTER_API void Init();
		PH_REGISTER_API void Finalize();

		PH_REGISTER_API void Load(StringRef_t fileName);
		PH_REGISTER_API void Load(std::istream &stream);

		PH_REGISTER_API void LoadAll(const String_t &path);

		PH_REGISTER_API Hive &CreateCustomHive(const String_t &name);			

		PH_REGISTER_API Hive &GetHive(const String_t &name);
		PH_REGISTER_API Hive *TryGetHive(const String_t &name);
		PH_REGISTER_API Table &GetTable(const String_t &hive, const String_t &table);
		PH_REGISTER_API Table *TryGetTable(const String_t &hive, const String_t &table);
		PH_REGISTER_API Table &GetTable(const Path &relativePath);

		PH_REGISTER_API void RegisterCommands(Shell::IContext &context);
	}
}

#endif
