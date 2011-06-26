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

#ifndef PH_THUMB_H
#define PH_THUMB_H

#include <PH_Error.h>
#include <PH_ContextCmd.h>

#include "PH_InputAPI.h"

namespace Phobos
{
	class IContext_c;

	class PH_INPUT_API Thumb_c
	{
		public:
			Thumb_c(const String_c &cmd, IContext_c *context = NULL);

			inline Float_t GetX() const;
			inline Float_t GetY() const;
			inline const Float_t *GetPoint() const;

			void Enable(IContext_c &context);
			void Disable();

		private:
			void CmdProc(const StringVector_t &args, Context_c &);

		private:
			ContextCmd_c cmdUpdate;

			Float_t	fpPoint[2];		
	};

	inline Float_t Thumb_c::GetX() const
	{
		return fpPoint[0];
	}

	inline Float_t Thumb_c::GetY() const
	{
		return fpPoint[1];
	}

	inline const Float_t *Thumb_c::GetPoint() const
	{
		return fpPoint;
	}
}

#endif
