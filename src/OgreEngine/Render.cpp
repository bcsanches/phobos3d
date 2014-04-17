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

#include "Phobos/OgreEngine/Render.h"

#include <OgreEntity.h>
#include <OgreLog.h>
#include <OgreLogManager.h>
#include <OgreMaterialManager.h>
#include <OgreMesh.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>
#include <OgreRTShaderSystem.h>
#include <OgreSceneNode.h>
#include <OgreShadowCameraSetup.h>
#include <OgreShadowCameraSetupPSSM.h>
#include <OgreSubEntity.h>
#include <OgreTechnique.h>
#include <OgreViewport.h>

#include <Phobos/Engine/Console.h>
#include <Phobos/Engine/Core.h>
#include <Phobos/Shell/Context.h>
#include <Phobos/Shell/Utils.h>
#include <Phobos/Error.h>
#include <Phobos/Enum.h>
#include <Phobos/Exception.h>
#include <Phobos/Memory.h>
#include <Phobos/Path.h>
#include <Phobos/String.h>

#include <Phobos/System/EventManager.h>

#include "Phobos/OgreEngine/Utils.h"

#define SHADER_SYSTEM_MATERIAL_GROUP "RTShaderSystemMaterialsGroup"

namespace
{
	class OgreLogListener_c: public Ogre::LogListener
	{
		public:
			virtual void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String &logName, bool& skipThisMessage )
			{
				Phobos::LogMessage("Ogre: " + message);
			}
	};

	static OgreLogListener_c clOgreLogListener_gl;	
}

namespace Phobos
{
	namespace OgreEngine
	{
		class ShaderGeneratorTechniqueResolverListener : public Ogre::MaterialManager::Listener
		{
			public:
				ShaderGeneratorTechniqueResolverListener(Ogre::RTShader::ShaderGenerator &shaderGenerator):
				  m_rclShaderGenerator(shaderGenerator)
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
						techniqueCreated = m_rclShaderGenerator.createShaderBasedTechnique(
							originalMaterial->getName(), 
							Ogre::MaterialManager::DEFAULT_SCHEME_NAME, 
							schemeName
						);	

