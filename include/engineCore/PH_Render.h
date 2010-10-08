/*
Phobos 3d
  April 2010

  Copyright (C) 2005-2010 Bruno Crivelari Sanches

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

#ifndef PH_RENDER_H
#define PH_RENDER_H

#include <boost/scoped_ptr.hpp>

#include <OgrePrerequisites.h>
#include <OgreSceneManager.h>

#include <PH_ContextCmd.h>
#include <PH_ContextVar.h>
#include <PH_Window.h>

#include "PH_CoreModule.h"

namespace Phobos
{
	class Context_c;
	class Render_c;

	typedef boost::intrusive_ptr<Render_c> RenderPtr_t;

	class PH_ENGINE_CORE_API Render_c: public CoreModule_c
	{		
		public:
			enum EntityMeshFlags_e
			{
				FORCE_EDGE_LIST_GENERATION = 0x01
			};

			// =====================================================
			// PUBLIC METHODS
			// =====================================================			
			void CreateDefaultCmds(Context_c &context);
			
			Ogre::SceneManager *CreateSceneManager(Ogre::SceneTypeMask typeMask);
			void DestroySceneManager(Ogre::SceneManager *manager);

			Ogre::SceneNode *CreateSceneNode(void);
			void DestroySceneNode(Ogre::SceneNode *node);

			Ogre::Entity	*CreateEntity(const String_c &meshName, UInt32_t flags = 0);
			Ogre::Entity	*CreateEntity(const String_c &entityName, const String_c &meshName, UInt32_t flags = 0);
			void DestroyEntity(Ogre::Entity *entity);

			Ogre::Light		*CreateLight();
			Ogre::Light		*CreateLight(const String_c &name);
			void DestroyLight(Ogre::Light *light);

			Ogre::Camera	*CreateCamera();
			Ogre::Camera	*CreateCamera(const String_c &name);
			void DestroyCamera(Ogre::Camera *camera);

			Ogre::ParticleSystem *CreateParticleSystem(const String_c &templateName);
			Ogre::ParticleSystem *CreateParticleSystem(const String_c &name, const String_c &templateName);
			void DestroyParticleSystem(Ogre::ParticleSystem *ps);

			void SetSkyBox(bool enable, const String_c &materialName, Float_t distance = 5000, bool drawFirst = true, const Ogre::Quaternion &orientation = Ogre::Quaternion::IDENTITY);
			void SetAmbientColor(const Ogre::ColourValue &value);

			Ogre::Viewport *AddViewport(Ogre::Camera *camera, int ZOrder = 0);
			void RemoveViewport(int ZOrder);

			const Ogre::RenderTarget::FrameStats &GetFrameStats();

			void LoadCaelum(const String_c &fileName);
			void UnloadCaelum();

			void SetShadowMode(Ogre::ShadowTechnique tech);

			void *GetWindowHandler();

		protected:
			// =====================================================
			// PROTECTED METHODS
			// =====================================================
			Render_c();
			~Render_c();

			void OnBoot();
			void OnInit();

			void OnUpdate();

			void OnPrepareToBoot();

		private:
			// =====================================================
			// PRIVATE METHODS
			// =====================================================			
			void CmdOgreLoadPlugin(const StringVector_t &args, Context_c &);
			void CmdOgreAddResourceLocation(const StringVector_t &args, Context_c &);
			void CmdOgreInitialiseResourceGroup(const StringVector_t &args, Context_c &);
			void CmdScreenshot(const StringVector_t &args, Context_c &);

			void CmdSetShadowMode(const StringVector_t &args, Context_c &);
			void CmdSetShadowFarDistance(const StringVector_t &args, Context_c &);

			void VarRCaelumChanged(const ContextVar_c &var, const String_c &currentValue, const String_c &newValue);

		private:
			// =====================================================
			// PRIVATE ATTRIBUTES
			// =====================================================		
			WindowPtr_t							ipWindow;
		
			Float_t								fpFrameTicks;
			Float_t								fpCurrentFPS;
			UInt_t								uFrameCount;

			ContextVar_c						varRScreenX;
			ContextVar_c						varRScreenY;
			ContextVar_c						varRVSync;
			ContextVar_c						varRFullScreen;
			ContextVar_c						varRRenderSystem;

			ContextVar_c						varRCaelum;

			ContextCmd_c						cmdOgreLoadPlugin;
			ContextCmd_c						cmdOgreAddResourceLocation;
			ContextCmd_c						cmdOgreInitialiseResourceGroup;
			ContextCmd_c						cmdScreenshot;

			ContextCmd_c						cmdSetShadowMode;
			ContextCmd_c						cmdSetShadowFarDistance;			

			boost::scoped_ptr<Ogre::Root>		spRoot;			
			Ogre::RenderWindow					*pclOgreWindow;
			Ogre::SceneManager					*pclMainSceneManager;

		public:		
			static RenderPtr_t CreateInstance();
			static void ReleaseInstance();
			static RenderPtr_t GetInstance();			

		private:
			// =====================================================
			// PRIVATE STATIC ATTRIBUTES
			// =====================================================		
			static RenderPtr_t ipInstance_gl;
	};
}

#endif