/*
Phobos 3d
June 2010
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

#ifndef PH_TRANSFORM_PROPERTY_H
#define PH_TRANSFORM_PROPERTY_H

#include <Phobos/NodeProperty.h>

#include "Math/Transform.h"

namespace Phobos
{
	namespace OgreEngine
	{
		class TransformProperty: public NodeProperty
		{
			private:
				Engine::Math::Transform m_clTransform;
			
			public:
				TransformProperty(const char *name):
					NodeProperty(name)
				{
					//empty
				}			

				inline void SetTransform(const Engine::Math::Transform &transform)
				{
					m_clTransform = transform;
				}

				inline const Ogre::Vector3 &GetOrigin(void) const
				{
					return m_clTransform.GetOrigin();
				}

				inline const Ogre::Quaternion &GetRotation(void) const
				{
					return m_clTransform.GetRotation();
				}

				inline void SetRotation(const Ogre::Quaternion &q)
				{
					m_clTransform.SetRotation(q);
				}

				inline void SetOrigin(const Ogre::Vector3 &v)
				{
					m_clTransform.SetOrigin(v);
				}

				inline void Translate(const Ogre::Vector3 &v)
				{
					m_clTransform.Translate(v);
				}

				inline void Rotate(const Ogre::Quaternion &q)
				{
					m_clTransform.Rotate(q);
				}

				inline void RotateX(const Ogre::Degree &angle)
				{
					m_clTransform.RotateX(angle);
				}

				inline void RotateY(const Ogre::Degree &angle)
				{
					m_clTransform.RotateY(angle);
				}

				inline void RotateZ(const Ogre::Degree &angle)
				{
					m_clTransform.RotateZ(angle);
				}

				inline void Rotate(const Ogre::Vector3 &axis, const Ogre::Degree &angle)
				{
					m_clTransform.Rotate(axis, angle);
				}

				inline const Engine::Math::Transform &GetTransform() const
				{
					return m_clTransform;
				}
		};
	}
}

#endif