						// Case technique registration succeeded.
						if (techniqueCreated)
						{
							// Force creating the shaders for the generated technique.
							m_rclShaderGenerator.validateMaterial(schemeName, originalMaterial->getName());
						
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
				Ogre::RTShader::ShaderGenerator &m_rclShaderGenerator;
		};
	}
}

Phobos::OgreEngine::RenderPtr_t Phobos::OgreEngine::Render::ipInstance_gl;

Phobos::OgreEngine::Render &Phobos::OgreEngine::Render::CreateInstance()
{
	PH_ASSERT(ipInstance_gl == NULL);

	ipInstance_gl.reset(PH_NEW Render());

	return *ipInstance_gl;
}

void Phobos::OgreEngine::Render::ReleaseInstance()
{
	ipInstance_gl.reset();
}

Phobos::OgreEngine::Render &Phobos::OgreEngine::Render::GetInstance()
{
	PH_ASSERT_VALID(ipInstance_gl);

	return(*ipInstance_gl);
}

Phobos::OgreEngine::Render::Render(void):
	Module("Render"),
	m_varRScreenX("dvRScreenX", "800"),
	m_varRScreenY("dvRScreenY", "600"),
	m_varRVSync("dvRVSync", "1"),
	m_varRFullScreen("dvRFullScreen", "0"),
	m_varRRenderSystem("dvRRenderSystem", "Direct3D9"),
	m_varRShaderSystem("dvRShaderSystem", "true"),
	m_varRShaderSystemLibPath("dvRShaderSystemLibPath", "resources/RTShaderLib"),
	m_varRCaelum("dvRCaelum", "1"),	
	m_varParentWindow("dvParentWindow", "0x0"),
	m_cmdOgreLoadPlugin("ogreLoadPlugin"),
	m_cmdOgreAddResourceLocation("ogreAddResourceLocation"),
	m_cmdOgreInitialiseResourceGroup("ogreInitialiseResourceGroup"),
	m_cmdScreenshot("screenshot"),
	m_cmdSetShadowMode("setShadowMode"),
	m_cmdSetShadowFarDistance("setShadowFarDistance"),
	m_cmdDumpSceneHierarchy("dumpSceneHierarchy"),
	m_pclOgreWindow(NULL),
	m_pclMainSceneManager(NULL),
	m_pclShaderGenerator(NULL),
	m_eShadowMode(Ogre::SHADOWTYPE_NONE),
	m_pclHelperScene(nullptr),
	m_pclHelperCamera(nullptr)
{
	LogMessage("[Render] Initializing");

	m_cmdOgreLoadPlugin.SetProc(PH_CONTEXT_CMD_BIND(&Render::CmdOgreLoadPlugin, this));
	m_cmdOgreAddResourceLocation.SetProc(PH_CONTEXT_CMD_BIND(&Render::CmdOgreAddResourceLocation, this));
	m_cmdOgreInitialiseResourceGroup.SetProc(PH_CONTEXT_CMD_BIND(&Render::CmdOgreInitialiseResourceGroup, this));
	m_cmdScreenshot.SetProc(PH_CONTEXT_CMD_BIND(&Render::CmdScreenshot, this));

	m_cmdSetShadowMode.SetProc(PH_CONTEXT_CMD_BIND(&Render::CmdSetShadowMode, this));
	m_cmdSetShadowFarDistance.SetProc(PH_CONTEXT_CMD_BIND(&Render::CmdSetShadowFarDistance, this));

	m_cmdDumpSceneHierarchy.SetProc(PH_CONTEXT_CMD_BIND(&Render::CmdDumpSceneHierarchy, this));

	//varRCaelum.SetCallback(VarCallback_t(this, &Render::VarRCaelumChanged));

	LogMessage("[Render] Initializing Ogre");
	m_upRoot.reset(new Ogre::Root("", ""));
	Ogre::Log *log = Ogre::LogManager::getSingleton().getDefaultLog();
	log->setDebugOutputEnabled(false);
	log->addListener(&clOgreLogListener_gl);

	m_upOverlaySystem.reset(new Ogre::OverlaySystem());

	LogMessage("[Render] Initialized.");
}

Phobos::OgreEngine::Render::~Render(void)
{
	//we must make sure that caelum is destroyed before ogre :(
	//clCaelum.Shutdown();

	m_upOverlaySystem.reset();

	if(m_ipWindow)
	{
		//EventManager_c::GetInstance()->RemoveListener(*(m_ipWindow.get()));
	}
}

size_t Phobos::OgreEngine::Render::GetScreenWidth()
{
	return m_ipWindow->GetWidth();
}
	
size_t Phobos::OgreEngine::Render::GetScreenHeight()
{
	return m_ipWindow->GetHeight();
}

void Phobos::OgreEngine::Render::SetParentWindow(void *parent)
{
	PH_ASSERT(m_ipWindow);

	m_ipWindow->SetParentWindow(parent);	
}

void Phobos::OgreEngine::Render::OnInit(void)
{		
	LogMessage("[Render::OnInit] Initializing");

	m_ipWindow = System::Window::Create("RenderWindow");

	UIntSize_t size(m_varRScreenX.GetInt(), m_varRScreenY.GetInt());
				
	bool fullScreen = m_varRFullScreen.GetBoolean();
	bool vsync = m_varRVSync.GetBoolean();

	unsigned int parentWindow = 0;
		
	if(!StringIsBlank(m_varParentWindow.GetValue()))
	{
		LogMessage("[Render::OnInit] Setting parent window.");

		parentWindow = std::stoi(m_varParentWindow.GetValue(), 0, 16);
	}
	else
	{
		LogMessage("[Render::OnInit] No parentWindow set.");
	}

	LogMessage("[Render::OnInit] Opening render window");
	m_ipWindow->Open("Phobos Engine", size, reinterpret_cast<void*>(parentWindow));

	//We need to do a "lazy load" with Ogre plugins, to make sure the render plugins are only 
	//loaded after screen is created.

	//Loading plugin befire creating the screen, may cause problem in some platforms: ie linux opengl plugin
	for(auto it : m_lstPluginsName)		
		m_upRoot->loadPlugin(it);

	m_lstPluginsName.clear();		

	const Ogre::RenderSystemList &renderSystems = (m_upRoot->getAvailableRenderers());
	Ogre::RenderSystemList::const_iterator r_it, end = renderSystems.end();

	LogMessage("[Render::OnBoot] Searching render system");
	bool foundRenderSystem = false;

	for(r_it = renderSystems.begin(); r_it != end; ++r_it)
	{
		const std::string &name((*r_it)->getName());
		LogMessage("\tFound: " + name);

		if(name.find(m_varRRenderSystem.GetValue()) >= 0)
		{
			m_upRoot->setRenderSystem(*r_it);
			foundRenderSystem = true;
			break;
		}
	}

	if(!foundRenderSystem)
	{
		PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Render::OnBoot", "Render system " + m_varRRenderSystem.GetValue() + " not available");
	}

	LogMessage("[Render::OnInit] render system found, initializing Ogre");

    m_upRoot->restoreConfig();
    m_upRoot->initialise(false);

	Ogre::NameValuePairList opts;

	std::stringstream tmp;
	tmp << size.m_tWidth << 'x' << size.m_tHeight;
	opts["resolution"] = tmp.str();
	opts["fullscreen"] = fullScreen ? "true" : "false";
	opts["vsync"] = vsync ? "true" : "false";
                
    void* handler = m_ipWindow->GetHandler();

    if (handler != NULL)
		opts["externalWindowHandle"] = std::to_string(reinterpret_cast<unsigned long>(handler));

    if (m_ipWindow->HasGLContext())
        opts["currentGLContext"] = "true";

	LogMessage("[Render::OnInit] Creating ogre window");
	m_pclOgreWindow = m_upRoot->createRenderWindow("PhobosMainWindow", size.m_tWidth, size.m_tHeight, fullScreen, &opts);

	m_pclOgreWindow->setVisible(true);
		
	LogMessage("[Render::OnInit] Creating SceneManager");
	m_pclMainSceneManager = m_upRoot->createSceneManager(Ogre::ST_GENERIC);
	m_pclHelperScene = m_upRoot->createSceneManager(Ogre::ST_GENERIC);

	//Add a sceneManager and camera just to allow clearing before we have anything to render full screen
	m_pclHelperCamera = m_pclHelperScene->createCamera("HelperUselessCameraToClearScreen");
	auto vp = m_pclOgreWindow->addViewport(m_pclHelperCamera, DefaultViewportZOrder::PRE_GAME);
	vp->setClearEveryFrame(true, Ogre::FBT_COLOUR);
		
	m_pclMainSceneManager->setShadowFarDistance(100);

	if(m_varRShaderSystem.GetBoolean())
	{
		LogMessage("[Render::OnInit] Initializing ShaderSystem.");

		Ogre::RTShader::ShaderGenerator::initialize();

		m_pclShaderGenerator = Ogre::RTShader::ShaderGenerator::getSingletonPtr();
		m_pclShaderGenerator->addSceneManager(m_pclMainSceneManager);		

		Ogre::ResourceGroupManager &resourceGroupManager = Ogre::ResourceGroupManager::getSingleton();

		resourceGroupManager.createResourceGroup(SHADER_SYSTEM_MATERIAL_GROUP, false);
		resourceGroupManager.addResourceLocation(m_varRShaderSystemLibPath.GetValue(), "FileSystem", SHADER_SYSTEM_MATERIAL_GROUP, true);
		resourceGroupManager.initialiseResourceGroup(SHADER_SYSTEM_MATERIAL_GROUP);
		resourceGroupManager.loadResourceGroup(SHADER_SYSTEM_MATERIAL_GROUP);
	}				

	this->SetShadowMode(m_eShadowMode);

	LogMessage("[Render::OnInit] Initializing all resource groups");
	Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();

	LogMessage("[Render::OnInit] Ready.");	
}

void Phobos::OgreEngine::Render::OnUpdate(void)
{		
	//Need to clear pending dirty hash because of multiple scene managers
	//http://www.ogre3d.org/forums/viewtopic.php?p=189032#189032
	if(!Ogre::Pass::getDirtyHashList().empty() || !Ogre::Pass::getPassGraveyard().empty())
	{
		Ogre::SceneManagerEnumerator::SceneManagerIterator scenesIter = m_upRoot->getSceneManagerIterator();
  
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

    m_upRoot->renderOneFrame();
}

void Phobos::OgreEngine::Render::OnPreInit()
{
	auto &console = Engine::Console::GetInstance();

	console.AddContextCommand(m_cmdOgreAddResourceLocation);
	console.AddContextCommand(m_cmdOgreInitialiseResourceGroup);
	console.AddContextCommand(m_cmdOgreLoadPlugin);
	console.AddContextCommand(m_cmdScreenshot);

	console.AddContextCommand(m_cmdSetShadowMode);
	console.AddContextCommand(m_cmdSetShadowFarDistance);

	console.AddContextCommand(m_cmdDumpSceneHierarchy);

	console.AddContextVariable(m_varRScreenX);
	console.AddContextVariable(m_varRScreenY);
	console.AddContextVariable(m_varRFullScreen);
	console.AddContextVariable(m_varRVSync);
	console.AddContextVariable(m_varRRenderSystem);
	console.AddContextVariable(m_varRCaelum);
	console.AddContextVariable(m_varParentWindow);

	console.AddContextVariable(m_varRShaderSystem);
	console.AddContextVariable(m_varRShaderSystemLibPath);
}

//
//
//RENDER FUNCTIONS
//
//

Ogre::SceneManager *Phobos::OgreEngine::Render::CreateSceneManager(Ogre::SceneTypeMask typeMask)
{
	return m_upRoot->createSceneManager(typeMask);
}

void Phobos::OgreEngine::Render::DestroySceneManager(Ogre::SceneManager *manager)
{
	m_upRoot->destroySceneManager(manager);
}

void Phobos::OgreEngine::Render::EnableOverlay(Ogre::SceneManager &manager)
{
	manager.addRenderQueueListener(m_upOverlaySystem.get());
}

void Phobos::OgreEngine::Render::ClearScene()
{
	m_pclMainSceneManager->clearScene();
}

void Phobos::OgreEngine::Render::AddRenderQueueListener(Ogre::RenderQueueListener &listener)
{
	m_pclMainSceneManager->addRenderQueueListener(&listener);
}

void Phobos::OgreEngine::Render::RemoveRenderQueueListener(Ogre::RenderQueueListener &listener)
{
	m_pclMainSceneManager->removeRenderQueueListener(&listener);
}

Ogre::SceneNode *Phobos::OgreEngine::Render::GetSceneNode(const String_t &name)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	return m_pclMainSceneManager->getSceneNode(name);
}

Ogre::SceneNode *Phobos::OgreEngine::Render::CreateSceneNode(const String_t &name)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	return m_pclMainSceneManager->getRootSceneNode()->createChildSceneNode(name);
}

