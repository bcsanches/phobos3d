/*
Phobos 3d
April 2010
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

#include "PH_Render.h"

#include <OgreEntity.h>
#include <OgreMaterialManager.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreRTShaderSystem.h>
#include <OgreShadowCameraSetup.h>
#include <OgreShadowCameraSetupPSSM.h>
#include <OgreSubEntity.h>

#include <PH_Console.h>
#include <PH_Context.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Enum.h>
#include <PH_EventManager.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_Memory.h>
#include <PH_Path.h>
#include <PH_String.h>

#include "PH_Core.h"
#include "PH_OgreUtils.h"

#define SHADER_SYSTEM_MATERIAL_GROUP "RTShaderSystemMaterialsGroup"

namespace Phobos
{
	RenderPtr_t Render_c::ipInstance_gl;

	class OgreLogListener_c: public Ogre::LogListener
	{
		public:
			virtual void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool& skipThisMessage )
			{
				Kernel_c::GetInstance().LogMessage("Ogre: " + message);
			}
	};

	static OgreLogListener_c clOgreLogListener_gl;
	
	class ShaderGeneratorTechniqueResolverListener : public Ogre::MaterialManager::Listener
	{
		public:
			ShaderGeneratorTechniqueResolverListener(Ogre::RTShader::ShaderGenerator &shaderGenerator):
			  rclShaderGenerator(shaderGenerator)
			{
				//empty
			}

			/** This is the hook point where shader based technique will be created.
			It will be called whenever the material manager won't find appropriate technique
			that satisfy the target scheme name. If the scheme name is out target RT Shader System
			scheme name we will try to create shader generated technique for it. 
			*/
			virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex, 
				const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex, 
				const Ogre::Renderable* rend)
			{	
				Ogre::Technique* generatedTech = NULL;

				// Case this is the default shader generator scheme.
				if (schemeName == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
				{
					bool techniqueCreated;

					// Create shader generated technique for this material.
					techniqueCreated = rclShaderGenerator.createShaderBasedTechnique(
						originalMaterial->getName(), 
						Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
						schemeName
					);	

					// Case technique registration succeeded.
					if (techniqueCreated)
					{
						// Force creating the shaders for the generated technique.
						rclShaderGenerator.validateMaterial(schemeName, originalMaterial->getName());
						
						// Grab the generated technique.
						Ogre::Material::TechniqueIterator itTech = originalMaterial->getTechniqueIterator();

						while (itTech.hasMoreElements())
						{
							Ogre::Technique* curTech = itTech.getNext();

							if (curTech->getSchemeName() == schemeName)
							{
								generatedTech = curTech;
								break;
							}
						}				
					}
				}

				return generatedTech;
			}

		private:	
			Ogre::RTShader::ShaderGenerator &rclShaderGenerator;
	};

	Render_c &Render_c::CreateInstance()
	{
		PH_ASSERT(ipInstance_gl == NULL);

		ipInstance_gl.reset(PH_NEW Render_c());

		return *ipInstance_gl;
	}

	void Render_c::ReleaseInstance()
	{
		ipInstance_gl.reset();
	}

	Render_c &Render_c::GetInstance()
	{
		PH_ASSERT_VALID(ipInstance_gl);

		return(*ipInstance_gl);
	}

	Render_c::Render_c(void):
		CoreModule_c("Render"),
		varRScreenX("dvRScreenX", "800"),
		varRScreenY("dvRScreenY", "600"),
		varRVSync("dvRVSync", "1"),
		varRFullScreen("dvRFullScreen", "0"),
		varRRenderSystem("dvRRenderSystem", "Direct3D9"),
		varRShaderSystem("dvRShaderSystem", "true"),
		varRShaderSystemLibPath("dvRShaderSystemLibPath", "resources/RTShaderLib"),
		varRCaelum("dvRCaelum", "1"),		
		cmdOgreLoadPlugin("ogreLoadPlugin"),
		cmdOgreAddResourceLocation("ogreAddResourceLocation"),
		cmdOgreInitialiseResourceGroup("ogreInitialiseResourceGroup"),
		cmdScreenshot("screenshot"),
		cmdSetShadowMode("setShadowMode"),
		cmdSetShadowFarDistance("setShadowFarDistance"),
		pclOgreWindow(NULL),
		pclMainSceneManager(NULL),
		pclShaderGenerator(NULL),
		eShadowMode(Ogre::SHADOWTYPE_NONE)
	{
		Kernel_c &kernel = Kernel_c::GetInstance();

		kernel.LogMessage("[Render] Initializing");

		cmdOgreLoadPlugin.SetProc(PH_CONTEXT_CMD_BIND(&Render_c::CmdOgreLoadPlugin, this));
		cmdOgreAddResourceLocation.SetProc(PH_CONTEXT_CMD_BIND(&Render_c::CmdOgreAddResourceLocation, this));
		cmdOgreInitialiseResourceGroup.SetProc(PH_CONTEXT_CMD_BIND(&Render_c::CmdOgreInitialiseResourceGroup, this));
		cmdScreenshot.SetProc(PH_CONTEXT_CMD_BIND(&Render_c::CmdScreenshot, this));

		cmdSetShadowMode.SetProc(PH_CONTEXT_CMD_BIND(&Render_c::CmdSetShadowMode, this));
		cmdSetShadowFarDistance.SetProc(PH_CONTEXT_CMD_BIND(&Render_c::CmdSetShadowFarDistance, this));

		//varRCaelum.SetCallback(VarCallback_t(this, &Render_c::VarRCaelumChanged));

		kernel.LogMessage("[Render] Initializing Ogre");
		spRoot.reset(new Ogre::Root("", ""));
		Ogre::Log *log = Ogre::LogManager::getSingleton().getDefaultLog();
		log->setDebugOutputEnabled(false);
		log->addListener(&clOgreLogListener_gl);

		kernel.LogMessage("[Render] Initialized.");
	}

	Render_c::~Render_c(void)
	{
		//we must make sure that caelum is destroyed before ogre :(
		//clCaelum.Shutdown();

		if(ipWindow)
		{
			//EventManager_c::GetInstance()->RemoveListener(*(ipWindow.get()));
		}
	}

	size_t Render_c::GetScreenWidth()
	{
		return ipWindow->GetWidth();
	}
	
	size_t Render_c::GetScreenHeight()
	{
		return ipWindow->GetHeight();
	}

	void *Render_c::GetWindowHandler()
	{
		PH_ASSERT(ipWindow);

		return ipWindow->GetHandler();
	}

	void Render_c::OnBoot(void)
	{
		Kernel_c	&kernel(Kernel_c::GetInstance());
		kernel.LogMessage("[Render_c::OnBoot] Starting");

		ipWindow = Window_c::Create("RenderWindow");

		Rect_s<UInt_t> r;

		r.tOrigin[0] = 0;
		r.tOrigin[1] = 0;
		r.tWidth = varRScreenX.GetInt();
		r.tHeight = varRScreenY.GetInt();
		bool fullScreen = varRFullScreen.GetBoolean();
		bool vsync = varRVSync.GetBoolean();

		kernel.LogMessage("[Render_c::OnBoot] Opening render window");
		ipWindow->Open("Phobos Engine", r);

		//We need to do a "lazy load" with Ogre plugins, to make sure the render plugins are only 
		//loaded after screen is created.

		//Loading plugin befire creating the screen, may cause problem in some platforms: ie linux opengl plugin
		for(StringList_t::iterator it = lstPluginsName.begin(), end = lstPluginsName.end(); it != end; ++it)
			spRoot->loadPlugin(*it);

		lstPluginsName.clear();		

		const Ogre::RenderSystemList &renderSystems = (spRoot->getAvailableRenderers());
		Ogre::RenderSystemList::const_iterator r_it, end = renderSystems.end();

		kernel.LogMessage("[Render_c::OnBoot] Searching render system");
		bool foundRenderSystem = false;

		for(r_it = renderSystems.begin(); r_it != end; ++r_it)
		{
			const std::string &name((*r_it)->getName());
			kernel.LogMessage("\tFound: " + name);

			if(name.find(varRRenderSystem.GetValue()) >= 0)
			{
				spRoot->setRenderSystem(*r_it);
				foundRenderSystem = true;
				break;
			}
		}

		if(!foundRenderSystem)
		{
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Render_c::OnBoot", "Render system " + varRRenderSystem.GetValue() + " not available");
		}

		kernel.LogMessage("[Render_c::OnBoot] render system found, initializing Ogre");
                spRoot->restoreConfig();
                spRoot->initialise(false);

		Ogre::NameValuePairList opts;

		std::stringstream tmp;
		tmp << r.tWidth << 'x' << r.tHeight;
		opts["resolution"] = tmp.str();
		opts["fullscreen"] = fullScreen ? "true" : "false";
		opts["vsync"] = vsync ? "true" : "false";
                
        void* handler = ipWindow->GetHandler();

        if (handler != NULL)
            opts["externalWindowHandle"] = PointerToString(handler);

        if (ipWindow->HasGLContext())
            opts["currentGLContext"] = "true";

		kernel.LogMessage("[Render_c::OnBoot] Creating ogre window");
		pclOgreWindow = spRoot->createRenderWindow("PhobosMainWindow", r.tWidth, r.tHeight, fullScreen, &opts);

		pclOgreWindow->setVisible(true);
		
		kernel.LogMessage("[Render_c::OnBoot] Creating SceneManager");
		pclMainSceneManager = spRoot->createSceneManager(Ogre::ST_GENERIC);
		
		pclMainSceneManager->setShadowFarDistance(100);

		if(varRShaderSystem.GetBoolean())
		{
			kernel.LogMessage("[Render_c::OnBoot] Initializing ShaderSystem.");

			Ogre::RTShader::ShaderGenerator::initialize();

			pclShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
			pclShaderGenerator->addSceneManager(pclMainSceneManager);		

			Ogre::ResourceGroupManager &resourceGroupManager = Ogre::ResourceGroupManager::getSingleton();

			resourceGroupManager.createResourceGroup(SHADER_SYSTEM_MATERIAL_GROUP, false);
			resourceGroupManager.addResourceLocation(varRShaderSystemLibPath.GetValue(), "FileSystem", SHADER_SYSTEM_MATERIAL_GROUP, true);
			resourceGroupManager.initialiseResourceGroup(SHADER_SYSTEM_MATERIAL_GROUP);
			resourceGroupManager.loadResourceGroup(SHADER_SYSTEM_MATERIAL_GROUP);
		}				

		this->SetShadowMode(eShadowMode);

		kernel.LogMessage("[Render_c::OnBoot] Initializing all resource groups");
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

		kernel.LogMessage("[Render_c::OnBoot] Ready.");
		Core_c::GetInstance().OnEvent(CoreEvents::RENDER_READY);
	}

	void Render_c::OnUpdate(void)
	{
		if(!ipWindow)
			return;
		
		//Need to clear pending dirty hash because of multiple scene managers
		//http://www.ogre3d.org/forums/viewtopic.php?p=189032#189032
		if(!Ogre::Pass::getDirtyHashList().empty() || !Ogre::Pass::getPassGraveyard().empty())
		{
			Ogre::SceneManagerEnumerator::SceneManagerIterator scenesIter = spRoot->getSceneManagerIterator();
  
			while(scenesIter.hasMoreElements())
			{
				Ogre::SceneManager* pScene = scenesIter.getNext();
				if(pScene)
				{
					Ogre::RenderQueue* pQueue = pScene->getRenderQueue();
					if(pQueue)
					{
						Ogre::RenderQueue::QueueGroupIterator groupIter = pQueue->_getQueueGroupIterator();
						while(groupIter.hasMoreElements())
						{
							Ogre::RenderQueueGroup* pGroup = groupIter.getNext();
							if(pGroup)
								pGroup->clear(false);
						}
					}
				}
			}
  
			// Now trigger the pending pass updates
			Ogre::Pass::processPendingPassUpdates();
		}		

		//clAnimationManager.Update();
		//clCaelum.Update(IM_GetGameTimer().fRenderFrameTime);

            spRoot->renderOneFrame();
	}

	void Render_c::OnPrepareToBoot()
	{
		Console_c &console = Console_c::GetInstance();

		console.AddContextCmd(cmdOgreAddResourceLocation);
		console.AddContextCmd(cmdOgreInitialiseResourceGroup);
		console.AddContextCmd(cmdOgreLoadPlugin);
		console.AddContextCmd(cmdScreenshot);

		console.AddContextCmd(cmdSetShadowMode);
		console.AddContextCmd(cmdSetShadowFarDistance);

		console.AddContextVar(varRScreenX);
		console.AddContextVar(varRScreenY);
		console.AddContextVar(varRFullScreen);
		console.AddContextVar(varRVSync);
		console.AddContextVar(varRRenderSystem);
		console.AddContextVar(varRCaelum);

		console.AddContextVar(varRShaderSystem);
		console.AddContextVar(varRShaderSystemLibPath);
	}

	//
	//
	//RENDER FUNCTIONS
	//
	//

	Ogre::SceneManager *Render_c::CreateSceneManager(Ogre::SceneTypeMask typeMask)
	{
		return spRoot->createSceneManager(typeMask);
	}

	void Render_c::DestroySceneManager(Ogre::SceneManager *manager)
	{
		spRoot->destroySceneManager(manager);
	}

	void Render_c::ClearScene()
	{
		pclMainSceneManager->clearScene();
	}

	void Render_c::AddRenderQueueListener(Ogre::RenderQueueListener &listener)
	{
		pclMainSceneManager->addRenderQueueListener(&listener);
	}

	void Render_c::RemoveRenderQueueListener(Ogre::RenderQueueListener &listener)
	{
		pclMainSceneManager->removeRenderQueueListener(&listener);
	}

	Ogre::SceneNode *Render_c::GetSceneNode(const String_c &name)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return pclMainSceneManager->getSceneNode(name);
	}

	Ogre::SceneNode *Render_c::CreateSceneNode(const String_c &name)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return pclMainSceneManager->getRootSceneNode()->createChildSceneNode(name);
	}

	Ogre::SceneNode *Render_c::CreateSceneNode()
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return pclMainSceneManager->getRootSceneNode()->createChildSceneNode();
	}

	void Render_c::DestroySceneNode(Ogre::SceneNode *node)
	{
		PH_ASSERT_VALID(pclMainSceneManager);		

		pclMainSceneManager->destroySceneNode(node->getName());
	}

	Ogre::Entity* Render_c::CreateEntity(const String_c &meshName, UInt32_t flags)
	{
		String_c tmpName;
		GenerateOgreName(tmpName);

		return this->CreateEntity(tmpName, meshName, flags);
	}

	Ogre::Entity *Render_c::CreateEntity(const String_c &entityName, const String_c &meshName, UInt32_t flags)
	{
		String_c tmpPath;		
		String_c extension;

		const String_c *meshNameToUse = &meshName;

		//Newer ogitor does nto include .mesh in mesh names, so fix it here
		bool found = Path_c::GetExtension(extension, meshName);		 
		if((found && extension != "mesh") || (!found))
		{
			tmpPath = meshName;
			tmpPath.append(".mesh");
			meshNameToUse = &tmpPath;
		}

		Ogre::Entity *ent = pclMainSceneManager->createEntity(entityName, *meshNameToUse);

		if(ent == NULL)
			return NULL;

		flags |= FORCE_EDGE_LIST_GENERATION;
		
		Ogre::MeshPtr mesh = ent->getMesh();
		if((flags & FORCE_EDGE_LIST_GENERATION) && (!mesh->isEdgeListBuilt()))
		{
			mesh->buildEdgeList();
		}

		return (ent);
	}

	void Render_c::DestroyEntity(Ogre::Entity *entity)
	{
		PH_ASSERT_VALID(pclMainSceneManager);
		PH_ASSERT_VALID(entity);

		pclMainSceneManager->destroyEntity(entity);
	}

	Ogre::Light *Render_c::CreateLight()
	{
		String_c str;
		GenerateOgreName(str);

		return this->CreateLight(str);
	}

	Ogre::Light *Render_c::CreateLight(const String_c &name)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return(pclMainSceneManager->createLight(name));
	}

	void Render_c::DestroyLight(Ogre::Light *light)
	{
		PH_ASSERT_VALID(pclMainSceneManager);
		PH_ASSERT_VALID(light);

		pclMainSceneManager->destroyLight(light);
	}

	Ogre::Camera *Render_c::CreateCamera()
	{
		String_c tmp;

		GenerateOgreName(tmp);

		return this->CreateCamera(tmp);
	}

	Ogre::Camera *Render_c::CreateCamera(const String_c &name)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return(pclMainSceneManager->createCamera(name));
	}

	void Render_c::DestroyCamera(Ogre::Camera *camera)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		pclMainSceneManager->destroyCamera(camera);
	}

	Ogre::ParticleSystem *Render_c::CreateParticleSystem(const String_c &templateName)
	{
		String_c name;
		GenerateOgreName(name);

		return this->CreateParticleSystem(name, templateName);
	}

	Ogre::ParticleSystem *Render_c::CreateParticleSystem(const String_c &name, const String_c &templateName)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return pclMainSceneManager->createParticleSystem(name, templateName);
	}

	void Render_c::DestroyParticleSystem(Ogre::ParticleSystem *ps)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		pclMainSceneManager->destroyParticleSystem(ps);
	}

	Ogre::TerrainGroup *Render_c::CreateTerrainGroup(Ogre::Terrain::Alignment align, UInt16_t terrainSize, Float_t terrainWorldSize)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return new Ogre::TerrainGroup(pclMainSceneManager, align, terrainSize, terrainWorldSize);
	}

	void Render_c::DestroyTerrainGroup(Ogre::TerrainGroup *group)
	{
		delete group;
	}

	void Render_c::SetSkyBox(bool enable, const String_c &materialName, Float_t distance, bool drawFirst, const Ogre::Quaternion& orientation)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		pclMainSceneManager->setSkyBox(enable, materialName, distance, drawFirst, orientation);
	}

	void Render_c::SetAmbientColor(const Ogre::ColourValue &value)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		pclMainSceneManager->setAmbientLight(value);		
	}

	Ogre::ColourValue Render_c::GetAmbientColor() const
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return pclMainSceneManager->getAmbientLight();
	}

	void Render_c::SetFog(Ogre::FogMode mode, Ogre::ColourValue color, Float_t density, Float_t min, Float_t max)
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		pclMainSceneManager->setFog(mode, color, density, min, max);
	}

	Ogre::Viewport *Render_c::AddViewport(Ogre::Camera *camera, int ZOrder)
	{
		PH_ASSERT_VALID(pclOgreWindow);

		Ogre::Viewport *viewport = pclOgreWindow->addViewport(camera, ZOrder);

		if(varRShaderSystem.GetBoolean())
			viewport->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

		return(viewport);
	}

	void Render_c::RemoveViewport(int ZOrder)
	{
		PH_ASSERT_VALID(pclOgreWindow);

		pclOgreWindow->removeViewport(ZOrder);
	}

	const Ogre::RenderTarget::FrameStats &Render_c::GetFrameStats()
	{
		PH_ASSERT_VALID(ipWindow);

		return pclOgreWindow->getStatistics();
	}

	/*
	IM_ErrorHandler_t Render_c::LoadCaelum(const String_c &fileName)
	{
		PH_ASSERT_VALID(pclMainSceneManager);
		PH_ASSERT_VALID(pclOgreWindow);

		if(!varRCaelum.GetBoolean())
			return IM_SUCCESS;

		if(IM_ErrorHandler_t e = clCaelum.Load(fileName, *pclMainSceneManager) != IM_SUCCESS)
			return e;

		clCaelum.Listen(*pclOgreWindow);

		return IM_SUCCESS;
	}

	void Render_c::UnloadCaelum()
	{
		clCaelum.StopListening(*pclOgreWindow);

		clCaelum.Unload();
	}
	*/

	//
	//
	// SHADOW MANAGEMENT
	//
	//

	struct ShadowType_s
	{
		const char				*pstrzName;
		Ogre::ShadowTechnique	eValue;
	};

	static ShadowType_s stShadowTypes_gl[] =
	{
		{"none", Ogre::SHADOWTYPE_NONE},
		{"stencil_additive", Ogre::SHADOWTYPE_STENCIL_ADDITIVE},
		{"stencil_modulative", Ogre::SHADOWTYPE_STENCIL_MODULATIVE},
		{"texture_modulative", Ogre::SHADOWTYPE_TEXTURE_MODULATIVE},
		{"texture_additive", Ogre::SHADOWTYPE_TEXTURE_ADDITIVE},
		{"texture_modulative_integrated", Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED}
	};

	void Render_c::CmdSetShadowMode(const StringVector_t &args, Context_c &)
	{
		static Enum_c<Ogre::ShadowTechnique, ShadowType_s> clShadowType_gl(stShadowTypes_gl);

		if(args.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[Render_c::CmdSetShadowMode] Missing parameter type, usage: setShadowMode [none|texture_modulative_integrated]");

			return;
		}

		Ogre::ShadowTechnique tech;

		if(!clShadowType_gl.TryGetValue(tech,args[1]))
		{
			Kernel_c::GetInstance().LogMessage("[Render_c::CmdSetShadowMode] Parameter " + args[1] + " is invalid");

			return;
		}

		if(tech == eShadowMode)
			return;

		if(eShadowMode != tech)
		{
			eShadowMode = tech;

			if(pclMainSceneManager == NULL)
				return;

			this->SetShadowMode(tech);
		}
	}

	void Render_c::CmdSetShadowFarDistance(const StringVector_t &args, Context_c &)
	{
		if(args.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[Render_c::CmdSetShadowFarDistance] Missing distance parameter, usage: setShadowFarDistance <value>");
		}
		else
		{
			pclMainSceneManager->setShadowFarDistance(StringToFloat(args[1]));
		}
	}

	void Render_c::SetShadowMode(Ogre::ShadowTechnique tech)
	{
		pclMainSceneManager->setShadowTechnique(tech);

		if(varRShaderSystem.GetBoolean())
		{
			Ogre::RTShader::RenderState *schemRenderState = pclShaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

			if(tech != Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED)
			{
				const Ogre::RTShader::SubRenderStateList& subRenderStateList = schemRenderState->getTemplateSubRenderStateList();
				Ogre::RTShader::SubRenderStateListConstIterator it = subRenderStateList.begin();
				Ogre::RTShader::SubRenderStateListConstIterator itEnd = subRenderStateList.end();

				for (; it != itEnd; ++it)
				{
					Ogre::RTShader::SubRenderState* curSubRenderState = *it;

					// This is the pssm3 sub render state -> remove it.
					if (curSubRenderState->getType() == Ogre::RTShader::IntegratedPSSM3::Type)
					{
						schemRenderState->removeTemplateSubRenderState(*it);
						break;
					}
				}
			}

			if((tech == Ogre::SHADOWTYPE_NONE) || (tech == Ogre::SHADOWTYPE_STENCIL_ADDITIVE) || (tech == Ogre::SHADOWTYPE_STENCIL_MODULATIVE))
			{			
				//empty
			}
			else if((tech == Ogre::SHADOWTYPE_TEXTURE_ADDITIVE) || (tech == Ogre::SHADOWTYPE_TEXTURE_MODULATIVE))
			{
				pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 1);
				pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 6);
				pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 3);

				pclMainSceneManager->setShadowTextureSettings(512, 18, Ogre::PF_FLOAT32_R);
				pclMainSceneManager->setShadowTextureSelfShadow(true);
				pclMainSceneManager->setShadowColour(Ogre::ColourValue(0.1f, 0.5f, 0.1f));
				pclMainSceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(new Ogre::DefaultShadowCameraSetup()));
			}
			else if(tech == Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED)
			{
				pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
				pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 6);
				pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 3);

				pclMainSceneManager->setShadowTextureSettings(512, 18, Ogre::PF_FLOAT32_R);
				pclMainSceneManager->setShadowTextureSelfShadow(true);

				pclMainSceneManager->setShadowTextureCasterMaterial("PSSM/shadow_caster");

				// Disable fog on the caster pass.
				Ogre::MaterialPtr passCaterMaterial = Ogre::MaterialManager::getSingleton().getByName("PSSM/shadow_caster");
				Ogre::Pass* pssmCasterPass = passCaterMaterial->getTechnique(0)->getPass(0);
				pssmCasterPass->setFog(true);

				// shadow camera setup
				Ogre::PSSMShadowCameraSetup *pssmSetup = new Ogre::PSSMShadowCameraSetup();
				pssmSetup->calculateSplitPoints(3, 0.1f, 100);
				pssmSetup->setSplitPadding(0.01f);
				pssmSetup->setOptimalAdjustFactor(0, 0.01f);
				pssmSetup->setOptimalAdjustFactor(1, 0.01f);
				pssmSetup->setOptimalAdjustFactor(2, 0.01f);

				pclMainSceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(pssmSetup));
		
				Ogre::RTShader::SubRenderState* subRenderState = pclShaderGenerator->createSubRenderState(Ogre::RTShader::IntegratedPSSM3::Type);	
				Ogre::RTShader::IntegratedPSSM3* pssm3SubRenderState = static_cast<Ogre::RTShader::IntegratedPSSM3*>(subRenderState);
				const Ogre::PSSMShadowCameraSetup::SplitPointList& srcSplitPoints = pssmSetup->getSplitPoints();
				Ogre::RTShader::IntegratedPSSM3::SplitPointList dstSplitPoints;

				for (unsigned int i=0; i < srcSplitPoints.size(); ++i)
				{
					dstSplitPoints.push_back(srcSplitPoints[i]);
				}

				pssm3SubRenderState->setSplitPoints(dstSplitPoints);
				schemRenderState->addTemplateSubRenderState(subRenderState);
			}
		}
	}

	/*
	IM_Bool_t Render_c::VarRCaelumChanged(const IM_ContextVar_c &var, const String_c &currentValue, const String_c &newValue)
	{
		//caleum turned off?
		if(!IM_StringToBoolean(newValue))
			this->UnloadCaelum();
		else
		{
			IM_KernelPrintf(IM_KERNEL_LOG_NORMAL_FLAGS, "[Render_c::VarRCaelumChanged] Caelum will only be show when it is reloaded");
		}

		return IM_TRUE;
	}
	*/

	//
	//
	//RESOURCE COMMANDS
	//
	//

	void Render_c::CmdOgreAddResourceLocation(const StringVector_t &args, Context_c &)
	{
		size_t sz = args.size();
		if(sz < 4)
		{
			Kernel_c::GetInstance().LogMessage("[Render_c::CmdOgreAddResourceLocation] ERROR: insuficient parameters, usage: ogreAddResourceLocation <name> <type> <group> [<recursive>]");

			return;
		}

		Ogre::ResourceGroupManager::getSingleton().addResourceLocation(args[1], args[2], args[3], (sz == 5) ? true : false);
	}

	void Render_c::CmdOgreInitialiseResourceGroup(const StringVector_t &args, Context_c &)
	{
		const size_t sz = args.size();
		if(sz < 2)
		{
			Kernel_c::GetInstance().LogMessage("[Render_c::CmdOgreInitializeResourceGroup] ERROR: insuficient parameters, usage: ogreInitializeResourceGroup <group0> [<group1> <groupN>]");

			return;
		}

		using namespace Ogre;

		ResourceGroupManager &manager = ResourceGroupManager::getSingleton();

		for(size_t i = 1; i < sz; ++i)
		{
			try
			{
				const String_c &name = args[i];

				if(manager.resourceGroupExists(name) && manager.isResourceGroupInitialised(name))
				{
					Kernel_c::GetInstance().LogMessage("[Render_c::CmdOgreInitialiseResourceGroup] Warning: ResourceGroup " + name + "is being initialised again, clearing it first");
					manager.clearResourceGroup(name);
				}

				manager.initialiseResourceGroup(name);
			}
			catch(Ogre::Exception &e)
			{
				LogOgreException("Render_c::CmdOgreInitialiseResourceGroup", e);
			}
		}
	}

	//
	//
	//MISC COMMANDS
	//
	//

	void Render_c::CmdOgreLoadPlugin(const StringVector_t &container, Context_c &)
	{
		if(container.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[Render_c::CmdOgreLoadPlugin] ERROR: insuficient parameters, usage: ogreLoadPlugin <pluginName>");

			return;
		}

		lstPluginsName.push_back(container[1]);		
	}

	void Render_c::CmdScreenshot(const StringVector_t &container, Context_c &)
	{
		if(pclOgreWindow)
		{
			try
			{
				Ogre::String str(pclOgreWindow->writeContentsToTimestampedFile("screenshots/screen", ".jpg"));

				Kernel_c::GetInstance().LogMessage("Created " + str + "");
			}
			catch(Ogre::Exception &e)
			{
				LogOgreException("Render_c::CmdScreenshot", e);
			}
		}
	}
}
