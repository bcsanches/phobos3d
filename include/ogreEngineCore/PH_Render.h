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

#include <Phobos/Shell/Command.h>
#include <Phobos/Shell/Variable.h>
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
	class Context;	
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

	class PH_OGRE_ENGINE_CORE_API Render: public CoreModule
	{		
		public:
			~Render();

			enum EntityMeshFlags_e
			{
				FORCE_EDGE_LIST_GENERATION = 0x01
			};

			// =====================================================
			// PUBLIC METHODS
			// =====================================================			
			void CreateDefaultCmds(Context &context);
			
			Ogre::SceneManager *CreateSceneManager(Ogre::SceneTypeMask typeMask);
			void DestroySceneManager(Ogre::SceneManager *manager);

			void ClearScene();

			void AddRenderQueueListener(Ogre::RenderQueueListener &listener);
			void RemoveRenderQueueListener(Ogre::RenderQueueListener &listener);
			
			//Ogre::SceneNode *GetOrCreateSceneNode(const String_t &name);
			Ogre::SceneNode *CreateSceneNode(const String_t &name);
			Ogre::SceneNode *CreateSceneNode();
			void DestroySceneNode(Ogre::SceneNode *node);
			Ogre::SceneNode *GetSceneNode(const String_t &name);

			Ogre::Entity	*CreateEntity(const String_t &meshName, UInt32_t flags = 0);
			Ogre::Entity	*CreateEntity(const String_t &entityName, const String_t &meshName, UInt32_t flags = 0);
			void DestroyEntity(Ogre::Entity *entity);

			Ogre::Light		*CreateLight();
			Ogre::Light		*CreateLight(const String_t &name);
			void DestroyLight(Ogre::Light *light);

			Ogre::Camera	*CreateCamera();
			Ogre::Camera	*CreateCamera(const String_t &name);
			void DestroyCamera(Ogre::Camera *camera);

			Ogre::ParticleSystem *CreateParticleSystem(const String_t &templateName);
			Ogre::ParticleSystem *CreateParticleSystem(const String_t &name, const String_t &templateName);
			void DestroyParticleSystem(Ogre::ParticleSystem *ps);

			Ogre::TerrainGroup *CreateTerrainGroup(Ogre::Terrain::Alignment align, UInt16_t terrainSize, Float_t terrainWorldSize);
			void DestroyTerrainGroup(Ogre::TerrainGroup *group);

			void SetSkyBox(bool enable, const String_t &materialName, Float_t distance = 5000, bool drawFirst = true, const Ogre::Quaternion &orientation = Ogre::Quaternion::IDENTITY);
			void SetAmbientColor(const Ogre::ColourValue &value);
			Ogre::ColourValue GetAmbientColor() const;

			void SetFog(Ogre::FogMode mode, Ogre::ColourValue color, Float_t density, Float_t min, Float_t max);

			Ogre::Viewport *AddViewport(Ogre::Camera *camera, int ZOrder = 0);
			void RemoveViewport(int ZOrder);

			const Ogre::RenderTarget::FrameStats &GetFrameStats();

			void LoadCaelum(const String_t &fileName);
			void UnloadCaelum();

			void SetShadowMode(Ogre::ShadowTechnique tech);

			void *GetWindowHandler();		

			size_t GetScreenWidth();
			size_t GetScreenHeight();

		protected:
			// =====================================================
			// PROTECTED METHODS
			// =====================================================
			Render();			

			virtual void OnBoot() override;			

			virtual void OnUpdate() override;

			virtual void OnPrepareToBoot() override;

		private:
			// =====================================================
			// PRIVATE METHODS
			// =====================================================			
			void CmdOgreLoadPlugin(const Shell::StringVector_t &args, Shell::Context &);
			void CmdOgreAddResourceLocation(const Shell::StringVector_t &args, Shell::Context &);
			void CmdOgreInitialiseResourceGroup(const Shell::StringVector_t &args, Shell::Context &);
			void CmdScreenshot(const Shell::StringVector_t &args, Shell::Context &);

			void CmdSetShadowMode(const Shell::StringVector_t &args, Shell::Context &);
			void CmdSetShadowFarDistance(const Shell::StringVector_t &args, Shell::Context &);

			void VarRCaelumChanged(const Shell::Variable &var, const String_t &currentValue, const String_t &newValue);

		private:
			// =====================================================
			// PRIVATE ATTRIBUTES
			// =====================================================		
			System::WindowPtr_t					m_ipWindow;
		
			Float_t								m_fpFrameTicks;
			Float_t								m_fpCurrentFPS;
			UInt_t								m_uFrameCount;

			Shell::Variable						m_varRScreenX;
			Shell::Variable						m_varRScreenY;
			Shell::Variable						m_varRVSync;
			Shell::Variable						m_varRFullScreen;
			Shell::Variable						m_varRRenderSystem;			

			Shell::Variable						m_varRShaderSystem;
			Shell::Variable						m_varRShaderSystemLibPath;

			Shell::Variable						m_varRCaelum;
			Shell::Variable						m_varParentWindow;

			Shell::Command						m_cmdOgreLoadPlugin;
			Shell::Command						m_cmdOgreAddResourceLocation;
			Shell::Command						m_cmdOgreInitialiseResourceGroup;
			Shell::Command						m_cmdScreenshot;

			Shell::Command						m_cmdSetShadowMode;
			Shell::Command						m_cmdSetShadowFarDistance;			

			std::unique_ptr<Ogre::Root>									m_upRoot;
			std::unique_ptr<ShaderGeneratorTechniqueResolverListener>	m_upShaderGeneratorTechiniqueResolverListener;
			Ogre::RenderWindow											*m_pclOgreWindow;
			Ogre::SceneManager											*m_pclMainSceneManager;
			Ogre::RTShader::ShaderGenerator								*m_pclShaderGenerator;

			Ogre::ShadowTechnique										m_eShadowMode;			

			typedef std::list<String_t>			StringList_t;
			StringList_t						m_lstPluginsName;

		public:		
			static Render &CreateInstance();
			static void ReleaseInstance();
			static Render &GetInstance();			

		private:
			// =====================================================
			// PRIVATE STATIC ATTRIBUTES
			// =====================================================		
			static RenderPtr_t ipInstance_gl;
	};
}

#endif