Ogre::SceneNode *Phobos::OgreEngine::Render::CreateSceneNode()
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	return m_pclMainSceneManager->getRootSceneNode()->createChildSceneNode();
}

void Phobos::OgreEngine::Render::DestroySceneNode(Ogre::SceneNode *node)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);		

	m_pclMainSceneManager->destroySceneNode(node->getName());
}

Ogre::Entity* Phobos::OgreEngine::Render::CreateEntity(const String_t &meshName, UInt32_t flags)
{
	String_t tmpName;
	GenerateOgreName(tmpName);

	return this->CreateEntity(tmpName, meshName, flags);
}

Ogre::Entity *Phobos::OgreEngine::Render::CreateEntity(const String_t &entityName, const String_t &meshName, UInt32_t flags)
{
	String_t tmpPath;		
	String_t extension;

	const String_t *meshNameToUse = &meshName;

	//Newer ogitor does nto include .mesh in mesh names, so fix it here
	bool found = Path::GetExtension(extension, meshName);		 
	if((found && extension != "mesh") || (!found))
	{
		tmpPath = meshName;
		tmpPath.append(".mesh");
		meshNameToUse = &tmpPath;
	}

	Ogre::Entity *ent = m_pclMainSceneManager->createEntity(entityName, *meshNameToUse);

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

void Phobos::OgreEngine::Render::DestroyEntity(Ogre::Entity *entity)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);
	PH_ASSERT_VALID(entity);

	m_pclMainSceneManager->destroyEntity(entity);
}

