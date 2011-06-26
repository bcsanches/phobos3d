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
#ifndef PH_DICTIONARY_MANAGER_H
#define PH_DICTIONARY_MANAGER_H

#include <PH_ContextCmd.h>
#include <PH_Node.h>
#include <PH_Singleton.h>

#include <istream>

#include "PH_DictionaryAPI.h"
#include "PH_DictionaryFwd.h"
#include "PH_DictionaryHiveFwd.h"

namespace Phobos
{
	class IContext_c;
	class Path_c;

	PH_DECLARE_SINGLETON_PTR(DictionaryManager);

	class PH_DICTIONARY_API DictionaryManager_c: public Node_c
	{
		PH_DECLARE_SINGLETON_METHODS(DictionaryManager);	

		public:
			void Load(const String_c &fileName);
			void Load(std::istream &stream);

			void LoadAll(const String_c &path);

			DictionaryHivePtr_t CreateCustomHive(const String_c &name);			

			DictionaryHivePtr_t GetDictionaryHive(const String_c &name);
			DictionaryHivePtr_t TryGetDictionaryHive(const String_c &name);
			DictionaryPtr_t GetDictionary(const String_c &hive, const String_c &dictionary);
			DictionaryPtr_t TryGetDictionary(const String_c &hive, const String_c &dictionary);
			DictionaryPtr_t GetDictionary(const Path_c &relativePath);

			void RegisterCommands(IContext_c &context);

		private:
			DictionaryManager_c();
			~DictionaryManager_c();

			void CmdLoadAllDeclarations(const StringVector_t &args, Context_c &);

		private:
			ContextCmd_c cmdLoadAllDeclarations;
	};
}

#endif