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
			typedef boost::unordered_map<String_c, String_c> StringMap_t;
			typedef StringMap_t::const_iterator StringMapConstIterator_t;

		public:
			static DictionaryPtr_t Create(const String_c &name);

			void Load(Parser_c &parser);

			const String_c &GetValue(const String_c &key) const;
			bool TryGetValue(const String_c &key, String_c &out) const;
			const String_c *TryGetValue(const String_c &key) const;

			bool GetBool(const String_c &key) const;

			void Get4Float(const float values[4], const String_c &key) const;
			void Get3Float(const float values[3], const String_c &key) const;
			int GetInt(const String_c &key) const;
			float GetFloat(const String_c &key) const;

			void AddValue(const String_c &key, const String_c &value);

			inline StringMapConstIterator_t begin() const;
			inline StringMapConstIterator_t end() const;

			const Dictionary_c *GetInherited() const;

		private:
			Dictionary_c(const String_c &name);
			~Dictionary_c();			

			static const String_c *TryGetValue(const Dictionary_c *current, const String_c &key);			

		private:			
			StringMap_t mapValues;

			String_c strInherit;
			String_c strBaseHive;
			mutable const Dictionary_c *pclInherit;
	};

	inline Dictionary_c::StringMapConstIterator_t Dictionary_c::begin() const
	{
		return mapValues.begin();
	}

	inline Dictionary_c::StringMapConstIterator_t Dictionary_c::end() const
	{
		return mapValues.end();
	}
}

#endif
