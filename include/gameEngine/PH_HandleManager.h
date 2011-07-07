/*
Phobos 3d
July 2011
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

#ifndef PH_ENTITY_CONTAINER_H
#define PH_ENTITY_CONTAINER_H

#include <PH_Exception.h>
#include <PH_Types.h>

//Based on http://gamesfromwithin.com/managing-data-relationships
namespace Phobos
{
	struct Handle_s
	{
		Handle_s():
			u12Index(0),
			u20Counter(0)
		{
		}

		Handle_s(UInt32_t index, UInt32_t counter):
            u12Index(index),
			u20Counter(counter)
		{
			//empty
		}

		inline operator UInt32_t () const
		{
			return u20Counter << 12 | u12Index;
		}

		UInt32_t u12Index : 12;
		UInt32_t u20Counter: 20;
	};

	template <typename T>
	class HandleManager_c
	{
		public:
			enum { MAX_ENTRIES = 4096 };

			HandleManager_c()
			{
				this->Clear();
			}

			void Clear()
			{
				uNextFreeItem = 0;
				uActiveObjects = 0;

				for(int i = 0;i < MAX_ENTRIES; ++i)
				{
					arstObjects[i].u12NextFreeItem = i + 1;
				}

				arstObjects[MAX_ENTRIES-1].u12NextFreeItem = 0;
			}

			Handle_s AddObject(T *ptr)
			{
				if(uActiveObjects == MAX_ENTRIES)
					PH_RAISE(INVALID_OPERATION_EXCEPTION, "[HandleManager_c::AddObject]", "HandleManager is full.");

				const int index = uNextFreeItem;
				uNextFreeItem = arstObjects[index].u12NextFreeItem;

				arstObjects[index].u20Counter++;

				//Never allow counter 0, to avoid blank handles
				if(arstObjects[index].u20Counter == 0)
					arstObjects[index].u20Counter = 1;

				arstObjects[index].ptObject = ptr;

				++uActiveObjects;

				return Handle_s(index, arstObjects[index].u20Counter);
			}

			void RemoveObject(const Handle_s handle)
			{
				const UInt_t index = handle.u12Index;
				if(arstObjects[index].u20Counter != handle.u20Counter)
					PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[HandleManager_c::RemoveObject]", "Invalid counter value.");

				if(arstObjects[index].ptObject == NULL)
					PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[HandleManager_c::RemoveObject]", "Object already removed, ptObject == NULL");

				arstObjects[index].ptObject = NULL;
				arstObjects[index].u12NextFreeItem = uNextFreeItem;
				uNextFreeItem = index;

				--uActiveObjects;
			}

			T *TryGetObject(const Handle_s handle) const
			{
				const UInt_t index = handle.u12Index;

				if(arstObjects[index].u20Counter != handle.u20Counter)
					return NULL;

				return arstObjects[index].ptObject;
			}

			inline UInt_t GetNumActiveObjects() const
			{
				return uActiveObjects;
			}


		private:
			struct HandleItem_s
			{
				UInt32_t u12NextFreeItem:12;
				UInt32_t u20Counter:20;

				T *ptObject;

				HandleItem_s():
					u12NextFreeItem(0),
					u20Counter(0),
					ptObject(NULL)
				{
					//empty
				}
			};

			HandleItem_s arstObjects[MAX_ENTRIES];
			UInt_t uNextFreeItem;
			UInt_t uActiveObjects;
	};
}

#endif
