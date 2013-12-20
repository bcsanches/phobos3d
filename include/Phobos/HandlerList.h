/*
Phobos 3d
July 2013
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

#ifndef PH_HANDLER_LIST_H
#define PH_HANDLER_LIST_H

#include "String.h"
#include "Exception.h"

#include <vector>

namespace Phobos
{	
	class Handler	
	{
		private:
			//safe bool: http://en.wikibooks.org/wiki/More_C%2B%2B_Idioms/Safe_bool
			typedef void (Handler::*BoolType_t)() const;
			void this_type_does_not_support_comparisons() const {}

		public:
			Handler():
				m_u16Serial(0),
				m_u16Index(0)
			{
				//empty
			}

			Handler(UInt16_t index, UInt16_t serial):
				m_u16Index(index),
				m_u16Serial(serial)
			{
				//empty
			}

			explicit Handler(StringRef_t handler) 
			{
				UInt32_t uHandler = std::stoi(handler.data());

				m_u16Index = uHandler & 0x0000FFFF;
				m_u16Serial = (uHandler >> 16);
			}
				
			inline UInt16_t GetIndex() const
			{
				return m_u16Index;
			}

			inline UInt16_t GetSerial() const
			{
				return m_u16Serial;
			}

			String_t ToString() const
			{
				return std::to_string(UInt32_t((m_u16Serial << 16) | m_u16Index));
			}

			operator BoolType_t() const
			{
				return m_u16Serial ? &Handler::this_type_does_not_support_comparisons : nullptr;
			}

		private:
			UInt16_t m_u16Serial;
			UInt16_t m_u16Index;
	};

	template <typename T>
	class HandlerList
	{
		public:
			static const UInt32_t MAX_HANDLERS = 0x0000FFFF;					

		public:
			HandlerList():
				m_u16NextFreeSlot(MAX_HANDLERS)
			{
				//empty
			}

			std::pair<Handler, T&> Add(T &&object)
			{
				auto slot = m_u16NextFreeSlot;

				if(slot == MAX_HANDLERS)
				{					
					m_vecObjects.emplace_back(std::make_pair(MiniHandler(), std::move(object)));
					slot = m_vecObjects.size()-1;
				}
				else
				{					
					m_u16NextFreeSlot = m_vecObjects[slot].first.m_u16NextFreeSlot;

					m_vecObjects[slot].second = std::move(object);
				}

				return std::make_pair(Handler(slot, m_vecObjects[slot].first.m_u16Serial), std::ref(m_vecObjects[slot].second));
			}

			T Remove(Handler h)
			{
				auto index = GetValidIndex(h);				

				//increment serial to avoid re-use
				m_vecObjects[index].first.m_u16Serial++;

				//queue free slot
				m_vecObjects[index].first.m_u16NextFreeSlot = m_u16NextFreeSlot;
				m_u16NextFreeSlot = index;

				//destory it
				return std::move(m_vecObjects[index].second);
			}

			template <typename CARRIER>
			CARRIER Acquire(Handler h)
			{
				using namespace Phobos;
								
				auto index = this->GetValidIndex(h);

				return CARRIER(m_vecObjects[index].second, h);
			}

			T &Get(Handler h)
			{				
				return m_vecObjects[GetValidIndex(h)].second;
			}

			void Clear()
			{
				m_u16NextFreeSlot = MAX_HANDLERS;
				m_vecObjects.clear();
			}

		private:			

			Phobos::UInt16_t GetValidIndex(Handler h)
			{
				const Phobos::UInt16_t index = h.GetIndex();
				const Phobos::UInt16_t serial = h.GetSerial();

				if(index >= m_vecObjects.size())
				{
					PH_RAISE(Phobos::INVALID_PARAMETER_EXCEPTION, "[RenderWorldImpl::AcquireDynamicSceneNodeHandler]", "handler is out of bounds");
				}

				if(m_vecObjects[index].first.m_u16Serial != serial)
				{
					PH_RAISE(Phobos::OBJECT_NOT_FOUND_EXCEPTION, "[RenderWorldImpl::AcquireDynamicSceneNodeHandler]", "Invalid serial for handler");
				}

				//return an valid index
				return index;
			}

			struct MiniHandler
			{
				Phobos::UInt16_t m_u16Serial;
				Phobos::UInt16_t m_u16NextFreeSlot;

				MiniHandler():
					m_u16Serial(1),
					m_u16NextFreeSlot(MAX_HANDLERS)
				{
					//empty
				}
			};

		private:
			typedef std::vector<std::pair<MiniHandler, T>> Vector_t;

			Vector_t m_vecObjects;

			Phobos::UInt16_t m_u16NextFreeSlot;
	};
}

#endif
