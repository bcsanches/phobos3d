/*
Phobos 3d
October 2010
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


#ifndef PH_SYSTEM_BUTTON_H
#define PH_SYSTEM_BUTTON_H

#include <PH_Types.h>
#include <PH_ContextCmd.h>

#include "Phobos/System/SystemAPI.h"

namespace Phobos
{	
	class IContext_c;

	namespace System
	{
		class PH_SYSTEM_API Button_c
		{
			public:
				Button_c(const String_c &up, const String_c &down, const String_c &update, IContext_c *context = NULL);		

				inline Float_t GetValue(void) const
				{
					return(fpValue);
				}

				void Enable(IContext_c &context);
				void Disable();

			private:
				void CmdProc(const StringVector_t &args, Context_c & );

			private:
				Float_t fpValue;

				ContextCmd_c cmdUp;
				ContextCmd_c cmdDown;
				ContextCmd_c cmdUpdate;
		};
	}
}

#endif
