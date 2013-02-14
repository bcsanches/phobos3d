/*
Phobos 3d
March 2010
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

#ifndef PH_CONTEXT_VAR_H
#define PH_CONTEXT_VAR_H

#include "Phobos/Shell/Item.h"
#include "Phobos/Defs.h"
#include "Phobos/Listener.h"

#include <boost/function.hpp>

namespace Phobos
{
	namespace Shell
	{
		class Variable;

		class VariableListener
		{
			public:
				virtual ~VariableListener() {};

				virtual void OnVariableValueChanged(const Variable &var) = 0;

			public:
				PH_DECLARE_LISTENER_HOOK;
		};

		/**
			This callback is called when the variable value changes, the parameters are:
				var -> reference to the variable that is being changed
				currentValue -> the current value on the variable
				newValue -> the new value being set

			The callback should throw an exception if the value cannot be set

		*/
		typedef boost::function3<void, const class Variable &, const String_t &, const String_t &> VariableCallback_t;			

		class PH_BASE_API Variable: public Item
		{				
			public:
				// =====================================================
				// PUBLIC METHODS
				// =====================================================
				inline Variable(const String_t &name, const String_t &value, VariableCallback_t callback = VariableCallback_t());

				/**
					This method may throw if the variable cannot accept the value

				*/
				void SetValue(const String_t &newValue);
			
				inline const String_t &GetValue(void) const;

				inline bool GetBoolean(void) const;
				inline Float_t GetFloat(void) const;
				inline int GetInt(void) const;

				inline void SetCallback(const VariableCallback_t &newCallback);

				PH_DECLARE_LISTENER_PROCS(VariableListener);	

			private:
				// =====================================================
				// PRIVATE TYPES
				// =====================================================
				String_t			m_strValue;
				VariableCallback_t	m_pfnCallback;

				PH_DECLARE_LISTENER_LIST(VariableListener, m_lstListeners);
		};

		// =====================================================
		// INLINE IMPLEMENTATION
		// =====================================================
		inline Variable::Variable(const String_t &name, const String_t &value, VariableCallback_t callback):
			Item(name),
			m_strValue(value),
			m_pfnCallback(callback)
		{
			//empty
		}

		inline const String_t &Variable::GetValue(void) const
		{
			return(m_strValue);
		}

		inline bool Variable::GetBoolean(void) const
		{
			return(StringToBoolean(m_strValue));
		}

		inline Float_t Variable::GetFloat(void) const
		{
			return(std::stof(m_strValue));
		}

		inline int	Variable::GetInt(void) const
		{
			return std::stoi(m_strValue);
		}

		inline void Variable::SetCallback(const VariableCallback_t &newCallback)
		{
			m_pfnCallback = newCallback;
		}
	}

}
#endif