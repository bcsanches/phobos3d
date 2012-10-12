/*
Phobos 3d
October 2010
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

#ifndef PH_ICONTEXT_H
#define PH_ICONTEXT_H

#include <PH_String.h>

namespace Phobos
{
	class ContextVar_c;

	class IContext_c
	{
		public:
			virtual void AddContextVar(ContextVar_c &var) = 0;			

			virtual void AddContextCmd(ContextCmd_c &cmd) = 0;
			virtual void RemoveContextCmd(ContextCmd_c &cmd) = 0;

			virtual const ContextVar_c &GetContextVar(const String_c &name) const = 0;
			virtual const ContextVar_c *TryGetContextVar(const String_c &name) const = 0;

			virtual void Execute(const String_c &cmd) = 0;

		protected:
			virtual ~IContext_c() {}

	};
}

#endif