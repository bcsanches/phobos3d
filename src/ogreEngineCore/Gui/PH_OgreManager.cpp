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

#include "Gui/PH_OgreManager.h"

#include <OgreRoot.h>

#include <Rocket/Controls.h>
#include <Rocket/Core/SystemInterface.h>

#include <PH_Console.h>
#include <PH_Kernel.h>

#include "Gui/PH_Context.h"
#include "Gui/PH_OgreRenderInterface.h"

#include "PH_Render.h"

Phobos::Gui::OgreManager_c &Phobos::Gui::OgreManager_c::CreateInstance(void)
{		
	Phobos::Gui::OgreManager_c::UpdateInstance(OgreManagerPtr_t(PH_NEW OgreManager_c()));		

	return static_cast<OgreManager_c &>(OgreManager_c::GetInstance());
}

Phobos::Gui::OgreManager_c::OgreManager_c():	
	pclSceneManager(NULL),
	pclCamera(NULL)
{
	//empty
}

Phobos::Gui::OgreManager_c::~OgreManager_c()
{
	//empty
}

void Phobos::Gui::OgreManager_c::OnRenderReady()
{
	Ogre::ResourceGroupManager::getSingleton().createResourceGroup("Rocket");
	Ogre::ResourceGroupManager::getSingleton().addResourceLocation("./", "FileSystem", "Rocket");
	Ogre::ResourceGroupManager::getSingleton().initialiseResourceGroup("Rocket");

	Phobos::Render_c &render = Phobos::Render_c::GetInstance();

	RenderInterfaceOgre_c *renderer = PH_NEW RenderInterfaceOgre_c(render.GetScreenWidth(), render.GetScreenHeight());
	Rocket::Core::SetRenderInterface(renderer);
	renderer->Release();

	Rocket::Core::SystemInterface *systemInterface = this->CreateSystemInterface();
	Rocket::Core::SetSystemInterface(systemInterface);
	systemInterface->Release();

	Rocket::Core::Initialise();
	Rocket::Controls::Initialise();

	this->LoadFonts();

	pclSceneManager = render.CreateSceneManager(Ogre::ST_GENERIC);
	pclCamera = pclSceneManager->createCamera("Phobos::Gui::Manager::Camera");
	
	pclSceneManager->addRenderQueueListener(this);

	Ogre::Viewport *vp = render.AddViewport(pclCamera, DefaultViewportZOrder::GUI);

	vp->setClearEveryFrame(false, Ogre::FBT_COLOUR);
}

void Phobos::Gui::OgreManager_c::OnFinalize()
{
	if(pclSceneManager)
	{
		Phobos::Render_c &render = Phobos::Render_c::GetInstance();

		pclSceneManager->destroyCamera(pclCamera);
		pclCamera = NULL;

		render.DestroySceneManager(pclSceneManager);
		pclSceneManager = NULL;
	}

	Rocket::Core::Shutdown();

	Gui::Manager_c::OnFinalize();
}

void Phobos::Gui::OgreManager_c::BuildProjectionMatrix(Ogre::Matrix4& projection_matrix)
{
	float z_near = -1;
	float z_far = 1;

	projection_matrix = Ogre::Matrix4::ZERO;

	Render_c &render = Render_c::GetInstance();

	// Set up matrices.
	projection_matrix[0][0] = 2.0f / render.GetScreenWidth();
	projection_matrix[0][3]= -1.0000000f;
	projection_matrix[1][1]= -2.0f / render.GetScreenHeight();
	projection_matrix[1][3]= 1.0000000f;
	projection_matrix[2][2]= -2.0f / (z_far - z_near);
	projection_matrix[3][3]= 1.0000000f;
}

void Phobos::Gui::OgreManager_c::ConfigureRenderSystem()
{
	Ogre::RenderSystem* render_system = Ogre::Root::getSingleton().getRenderSystem();

	// Set up the projection and view matrices.
	Ogre::Matrix4 projection_matrix;
	BuildProjectionMatrix(projection_matrix);
	render_system->_setProjectionMatrix(projection_matrix);
	render_system->_setViewMatrix(Ogre::Matrix4::IDENTITY);

	// Disable lighting, as all of Rocket's geometry is unlit.
	render_system->setLightingEnabled(false);
	// Disable depth-buffering; all of the geometry is already depth-sorted.
	render_system->_setDepthBufferParams(false, false);
	// Rocket generates anti-clockwise geometry, so enable clockwise-culling.
	render_system->_setCullingMode(Ogre::CULL_CLOCKWISE);
	// Disable fogging.
	render_system->_setFog(Ogre::FOG_NONE);
	// Enable writing to all four channels.
	render_system->_setColourBufferWriteEnabled(true, true, true, true);
	// Unbind any vertex or fragment programs bound previously by the application.
	render_system->unbindGpuProgram(Ogre::GPT_FRAGMENT_PROGRAM);
	render_system->unbindGpuProgram(Ogre::GPT_VERTEX_PROGRAM);

	// Set texture settings to clamp along both axes.
	Ogre::TextureUnitState::UVWAddressingMode addressing_mode;
	addressing_mode.u = Ogre::TextureUnitState::TAM_CLAMP;
	addressing_mode.v = Ogre::TextureUnitState::TAM_CLAMP;
	addressing_mode.w = Ogre::TextureUnitState::TAM_CLAMP;
	render_system->_setTextureAddressingMode(0, addressing_mode);

	// Set the texture coordinates for unit 0 to be read from unit 0.
	render_system->_setTextureCoordSet(0, 0);
	// Disable texture coordinate calculation.
	render_system->_setTextureCoordCalculation(0, Ogre::TEXCALC_NONE);
	// Enable linear filtering; images should be rendering 1 texel == 1 pixel, so point filtering could be used
	// except in the case of scaling tiled decorators.
	render_system->_setTextureUnitFiltering(0, Ogre::FO_LINEAR, Ogre::FO_LINEAR, Ogre::FO_POINT);
	// Disable texture coordinate transforms.
	render_system->_setTextureMatrix(0, Ogre::Matrix4::IDENTITY);
	// Reject pixels with an alpha of 0.
	render_system->_setAlphaRejectSettings(Ogre::CMPF_GREATER, 0, false);
	// Disable all texture units but the first.
	render_system->_disableTextureUnitsFrom(1);

	// Enable simple alpha blending.
	render_system->_setSceneBlending(Ogre::SBF_SOURCE_ALPHA, Ogre::SBF_ONE_MINUS_SOURCE_ALPHA);

	// Disable depth bias.
	render_system->_setDepthBias(0, 0);
}

void Phobos::Gui::OgreManager_c::renderQueueStarted(Ogre::uint8 queueGroupId, const Ogre::String& invocation, bool &skipThisInvocation)
{
	if (queueGroupId == Ogre::RENDER_QUEUE_OVERLAY && Ogre::Root::getSingleton().getRenderSystem()->_getViewport()->getOverlaysEnabled())
	{
		this->ConfigureRenderSystem();

		for(Node_c::const_iterator it = this->begin(), end = this->end(); it != end; ++it)
		{
			Context_c *context = static_cast<Context_c *>(it->second);

			context->Render();
		}
	}
}

size_t Phobos::Gui::OgreManager_c::GetScreenWidth()
{
	return Render_c::GetInstance().GetScreenWidth();
}

size_t Phobos::Gui::OgreManager_c::GetScreenHeight()
{
	return Render_c::GetInstance().GetScreenHeight();
}
