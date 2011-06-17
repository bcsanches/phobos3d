/*
Phobos 3d
  June 2011

  Copyright (C) 2005-2011 Bruno Crivelari Sanches

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

#include <PH_NodeProperty.h>

#include "PH_Transform.h"

namespace Phobos
{
	class TransformProperty_c: public NodeProperty_c
	{
		private:
			Transform_c clTransform;
			
		public:
			TransformProperty_c(const char *name):
				NodeProperty_c(name)
			{
				//empty
			}			

			inline const Ogre::Vector3 &GetOrigin(void) const
			{
				return clTransform.GetOrigin();
			}

			inline const Ogre::Quaternion &GetRotation(void) const
			{
				return clTransform.GetRotation();
			}

			inline void SetRotation(const Ogre::Quaternion &q)
			{
				clTransform.SetRotation(q);
			}

			inline void SetOrigin(const Ogre::Vector3 &v)
			{
				clTransform.SetOrigin(v);
			}

			inline void Translate(const Ogre::Vector3 &v)
			{
				clTransform.Translate(v);
			}

			inline void Rotate(const Ogre::Quaternion &q)
			{
				clTransform.Rotate(q);
			}

			inline void RotateX(const Ogre::Degree &angle)
			{
				clTransform.RotateX(angle);
			}

			inline void RotateY(const Ogre::Degree &angle)
			{
				clTransform.RotateY(angle);
			}

			inline void RotateZ(const Ogre::Degree &angle)
			{
				clTransform.RotateZ(angle);
			}

			inline void Rotate(const Ogre::Vector3 &axis, const Ogre::Degree &angle)
			{
				clTransform.Rotate(axis, angle);
			}
	};
}