Ogre::Light *Phobos::OgreEngine::Render::CreateLight()
{
	String_t str;
	GenerateOgreName(str);

	return this->CreateLight(str);
}

Ogre::Light *Phobos::OgreEngine::Render::CreateLight(const String_t &name)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	return(m_pclMainSceneManager->createLight(name));
}

void Phobos::OgreEngine::Render::DestroyLight(Ogre::Light *light)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);
	PH_ASSERT_VALID(light);

	m_pclMainSceneManager->destroyLight(light);
}

Ogre::Camera *Phobos::OgreEngine::Render::CreateCamera()
{
	String_t tmp;

	GenerateOgreName(tmp);

	return this->CreateCamera(tmp);
}

Ogre::Camera *Phobos::OgreEngine::Render::CreateCamera(const String_t &name)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	return(m_pclMainSceneManager->createCamera(name));
}

void Phobos::OgreEngine::Render::DestroyCamera(Ogre::Camera *camera)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	m_pclMainSceneManager->destroyCamera(camera);
}

Ogre::ParticleSystem *Phobos::OgreEngine::Render::CreateParticleSystem(const String_t &templateName)
{
	String_t name;
	GenerateOgreName(name);

	return this->CreateParticleSystem(name, templateName);
}

Ogre::ParticleSystem *Phobos::OgreEngine::Render::CreateParticleSystem(const String_t &name, const String_t &templateName)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	return m_pclMainSceneManager->createParticleSystem(name, templateName);
}

