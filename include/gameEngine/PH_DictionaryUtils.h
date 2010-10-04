/*
Phobos 3d
  October 2010

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

#ifndef PH_DICTIONARY_UTILS_H
#define PH_DICTIONARY_UTILS_H

#include <PH_Dictionary.h>

#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace Phobos
{
	inline Ogre::Vector3 DictionaryGetVector3(const Dictionary_c &dict, const String_c &key)
	{		
		const String_c &value = dict.GetValue(key);

		Ogre::Vector3 tmp;
		sscanf(value.c_str(), "%f %f %f", &tmp.x, &tmp.y, &tmp.z);

		return tmp;
	}

	inline Ogre::Quaternion DictionaryGetQuaternion(const Dictionary_c &dict, const String_c &key)
	{
		const String_c &value = dict.GetValue(key);

		Ogre::Quaternion q;
		sscanf(value.c_str(), "%f %f %f %f", &q.w, &q.x, &q.y, &q.z);

		return q;
	}

	inline Ogre::ColourValue DictionaryGetColour(const Dictionary_c &dict, const String_c &key)
	{
		const String_c &value = dict.GetValue(key);

		Ogre::ColourValue colour;

		sscanf(value.c_str(), "%f %f %f %f", &colour.r, &colour.g, &colour.b, &colour.a);

		return colour;
	}
}

#endif
