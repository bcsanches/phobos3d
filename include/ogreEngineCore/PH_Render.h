/*
Phobos 3d
April 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_RENDER_H
#define PH_RENDER_H

#include <list>

#include <OgrePrerequisites.h>
#include <OgreSceneManager.h>
#include <OgreTerrainGroup.h>
#include <OgreTerrainPrerequisites.h>

#include <PH_ContextCmd.h>
#include <PH_ContextVar.h>
#include <PH_CoreModule.h>

#include <Phobos/System/Window.h>

#include "PH_OgreEngineCoreAPI.h"

namespace Ogre
{
	namespace RTShader
	{
		class ShaderGenerator;
	}
}

namespace Phobos
{
	class Context_c;	
	class ShaderGeneratorTechniqueResolverListener;

	PH_DECLARE_NODE_PTR(Render);	

	namespace DefaultViewportZOrder
	{
		enum Enum
		{
			CONSOLE = -4096,
			GAME = 0,
			GUI = 4096
		};
	}

	class PH_OGRE_ENGINE_CORE_API Render_c: public CoreModule_c
	{		
		public:
			~Render_c();

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

			void ClearScene();

			void AddRenderQueueListener(Ogre::RenderQueueListener &listener);
			void RemoveRenderQueueListener(Ogre::RenderQueueListener &listener);
			
			//Ogre::SceneNode *GetOrCreateSceneNode(const String_c &name);
			Ogre::SceneNode *CreateSceneNode(const String_c &name);
			Ogre::SceneNode *CreateSceneNode();
			void DestroySceneNode(Ogre::SceneNode *node);
			Ogre::SceneNode *GetSceneNode(const String_c &name);

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

			Ogre::TerrainGroup *CreateTerrainGroup(Ogre::Terrain::Alignment align, UInt16_t terrainSize, Float_t terrainWorldSize);
			void DestroyTerrainGroup(Ogre::TerrainGroup *group);

			void SetSkyBox(bool enable, const String_c &materialName, Float_t distance = 5000, bool drawFirst = true, const Ogre::Quaternion &orientation = Ogre::Quaternion::IDENTITY);
			void SetAmbientColor(const Ogre::ColourValue &value);
			Ogre::ColourValue GetAmbientColor() const;

			void SetFog(Ogre::FogMode mode, Ogre::ColourValue color, Float_t density, Float_t min, Float_t max);

			Ogre::Viewport *AddViewport(Ogre::Camera *camera, int ZOrder = 0);
			void RemoveViewport(int ZOrder);

			const Ogre::RenderTarget::FrameStats &GetFrameStats();

			void LoadCaelum(const String_c &fileName);
			void UnloadCaelum();

			void SetShadowMode(Ogre::ShadowTechnique tech);

			void *GetWindowHandler();		

			size_t GetScreenWidth();
			size_t GetScreenHeight();

			void DrawLine(const Ogre::Vector3 &start, Ogre::Vector3 &end, Ogre::ColourValue colour);

		protected:
			// =====================================================
			// PROTECTED METHODS
			// =====================================================
			Render_c();			

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
			System::WindowPtr_t					ipWindow;
		
			Float_t								fpFrameTicks;
			Float_t								fpCurrentFPS;
			UInt_t								uFrameCount;

			ContextVar_c						varRScreenX;
			ContextVar_c						varRScreenY;
			ContextVar_c						varRVSync;
			ContextVar_c						varRFullScreen;
			ContextVar_c						varRRenderSystem;			

			ContextVar_c						varRShaderSystem;
			ContextVar_c						varRShaderSystemLibPath;

			ContextVar_c						varRCaelum;
			ContextVar_c						varParentWindow;

			ContextCmd_c						cmdOgreLoadPlugin;
			ContextCmd_c						cmdOgreAddResourceLocation;
			ContextCmd_c						cmdOgreInitialiseResourceGroup;
			ContextCmd_c						cmdScreenshot;

			ContextCmd_c						cmdSetShadowMode;
			ContextCmd_c						cmdSetShadowFarDistance;			

			std::unique_ptr<Ogre::Root>									upRoot;
			std::unique_ptr<ShaderGeneratorTechniqueResolverListener>	upShaderGeneratorTechiniqueResolverListener;
			Ogre::RenderWindow											*pclOgreWindow;
			Ogre::SceneManager											*pclMainSceneManager;
			Ogre::RTShader::ShaderGenerator								*pclShaderGenerator;

			Ogre::ShadowTechnique										eShadowMode;			

			typedef std::list<String_c>			StringList_t;
			StringList_t						lstPluginsName;

		public:		
			static Render_c &CreateInstance();
			static void ReleaseInstance();
			static Render_c &GetInstance();			

		private:
			// =====================================================
			// PRIVATE STATIC ATTRIBUTES
			// =====================================================		
			static RenderPtr_t ipInstance_gl;
	};
}

#endif