void Phobos::OgreEngine::Render::DestroyParticleSystem(Ogre::ParticleSystem *ps)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	m_pclMainSceneManager->destroyParticleSystem(ps);
}

Ogre::TerrainGroup *Phobos::OgreEngine::Render::CreateTerrainGroup(Ogre::Terrain::Alignment align, UInt16_t terrainSize, Float_t terrainWorldSize)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	return new Ogre::TerrainGroup(m_pclMainSceneManager, align, terrainSize, terrainWorldSize);
}

void Phobos::OgreEngine::Render::DestroyTerrainGroup(Ogre::TerrainGroup *group)
{
	delete group;
}

void Phobos::OgreEngine::Render::SetSkyBox(bool enable, const String_t &materialName, Float_t distance, bool drawFirst, const Ogre::Quaternion& orientation)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	m_pclMainSceneManager->setSkyBox(enable, materialName, distance, drawFirst, orientation);
}

void Phobos::OgreEngine::Render::SetAmbientColor(const Ogre::ColourValue &value)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	m_pclMainSceneManager->setAmbientLight(value);		
}

Ogre::ColourValue Phobos::OgreEngine::Render::GetAmbientColor() const
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	return m_pclMainSceneManager->getAmbientLight();
}

void Phobos::OgreEngine::Render::SetFog(Ogre::FogMode mode, Ogre::ColourValue color, Float_t density, Float_t min, Float_t max)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);

	m_pclMainSceneManager->setFog(mode, color, density, min, max);
}

