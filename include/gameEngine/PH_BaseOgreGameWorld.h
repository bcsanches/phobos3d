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

#include <PH_String.h>

#include "PH_CollisionTagFwd.h"
#include "PH_RigidBodyFwd.h"

#define PH_WORLD_SCENE_MANAGER_NAME "SceneManager"

namespace Phobos
{	
	class Transform_c;

	class PH_GAME_ENGINE_API BaseOgreGameWorld_c: public GameWorld_c
	{
		protected:
			struct StaticObject_s
			{
				String_c strParent;

				Ogre::SceneNode *pclSceneNode;
				Ogre::Entity *pclEntity;
				Ogre::Light *pclLight;
				
				Physics::RigidBodyPtr_t spRigidBody;

				void Clear();

				inline StaticObject_s():
					pclSceneNode(NULL),
					pclEntity(NULL),
					pclLight(NULL)
				{
					//empty
				}
			};

			struct TempStaticObject_s
			{
				Ogre::Entity *pclEntity;
				Ogre::SceneNode *pclSceneNode;
				Ogre::Light *pclLight;		

				bool fParent;
				String_c strName;

				TempStaticObject_s():
					pclEntity(NULL),
					pclSceneNode(NULL),
					pclLight(NULL),						
					fParent(false)
				{
				}

				inline void Commit(StaticObject_s &object)
				{
					object.pclEntity = pclEntity;
					object.pclSceneNode = pclSceneNode;
					object.pclLight = pclLight;						

					pclEntity = NULL;
					pclLight = NULL;
					pclSceneNode = NULL;			
				}

				~TempStaticObject_s();
			};

			void CreateStaticObjectRigidBody(StaticObject_s &staticObj, const Transform_c &transform, const Ogre::Vector3 &scale, const Physics::CollisionTag_c &collisionTag) const;
	};
}

#endif
