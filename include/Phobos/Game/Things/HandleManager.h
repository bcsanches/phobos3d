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

#ifndef PH_HANDLE_MANAGER_H
#define PH_HANDLE_MANAGER_H

#include <Phobos/Exception.h>
#include <Phobos/Types.h>

//Based on http://gamesfromwithin.com/managing-data-relationships
namespace Phobos
{
	namespace Game
	{
		namespace Things
		{
			struct Handle_s
			{
				Handle_s():
					m_u12Index(0),
					m_u20Counter(0)
				{
				}

				Handle_s(UInt32_t index, UInt32_t counter):
					m_u12Index(index),
					m_u20Counter(counter)
				{
					//empty
				}

				inline operator UInt32_t () const
				{
					return m_u20Counter << 12 | m_u12Index;
				}

				UInt32_t m_u12Index : 12;
				UInt32_t m_u20Counter: 20;
			};

			enum
			{
				MAX_HANDLES = 4096
			};
	
			template <typename T>
			class HandleManager
			{
				public:
					enum { MAX_ENTRIES = MAX_HANDLES };

					HandleManager()
					{
						this->Clear();
					}

					void Clear()
					{
						m_uNextFreeItem = 0;
						m_uActiveObjects = 0;

						for(int i = 0;i < MAX_ENTRIES; ++i)
						{
							m_arstObjects[i].m_u12NextFreeItem = i + 1;
						}

						m_arstObjects[MAX_ENTRIES-1].m_u12NextFreeItem = 0;
					}

					Handle_s AddObject(T *ptr)
					{
						if(m_uActiveObjects == MAX_ENTRIES)
							PH_RAISE(INVALID_OPERATION_EXCEPTION, "[HandleManager::AddObject]", "HandleManager is full.");

						const int index = m_uNextFreeItem;
						m_uNextFreeItem = m_arstObjects[index].m_u12NextFreeItem;

						m_arstObjects[index].m_u20Counter++;

						//Never allow counter 0, to avoid blank handles
						if(m_arstObjects[index].m_u20Counter == 0)
							m_arstObjects[index].m_u20Counter = 1;

						m_arstObjects[index].m_ptObject = ptr;

						++m_uActiveObjects;

						return Handle_s(index, m_arstObjects[index].m_u20Counter);
					}

					void RemoveObject(const Handle_s handle)
					{
						const UInt_t index = handle.m_u12Index;
						if(m_arstObjects[index].m_u20Counter != handle.m_u20Counter)
							PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[HandleManager::RemoveObject]", "Invalid counter value.");

						if(m_arstObjects[index].m_ptObject == NULL)
							PH_RAISE(INVALID_PARAMETER_EXCEPTION, "[HandleManager::RemoveObject]", "Object already removed, ptObject == NULL");

						m_arstObjects[index].m_ptObject = NULL;
						m_arstObjects[index].m_u12NextFreeItem = m_uNextFreeItem;
						m_uNextFreeItem = index;

						--m_uActiveObjects;
					}

					T *TryGetObject(const Handle_s handle) const
					{
						const UInt_t index = handle.m_u12Index;

						if(m_arstObjects[index].m_u20Counter != handle.m_u20Counter)
							return NULL;

						return m_arstObjects[index].m_ptObject;
					}

					inline UInt_t GetNumActiveObjects() const
					{
						return m_uActiveObjects;
					}


				private:
					struct HandleItem_s
					{
						UInt32_t m_u12NextFreeItem:12;
						UInt32_t m_u20Counter:20;

						T *m_ptObject;

						HandleItem_s():
							m_u12NextFreeItem(0),
							m_u20Counter(0),
							m_ptObject(NULL)
						{
							//empty
						}
					};

					HandleItem_s m_arstObjects[MAX_ENTRIES];
					UInt_t m_uNextFreeItem;
					UInt_t m_uActiveObjects;
			};
		}
	}
}

#endif