Ogre::Viewport *Phobos::OgreEngine::Render::AddViewport(Ogre::Camera *camera, int ZOrder)
{
	PH_ASSERT_VALID(m_pclOgreWindow);

	Ogre::Viewport *viewport = m_pclOgreWindow->addViewport(camera, ZOrder);

	if(m_varRShaderSystem.GetBoolean())
		viewport->setMaterialScheme(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

	return(viewport);
}

void Phobos::OgreEngine::Render::RemoveViewport(int ZOrder)
{
	PH_ASSERT_VALID(m_pclOgreWindow);

	m_pclOgreWindow->removeViewport(ZOrder);
}

const Ogre::RenderTarget::FrameStats &Phobos::OgreEngine::Render::GetFrameStats()
{
	PH_ASSERT_VALID(m_ipWindow);

	return m_pclOgreWindow->getStatistics();
}

/*
IM_ErrorHandler_t Render::LoadCaelum(const String_t &fileName)
{
	PH_ASSERT_VALID(m_pclMainSceneManager);
	PH_ASSERT_VALID(m_pclOgreWindow);

	if(!varRCaelum.GetBoolean())
		return IM_SUCCESS;

	if(IM_ErrorHandler_t e = clCaelum.Load(fileName, *m_pclMainSceneManager) != IM_SUCCESS)
		return e;

	clCaelum.Listen(*m_pclOgreWindow);

	return IM_SUCCESS;
}

void Render::UnloadCaelum()
{
	clCaelum.StopListening(*m_pclOgreWindow);

	clCaelum.Unload();
}
*/

//
//
// SHADOW MANAGEMENT
//
//

namespace
{
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
}

void Phobos::OgreEngine::Render::CmdSetShadowMode(const Shell::StringVector_t &args, Shell::Context &)
{
	static Enum<Ogre::ShadowTechnique, ShadowType_s> clShadowType_gl(stShadowTypes_gl);

	if(args.size() < 2)
	{
		LogMessage("[Render::CmdSetShadowMode] Missing parameter type, usage: setShadowMode [none|texture_modulative_integrated]");

		return;
	}

	Ogre::ShadowTechnique tech;

	if(!clShadowType_gl.TryGetValue(tech,args[1]))
	{
		LogMessage("[Render::CmdSetShadowMode] Parameter " + args[1] + " is invalid");

		return;
	}

	if(tech == m_eShadowMode)
		return;

	if(m_eShadowMode != tech)
	{
		m_eShadowMode = tech;

		if(m_pclMainSceneManager == NULL)
			return;

		this->SetShadowMode(tech);
	}
}

void Phobos::OgreEngine::Render::CmdSetShadowFarDistance(const Shell::StringVector_t &args, Shell::Context &)
{
	if(args.size() < 2)
	{
		LogMessage("[Render::CmdSetShadowFarDistance] Missing distance parameter, usage: setShadowFarDistance <value>");
	}
	else
	{
		m_pclMainSceneManager->setShadowFarDistance(std::stof(args[1]));
	}
}

void Phobos::OgreEngine::Render::SetShadowMode(Ogre::ShadowTechnique tech)
{
	m_pclMainSceneManager->setShadowTechnique(tech);

	if(m_varRShaderSystem.GetBoolean())
	{
		Ogre::RTShader::RenderState *schemRenderState = m_pclShaderGenerator->getRenderState(Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME);

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
			m_pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 1);
			m_pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 6);
			m_pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 3);

			m_pclMainSceneManager->setShadowTextureSettings(512, 18, Ogre::PF_FLOAT32_R);
			m_pclMainSceneManager->setShadowTextureSelfShadow(true);
			m_pclMainSceneManager->setShadowColour(Ogre::ColourValue(0.1f, 0.5f, 0.1f));
			m_pclMainSceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(new Ogre::DefaultShadowCameraSetup()));
		}
		else if(tech == Ogre::SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED)
		{
			m_pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_DIRECTIONAL, 3);
			m_pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_POINT, 6);
			m_pclMainSceneManager->setShadowTextureCountPerLightType(Ogre::Light::LT_SPOTLIGHT, 3);

			m_pclMainSceneManager->setShadowTextureSettings(512, 18, Ogre::PF_FLOAT32_R);
			m_pclMainSceneManager->setShadowTextureSelfShadow(true);

			m_pclMainSceneManager->setShadowTextureCasterMaterial("PSSM/shadow_caster");

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

			m_pclMainSceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(pssmSetup));
		
			Ogre::RTShader::SubRenderState* subRenderState = m_pclShaderGenerator->createSubRenderState(Ogre::RTShader::IntegratedPSSM3::Type);	
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
IM_Bool_t Render::VarRCaelumChanged(const IM_Shell::Variable &var, const String_t &currentValue, const String_t &newValue)
{
	//caleum turned off?
	if(!IM_StringToBoolean(newValue))
		this->UnloadCaelum();
	else
	{
		IM_KernelPrintf(IM_KERNEL_LOG_NORMAL_FLAGS, "[Render::VarRCaelumChanged] Caelum will only be show when it is reloaded");
	}

	return IM_TRUE;
}
*/

