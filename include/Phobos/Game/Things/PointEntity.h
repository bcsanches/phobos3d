/*
Phobos 3d
October 2011
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


#ifndef PH_POINT_ENGINE_H
#define PH_POINT_ENGINE_H

#include "Phobos/Game/Things/Entity.h"

#include <Phobos/OgreEngine/Math/Transform.h>

namespace Phobos
{
	namespace Game
	{
		namespace Things
		{
			PH_DECLARE_NODE_PTR(PointEntity);

			class PointEntity: public Entity
			{
				public:
					static std::unique_ptr<Entity> Create(const String_t &name);

					inline const Engine::Math::Transform &GetTransform() const;

				protected:
					PointEntity(const String_t &name);

					void OnLoad(const Register::Table &table);

				private:
					Engine::Math::Transform	m_clTransform;
			};

			inline const Engine::Math::Transform &PointEntity::GetTransform() const
			{
				return m_clTransform;
			}
		}
	}
}


#endif
