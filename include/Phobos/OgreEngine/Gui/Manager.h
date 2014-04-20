/*
Phobos 3d
September 2012
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

#ifndef PH_GUI_OGRE_MANAGER_H
#define PH_GUI_OGRE_MANAGER_H

#include <list>

#include <OgreRenderQueueListener.h>

#include <Phobos/Engine/Gui/Manager.h>

#include "Phobos/OgreEngine/OgreEngineAPI.h"

namespace Phobos
{
	namespace OgreEngine
	{
		namespace Gui
		{
			PH_DECLARE_NODE_PTR(Manager);

			class PH_OGRE_ENGINE_API Manager: public Engine::Gui::Manager, Ogre::RenderQueueListener
			{			
				public:				
					static Manager &CreateInstance(void);

					virtual ~Manager();

				protected:				
					virtual void OnStart() override; 	
					virtual void OnFinalize() override;

					virtual size_t GetScreenWidth();
					virtual size_t GetScreenHeight();

				private:
					virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);				

				private:
					Manager();

					void ConfigureRenderSystem();
					void BuildProjectionMatrix(Ogre::Matrix4& projection_matrix);
				
				private:				
					/**
						Here we need to have our own Scene and own camera to allow GUI rendering when no scenario is loaded

					*/
					Ogre::SceneManager	*m_pclSceneManager;
					Ogre::Camera		*m_pclCamera;
			};
		}
	}
}

#endif
