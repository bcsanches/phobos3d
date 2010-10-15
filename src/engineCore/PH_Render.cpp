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

#include "PH_Render.h"

#include <OgreEntity.h>
#include <OgreRenderWindow.h>
#include <OgreRoot.h>

#include <PH_Console.h>
#include <PH_Context.h>
#include <PH_ContextUtils.h>
#include <PH_Error.h>
#include <PH_Enum.h>
#include <PH_EventManager.h>
#include <PH_Exception.h>
#include <PH_Kernel.h>
#include <PH_String.h>

#include "PH_Core.h"
#include "PH_OgreUtils.h"

namespace Phobos
{

	RenderPtr_t Render_c::ipInstance_gl;

	class OgreLogListener_c: public Ogre::LogListener
	{
		public:
			virtual void messageLogged( const Ogre::String& message, Ogre::LogMessageLevel lml, bool maskDebug, const Ogre::String & )
			{
				Kernel_c::GetInstance().LogMessage("Ogre: " + message);
			}
	};

	static OgreLogListener_c clOgreLogListener_gl;

	RenderPtr_t Render_c::CreateInstance()
	{
		PH_ASSERT(ipInstance_gl == NULL);

		ipInstance_gl.reset(new Render_c());

		return ipInstance_gl;
	}

	void Render_c::ReleaseInstance()
	{
		ipInstance_gl.reset();
	}

	RenderPtr_t Render_c::GetInstance()
	{
		PH_ASSERT_VALID(ipInstance_gl);

		return(ipInstance_gl);
	}	

	Render_c::Render_c(void):
		CoreModule_c("Render"),		
		varRScreenX("dvRScreenX", "800"),
		varRScreenY("dvRScreenY", "600"),
		varRVSync("dvRVSync", "1"),
		varRFullScreen("dvRFullScreen", "0"),
		varRRenderSystem("dvRRenderSystem", "Direct3D9"),
		varRCaelum("dvRCaelum", "1"),
		cmdOgreLoadPlugin("ogreLoadPlugin"),
		cmdOgreAddResourceLocation("ogreAddResourceLocation"),
		cmdOgreInitialiseResourceGroup("ogreInitialiseResourceGroup"),
		cmdScreenshot("screenshot"),
		cmdSetShadowMode("setShadowMode"),
		cmdSetShadowFarDistance("setShadowFarDistance"),
		pclOgreWindow(NULL),
		pclMainSceneManager(NULL),
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

		ipWindow->SetEventManager(EventManager_c::GetInstance());
	
		const Ogre::RenderSystemList &renderSystems = (spRoot->getAvailableRenderers());	
		Ogre::RenderSystemList::const_iterator r_it, end = renderSystems.end();

		kernel.LogMessage("[Render_c::OnBoot] Searching render system");
		bool foundRenderSystem = false;

		for(r_it = renderSystems.begin(); r_it != end; ++r_it)
		{
			const std::string &name((*r_it)->getName());

			if(name.find(varRRenderSystem.GetValue()) >= 0)
			{
				spRoot->setRenderSystem(*r_it);
				foundRenderSystem = true;
				break;
			}
		}

		if(!foundRenderSystem)
		{			
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Render_c::OnBoot", "Render system " + varRRenderSystem.GetValue() + "not available");						
		}

		kernel.LogMessage("[Render_c::OnBoot] render system found, initializing Ogre");
		spRoot->initialise(false);

		Ogre::NameValuePairList opts;

		std::stringstream tmp;
		tmp << r.tWidth << 'x' << r.tHeight;	
		opts["resolution"] = tmp.str();
		opts["fullscreen"] = fullScreen ? "true" : "false";
		opts["vsync"] = vsync ? "true" : "false";
	
		opts["externalWindowHandle"] = PointerToString(ipWindow->GetHandler());

		kernel.LogMessage("[Render_c::OnBoot] Creating ogre window");
		pclOgreWindow = spRoot->createRenderWindow("PhobosMainWindow", r.tWidth, r.tHeight, fullScreen, &opts);			
		
		kernel.LogMessage("[Render_c::OnBoot] Initializing all resource groups");
		Ogre::ResourceGroupManager::getSingleton().initialiseAllResourceGroups();		

		kernel.LogMessage("[Render_c::OnBoot] Creating SceneManager");
		pclMainSceneManager = spRoot->createSceneManager(Ogre::ST_GENERIC);

		this->SetShadowMode(eShadowMode);
	
		pclMainSceneManager->setShadowFarDistance(1000);

		kernel.LogMessage("[Render_c::OnBoot] Ready.");
		Core_c::GetInstance()->OnEvent(CORE_EVENT_RENDER_READY);		
	}

	void Render_c::OnUpdate(void)
	{
		if(!ipWindow)
			return;		
	
		//clAnimationManager.Update();	
		//clCaelum.Update(IM_GetGameTimer().fRenderFrameTime);

		spRoot->renderOneFrame();		
	}

	void Render_c::OnPrepareToBoot()
	{
		ConsolePtr_t console = Console_c::GetInstance();

		console->AddContextCmd(cmdOgreAddResourceLocation);
		console->AddContextCmd(cmdOgreInitialiseResourceGroup);
		console->AddContextCmd(cmdOgreLoadPlugin);
		console->AddContextCmd(cmdScreenshot);

		console->AddContextCmd(cmdSetShadowMode);
		console->AddContextCmd(cmdSetShadowFarDistance);

		console->AddContextVar(varRScreenX);
		console->AddContextVar(varRScreenY);
		console->AddContextVar(varRFullScreen);
		console->AddContextVar(varRVSync);
		console->AddContextVar(varRRenderSystem);
		console->AddContextVar(varRCaelum);
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

	Ogre::SceneNode *Render_c::CreateSceneNode()
	{
		PH_ASSERT_VALID(pclMainSceneManager);

		return (pclMainSceneManager->getRootSceneNode()->createChildSceneNode());
	}

	void Render_c::DestroySceneNode(Ogre::SceneNode *node)
	{
		PH_ASSERT_VALID(pclMainSceneManager);
		PH_ASSERT_VALID(node);

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
		Ogre::Entity *ent = pclMainSceneManager->createEntity(entityName, meshName);

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

	Ogre::Viewport *Render_c::AddViewport(Ogre::Camera *camera, int ZOrder)
	{
		PH_ASSERT_VALID(pclOgreWindow);

		return(pclOgreWindow->addViewport(camera, ZOrder));
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
		{"stencil_modulative", Ogre::SHADOWTYPE_STENCIL_MODULATIVE}
	};

	void Render_c::CmdSetShadowMode(const StringVector_t &args, Context_c &)
	{
		static Enum_c<Ogre::ShadowTechnique, ShadowType_s> clShadowType_gl(stShadowTypes_gl);

		if(args.size() < 2)
		{
			Kernel_c::GetInstance().LogMessage("[Render_c::CmdSetShadowMode] Missing parameter type, usage: setShadowMode [none|stencil_additive|stencil_modulative]");			

			return;
		}

		Ogre::ShadowTechnique tech;
	
		if(!clShadowType_gl.GetValue(tech,args[1]))
		{
			Kernel_c::GetInstance().LogMessage("[Render_c::CmdSetShadowMode] Parameter " + args[1] + " is invalid");

			return;
		}

		this->SetShadowMode(tech);		
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

		spRoot->loadPlugin(container[1]);		
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
