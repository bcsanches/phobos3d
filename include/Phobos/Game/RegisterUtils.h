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

#ifndef PH_GAME_REGISTER_UTILS_H
#define PH_GAME_REGISTER_UTILS_H

#include <Phobos/Register/Table.h>

#include <OgreColourValue.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>

namespace Phobos
{	
	namespace Register	
	{
		inline void SetVector3(Phobos::Register::Table &table, StringRef_t key, const Ogre::Vector3 &v)
		{
			std::stringstream stream;

			stream << v.x << ' ' << v.y << ' ' << v.z;
			table.SetString(key, stream.str());
		}

		inline Ogre::Vector3 GetVector3(const Phobos::Register::Table &dict, const String_t &key)
		{		
			const String_t &value = dict.GetString(key);

			Ogre::Vector3 tmp;
			sscanf(value.c_str(), "%f %f %f", &tmp.x, &tmp.y, &tmp.z);

			return tmp;
		}

		inline Ogre::Vector3 TryGetVector3(const Phobos::Register::Table &dict, const String_t &key, const Ogre::Vector3 &defaultValue)
		{		
			const String_t *value = dict.TryGetString(key);
			if(value)
			{
				Ogre::Vector3 tmp;
				sscanf(value->c_str(), "%f %f %f", &tmp.x, &tmp.y, &tmp.z);

				return tmp;
			}
			else
			{
				return defaultValue;
			}
		}

		inline void SetQuaternion(Phobos::Register::Table &table, StringRef_t key, const Ogre::Quaternion &q)
		{
			std::stringstream stream;

			stream << q.w << ' ' << q.x << ' ' << q.y << ' ' << q.z;
			table.SetString(key, stream.str());
		}

		inline Ogre::Quaternion GetQuaternion(const Phobos::Register::Table &dict, const String_t &key)
		{
			const String_t &value = dict.GetString(key);

			Ogre::Quaternion q;
			sscanf(value.c_str(), "%f %f %f %f", &q.w, &q.x, &q.y, &q.z);

			return q;
		}

		inline Ogre::ColourValue GetColour(const Phobos::Register::Table &dict, const String_t &key)
		{
			const String_t &value = dict.GetString(key);

			Ogre::ColourValue colour;

			sscanf(value.c_str(), "%f %f %f %f", &colour.r, &colour.g, &colour.b, &colour.a);

			return colour;
		}		
	}	
}

#endif
