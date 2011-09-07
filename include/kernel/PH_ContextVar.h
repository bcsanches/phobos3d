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

#include "PH_ContextItem.h"
#include "PH_Defs.h"
#include "PH_Listener.h"

#include <boost/function.hpp>

namespace Phobos
{
	class ContextVarListener_c
	{
		public:
			virtual ~ContextVarListener_c() {};

			virtual void OnVariableValueChanged(const class ContextVar_c &var) = 0;

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
	typedef boost::function3<void, const class ContextVar_c &, const String_c &, const String_c &> VarCallback_t;			

	class PH_KERNEL_API ContextVar_c: public ContextItem_c
	{				
		public:
			// =====================================================
			// PUBLIC METHODS
			// =====================================================
			inline ContextVar_c(const String_c &name, const String_c &value, VarCallback_t callback = VarCallback_t());

			/**
				This method may throw if the variable cannot accept the value

			*/
			void SetValue(const String_c &newValue);
			
			inline const String_c &GetValue(void) const;

			inline bool GetBoolean(void) const;
			inline Float_t GetFloat(void) const;
			inline int GetInt(void) const;

			inline void SetCallback(const VarCallback_t &newCallback);

			PH_DECLARE_LISTENER_PROCS(ContextVarListener_c);	

		private:
			// =====================================================
			// PRIVATE TYPES
			// =====================================================
			String_c		strValue;
			VarCallback_t	pfnCallback;

			PH_DECLARE_LISTENER_LIST(ContextVarListener_c, lstListeners);
	};

	// =====================================================
	// INLINE IMPLEMENTATION
	// =====================================================
	inline ContextVar_c::ContextVar_c(const String_c &name, const String_c &value, VarCallback_t callback):
		ContextItem_c(name),
		strValue(value),
		pfnCallback(callback)
	{
		//empty
	}

	inline const String_c &ContextVar_c::GetValue(void) const
	{
		return(strValue);
	}

	inline bool ContextVar_c::GetBoolean(void) const
	{
		return(StringToBoolean(strValue));
	}

	inline Float_t ContextVar_c::GetFloat(void) const
	{
		return(StringToFloat(strValue));
	}

	inline int	ContextVar_c::GetInt(void) const
	{
		return StringToInt(strValue);
	}

	inline void ContextVar_c::SetCallback(const VarCallback_t &newCallback)
	{
		pfnCallback = newCallback;
	}

}
#endif