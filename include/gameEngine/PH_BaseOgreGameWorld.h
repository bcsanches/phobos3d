/*
Phobos 3d
May 2012
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

#ifndef PH_BASE_OGRE_GAME_WORLD_H
#define PH_BASE_OGRE_GAME_WORLD_H

#include "PH_GameWorld.h"

#include <OgrePrerequisites.h>

#include <Phobos/String.h>

#include <Phobos/OgreEngine/Math/TransformFwd.h>

#include "PH_CollisionTagFwd.h"
#include "PH_RigidBodyFwd.h"

#define PH_WORLD_SCENE_MANAGER_NAME "SceneManager"

namespace Phobos
{		
	class PH_GAME_ENGINE_API BaseOgreGameWorld: public GameWorld
	{
		protected:
			struct StaticObject_s
			{
				String_t strParent;

				Ogre::SceneNode *m_pclSceneNode;
				Ogre::Entity *m_pclEntity;
				Ogre::Light *m_pclLight;
				
				Physics::RigidBodyPtr_t m_spRigidBody;

				void Clear();

				inline StaticObject_s():
					m_pclSceneNode(NULL),
					m_pclEntity(NULL),
					m_pclLight(NULL)
				{
					//empty
				}
			};

			struct TempStaticObject_s
			{
				Ogre::Entity *m_pclEntity;
				Ogre::SceneNode *m_pclSceneNode;
				Ogre::Light *m_pclLight;		

				bool m_fParent;
				String_t m_strName;

				TempStaticObject_s():
					m_pclEntity(NULL),
					m_pclSceneNode(NULL),
					m_pclLight(NULL),						
					m_fParent(false)
				{
				}

				inline void Commit(StaticObject_s &object)
				{
					object.m_pclEntity = m_pclEntity;
					object.m_pclSceneNode = m_pclSceneNode;
					object.m_pclLight = m_pclLight;						

					m_pclEntity = NULL;
					m_pclLight = NULL;
					m_pclSceneNode = NULL;			
				}

				~TempStaticObject_s();
			};

			void CreateStaticObjectRigidBody(StaticObject_s &staticObj, const Engine::Math::Transform &transform, const Ogre::Vector3 &scale, const Physics::CollisionTag &collisionTag) const;
	};
}

#endif