//
//
//RESOURCE COMMANDS
//
//

void Phobos::OgreEngine::Render::CmdOgreAddResourceLocation(const Shell::StringVector_t &args, Shell::Context &)
{
	size_t sz = args.size();
	if(sz < 4)
	{
		LogMessage("[Render::CmdOgreAddResourceLocation] ERROR: insuficient parameters, usage: ogreAddResourceLocation <name> <type> <group> [<recursive>]");

		return;
	}

	Ogre::ResourceGroupManager::getSingleton().addResourceLocation(args[1], args[2], args[3], (sz == 5) ? true : false);
}

void Phobos::OgreEngine::Render::CmdOgreInitialiseResourceGroup(const Shell::StringVector_t &args, Shell::Context &)
{
	const size_t sz = args.size();
	if(sz < 2)
	{
		LogMessage("[Render::CmdOgreInitializeResourceGroup] ERROR: insuficient parameters, usage: ogreInitializeResourceGroup <group0> [<group1> <groupN>]");

		return;
	}

	using namespace Ogre;

	ResourceGroupManager &manager = ResourceGroupManager::getSingleton();

	for(size_t i = 1; i < sz; ++i)
	{
		try
		{
			const String_t &name = args[i];

			if(manager.resourceGroupExists(name) && manager.isResourceGroupInitialised(name))
			{
				LogMessage("[Render::CmdOgreInitialiseResourceGroup] Warning: ResourceGroup " + name + "is being initialised again, clearing it first");
				manager.clearResourceGroup(name);
			}

			manager.initialiseResourceGroup(name);
		}
		catch(Ogre::Exception &e)
		{
			LogOgreException("Render::CmdOgreInitialiseResourceGroup", e);
		}
	}
}

//
//
//MISC COMMANDS
//
//

void Phobos::OgreEngine::Render::CmdOgreLoadPlugin(const Shell::StringVector_t &container, Shell::Context &)
{
	if(container.size() < 2)
	{
		LogMessage("[Render::CmdOgreLoadPlugin] ERROR: insuficient parameters, usage: ogreLoadPlugin <pluginName>");

		return;
	}

	m_lstPluginsName.push_back(container[1]);		
}

void Phobos::OgreEngine::Render::CmdScreenshot(const Shell::StringVector_t &container, Shell::Context &)
{
	if(m_pclOgreWindow)
	{
		try
		{
			Ogre::String str(m_pclOgreWindow->writeContentsToTimestampedFile("screenshots/screen", ".jpg"));

			LogMessage("Created " + str + "");
		}
		catch(Ogre::Exception &e)
		{
			LogOgreException("Render::CmdScreenshot", e);
		}
	}
}

static void DumpSceneHierarchy_r(std::ostream &stream, const Ogre::SceneNode &node, unsigned int depth)
{
	std::string tabs(depth, '\t');

	stream << tabs << node.getName() << std::endl;
	stream << tabs << "position: " << node.getPosition() << std::endl;
	stream << tabs << "orientation: " << node.getOrientation() << std::endl;
	stream << tabs << "scale: " << node.getScale() << std::endl;

	for(auto it = node.getChildIterator();	it.hasMoreElements(); it.moveNext())
	{
		DumpSceneHierarchy_r(stream, *static_cast<Ogre::SceneNode*>(it.current()->second), depth+1);
	}
	stream << std::endl;
}

void Phobos::OgreEngine::Render::CmdDumpSceneHierarchy(const Shell::StringVector_t &container, Shell::Context &)
{
	std::ofstream output("scene.txt");

	DumpSceneHierarchy_r(output, *(m_pclMainSceneManager->getRootSceneNode()), 0);
}
