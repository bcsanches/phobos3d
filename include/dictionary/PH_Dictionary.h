/*
Phobos 3d
  September 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#ifndef PH_DICTIONARY_H
#define PH_DICTIONARY_H

#include <boost/unordered_map.hpp>

#include <PH_Node.h>

#include "PH_DictionaryAPI.h"
#include "PH_DictionaryFwd.h"

namespace Phobos
{	
	class Parser_c;

	class PH_DICTIONARY_API Dictionary_c: public Node_c
	{
		public:
			static DictionaryPtr_t Create(const String_c &name);

			void Load(Parser_c &parser);

			const String_c &GetValue(const String_c &key) const;
			bool TryGetValue(const String_c &key, String_c &out) const;

		private:
			Dictionary_c(const String_c &name);
			~Dictionary_c();

			const Dictionary_c *GetInherited() const;

			static const String_c *TryGetValue(const Dictionary_c *current, const String_c &key);			

		private:
			typedef boost::unordered_map<String_c, String_c> StringMap_t;
			StringMap_t mapValues;

			String_c strInherit;
			mutable const Dictionary_c *pclInherit;
	};
}

#endif
