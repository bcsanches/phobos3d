/*
Phobos 3d
September 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_REGISTER_HIVE_H
#define PH_REGISTER_HIVE_H

#include <PH_Node.h>

#include "Phobos/Register/RegisterAPI.h"
#include "Phobos/Register/TableFwd.h"

namespace Phobos
{
	class Parser_c;

	namespace Register
	{
		PH_DECLARE_NODE_PTR(Hive);

		class PH_REGISTER_API Hive_c: public Node_c
		{
			public:
				static HivePtr_t Create(const String_c &name);

				Hive_c(const String_c &name);
				~Hive_c();

				void Load(Parser_c &parser);

				Table_c &GetTable(const String_c &name);
				Table_c *TryGetTable(const String_c &name);

				void AddTable(std::unique_ptr<Table_c> &&dict);

	
			private:			
				UInt_t	uSequence;
		};
	}
}

#endif