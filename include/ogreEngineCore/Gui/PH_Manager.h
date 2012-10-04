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

#ifndef PH_GUI_MANAGER_H
#define PH_GUI_MANAGER_H

#include <list>

#include <OgreRenderQueueListener.h>

#include <PH_ContextCmd.h>
#include <PH_Singleton.h>

#include "PH_CoreModule.h"
#include "PH_OgreEngineCoreAPI.h"

#include "PH_ContextFwd.h"

namespace Phobos
{
	namespace Gui
	{
		PH_DECLARE_NODE_PTR(Manager);

		class PH_OGRE_ENGINE_CORE_API Manager_c: public CoreModule_c, Ogre::RenderQueueListener
		{
			PH_DECLARE_SINGLETON_METHODS(Manager);

			public:
				ContextPtr_t CreateContext(const String_c &name);

				~Manager_c();

			protected:
				virtual void OnPrepareToBoot();
				virtual void OnRenderReady();
				virtual void OnUpdate();
				virtual void OnFinalize();

			private:
				virtual void renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool& skipThisInvocation);				

			private:
				Manager_c();

				void ConfigureRenderSystem();
				void BuildProjectionMatrix(Ogre::Matrix4& projection_matrix);

				void CmdRocketLoadFonfFace(const StringVector_t &container, Phobos::Context_c &);

			private:
				std::list<String_c> lstFontFacesToLoad;

				ContextCmd_c		cmdRocketLoadFontFace;

				/**
					Here we need to have our own Scene and own camera to allow GUI rendering when no scenario is loaded

				*/
				Ogre::SceneManager	*pclSceneManager;
				Ogre::Camera		*pclCamera;
		};
	}
}

#endif
