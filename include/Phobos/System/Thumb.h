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

#ifndef PH_SYSTEM_THUMB_H
#define PH_SYSTEM_THUMB_H

#include <Phobos/Error.h>
#include <Phobos/Shell/Command.h>
#include <Phobos/Shell/IContext.h>

#include "Phobos/System/SystemAPI.h"

namespace Phobos
{	
	namespace System
	{		
		class PH_SYSTEM_API Thumb
		{
			public:
				Thumb(const String_t &cmd, Shell::IContext *context = NULL);

				inline Float_t GetX() const;
				inline Float_t GetY() const;
				inline const Float_t *GetPoint() const;

				void Enable(Shell::IContext &context);
				void Disable();

			private:
				void CmdProc(const Shell::StringVector_t &args, Shell::Context &);

			private:
				Shell::Command m_cmdUpdate;

				Float_t	m_fpPoint[2];		
		};

		inline Float_t Thumb::GetX() const
		{
			return m_fpPoint[0];
		}

		inline Float_t Thumb::GetY() const
		{
			return m_fpPoint[1];
		}

		inline const Float_t *Thumb::GetPoint() const
		{
			return m_fpPoint;
		}
	}
}

#endif
