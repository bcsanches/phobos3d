/*
Phobos 3d
May 2010
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

#include "Phobos/OgreEngine/Utils.h"

#include <string>

#include <atomic>

#include <Phobos/Log.h>

namespace Phobos
{
	static std::atomic_uint tCount_gl;

	void LogOgreException(const Char_t *moduleName, const Ogre::Exception &ex)
	{		
		LogMakeStream() << "[" << moduleName << "]: Exception: " << ex.getFullDescription();
	}

	String_t &GenerateOgreName(String_t &out)
	{		
		out = "name_";
		out += std::to_string(++tCount_gl);	

		return out;
	}
}
