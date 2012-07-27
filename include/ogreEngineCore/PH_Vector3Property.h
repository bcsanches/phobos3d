/*
Phobos 3d
July 2012
Copyright (c) 2005-2012 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_VECTOR3_PROPERTY_H
#define PH_VECTOR3_PROPERTY_H

#include <PH_NodeProperty.h>

#include <OgreVector3.h>

namespace Phobos
{
	class Vector3Property_c: public NodeProperty_c
	{
		private:
			Ogre::Vector3 v3Vector;
			
		public:
			Vector3Property_c(const char *name):
				NodeProperty_c(name),
				v3Vector(0, 0, 0)
			{
				//empty
			}			

			inline void SetVector(const Ogre::Vector3 &vector)
			{
				v3Vector = vector;
			}

			inline const Ogre::Vector3 &GetVector(void) const
			{
				return v3Vector;
			}			
	};
}

#endif