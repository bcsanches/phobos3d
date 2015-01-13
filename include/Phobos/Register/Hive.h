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

#include <Phobos/DisableCopy.h>
#include <Phobos/Node.h>

#include "Phobos/Register/RegisterAPI.h"
#include "Phobos/Register/TableFwd.h"

namespace Phobos
{
	class Parser;

	namespace Register
	{
		PH_DECLARE_NODE_PTR(Hive);

		class PH_REGISTER_API Hive: public Node
		{
			PH_DISABLE_COPY(Hive);

			public:				
				Hive(const String_t &name);
				~Hive();

				void Load(Parser &parser);

				Table &GetTable(const String_t &name);
				Table *TryGetTable(const String_t &name);

				void AddTable(std::unique_ptr<Table> &&dict);

	
			private:			
				UInt_t	m_uSequence;
		};
	}
}

#endif
