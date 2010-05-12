/*
Phobos 3d
  May 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

  This software is provided 'as-is', without any express or implied
  warranty. In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Bruno Crivelari Sanches bcsanches@gmail.com
*/

#include "PH_OgreUtils.h"

#include <string>

#include <boost/detail/atomic_count.hpp>

#include <PH_Kernel.h>

namespace Phobos
{
	static boost::detail::atomic_count tCount_gl(0);

	void LogOgreException(const Char_t *moduleName, const Ogre::Exception &ex)
	{
		std::stringstream stream;

		stream << "[" << moduleName << "]: Exception: " << ex.getFullDescription();
		Kernel_c::GetInstance().LogMessage(stream.str());
	}

	String_c &GenerateOgreName(String_c &out)
	{		
		out = "name_";
		out += NumberToString(++tCount_gl);	

		return out;
	}
}