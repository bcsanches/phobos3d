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

#ifndef PH_TRANSFORM_H
#define PH_TRANSFORM_H

#include <OgreVector3.h>
#include <OgreQuaternion.h>

#include "PH_OgreMathFunctions.h"
#include "Math/PH_Interpolator.h"

namespace Phobos
{
	class Transform_c
	{
		public:
			inline Transform_c(const Ogre::Vector3 &origin = Ogre::Vector3::ZERO, const Ogre::Quaternion &q = Ogre::Quaternion::IDENTITY);		
			inline Transform_c(const Transform_c &from, const Transform_c &to, Float_t delta);

			inline const Ogre::Vector3 &GetOrigin(void) const;
			inline const Ogre::Quaternion &GetRotation(void) const;

			inline void SetRotation(const Ogre::Quaternion &q);
			inline void SetOrigin(const Ogre::Vector3 &v);
			inline void Translate(const Ogre::Vector3 &v);

			inline void Rotate(const Ogre::Quaternion &q);

			inline void RotateX(const Ogre::Degree &angle);
			inline void RotateY(const Ogre::Degree &angle);
			inline void RotateZ(const Ogre::Degree &angle);

			inline void Rotate(const Ogre::Vector3 &axis, const Ogre::Degree &angle);

			inline static void Interpolate(Transform_c &out, const Transform_c &a, const Transform_c &b, Float_t delta);
			inline static Transform_c Interpolate(const Transform_c &a, const Transform_c &b, Float_t delta);

		private:
			Ogre::Vector3 v3Origin;
			Ogre::Quaternion qRotation;
	};

	inline Transform_c::Transform_c(const Ogre::Vector3 &origin, const Ogre::Quaternion &q):
		v3Origin(origin),
		qRotation(q)
	{
		//empty
	}

	inline Transform_c::Transform_c(const Transform_c &from, const Transform_c &to, Float_t delta):
		v3Origin(Math::Interpolate(from.v3Origin, to.v3Origin, delta)),
		qRotation(Math::Interpolate(from.qRotation, to.qRotation, delta))
	{
		//empty
	}

	inline const Ogre::Vector3 &Transform_c::GetOrigin(void) const
	{
		return(v3Origin);
	}

	inline const Ogre::Quaternion &Transform_c::GetRotation(void) const
	{
		return(qRotation);
	}

	inline void Transform_c::RotateX(const Ogre::Degree &angle)
	{
		this->Rotate(Ogre::Vector3::UNIT_X, angle);
	}

	inline void Transform_c::RotateY(const Ogre::Degree &angle)
	{
		this->Rotate(Ogre::Vector3::UNIT_Y, angle);
	}

	inline void Transform_c::RotateZ(const Ogre::Degree &angle)
	{
		this->Rotate(Ogre::Vector3::UNIT_Z, angle);
	}

	inline void Transform_c::Rotate(const Ogre::Vector3 &axis, const Ogre::Degree &angle)
	{
		//Ogre::Quaternion q(angle, axis);
		Ogre::Quaternion q;
		q.FromAngleAxis(angle, axis);

		this->Rotate(q);
	}

	inline void Transform_c::SetRotation(const Ogre::Quaternion &q)
	{
		qRotation = q;
	}

	inline void Transform_c::Rotate(const Ogre::Quaternion &q)
	{
		qRotation = qRotation * q;
	}

	inline void Transform_c::SetOrigin(const Ogre::Vector3 &v)
	{
		v3Origin = v;
	}

	inline void Transform_c::Translate(const Ogre::Vector3 &v)
	{
		v3Origin += v;
	}

	inline void Transform_c::Interpolate(Transform_c &out, const Transform_c &a, const Transform_c &b, Float_t delta)
	{
		out = Transform_c(a, b, delta);	
	}

	inline Transform_c Transform_c::Interpolate(const Transform_c &a, const Transform_c &b, Float_t delta)
	{
		return Transform_c(a, b, delta);
	}

	namespace Math
	{
		template <>
		inline Transform_c Interpolate(const Transform_c &a, const Transform_c &b, Float_t alpha)
		{
			return(Transform_c(
				Interpolate(a.GetOrigin(), b.GetOrigin(), alpha), 
				Interpolate(a.GetRotation(), b.GetRotation(), alpha)
			));
		}
	}


	typedef Math::Interpolator_c<Transform_c> TransformInterpolator_c;
}


#endif
