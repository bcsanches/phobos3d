/*
Phobos 3d
May 2010
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

#include "PH_OgreConsole.h"

#include <limits.h>

#include <PH_Core.h>

#include <Phobos/Error.h>
#include <Phobos/Memory.h>

#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreSceneManager.h>

#include "PH_Render.h"
#include "PH_OgreUtils.h"

#define CONSOLE_TIME 2.5f

#define CONSOLE_LINE_LENGHT 120

#define CONSOLE_HEIGHT 0.5f

#define CONSOLE_LINE_COUNT 15

namespace Phobos
{	
	OgreConsole &OgreConsole::CreateInstance(void)
	{		
		Console::UpdateInstance(OgreConsolePtr_t(PH_NEW OgreConsole()));		

		return static_cast<OgreConsole &>(Console::GetInstance());
	}
	
	OgreConsole::OgreConsole(void):
		Console("Console"),
		m_pclRect(NULL),
		m_pclTextBox(NULL),
		m_pclOverlay(NULL),
		m_pclRenderInfoOverlay(NULL),
		m_fpHeight(0),		
		m_varMaterialName("dvConsoleMaterialName", "PH_Console/Background"),
		m_varShowRenderInfo("dvShowRenderInfo", "0"),
		m_fEditBoxChanged(true),
		m_fTextBufferChanged(true)
	{				
		this->AddContextVariable(m_varMaterialName);
		this->AddContextVariable(m_varShowRenderInfo);		
	}

	OgreConsole::~OgreConsole(void)
	{
		//empty
	}	

	void OgreConsole::OnFinalize()
	{
		Ogre::OverlayManager& overlayManager = Ogre::OverlayManager::getSingleton();

		overlayManager.destroyOverlayElement(m_pclRect);
		overlayManager.destroyOverlayElement(m_pclTextBox);	

		overlayManager.destroy(m_pclOverlay);		
		overlayManager.destroy(m_pclRenderInfoOverlay);		

		if(m_pclSceneManager)
		{
			if(m_pclCamera)
				m_pclSceneManager->destroyCamera(m_pclCamera);

			Render &render = Render::GetInstance();

			render.DestroySceneManager(m_pclSceneManager);
		}				
	}

	void OgreConsole::OnEditBoxChanged()
	{
		m_fEditBoxChanged = true;
	}
	
	void OgreConsole::OnTextListChanged()
	{
		m_fTextBufferChanged = true;
	}

	void OgreConsole::OnUpdate(void)
	{
		if(m_pclRenderInfoOverlay)
		{
			if(m_varShowRenderInfo.GetBoolean())
			{
				if(!m_pclRenderInfoOverlay->isVisible())
					m_pclRenderInfoOverlay->show();

				this->UpdateRenderInfo();
			}
			else if(m_pclRenderInfoOverlay->isVisible())
				m_pclRenderInfoOverlay->hide();
		}

		if(!m_pclTextBox)
			return;

		bool visible = true;

		if(this->IsActive())
		{
			if(m_fpHeight < CONSOLE_HEIGHT)
			{
				m_fpHeight += Core::GetInstance().GetSimInfo().m_stTimers[CORE_SYS_TIMER].m_fpRenderFrameTime * CONSOLE_TIME;
				m_fUIMoved = true;
				if(m_fpHeight >= CONSOLE_HEIGHT)
				{
					m_fpHeight = CONSOLE_HEIGHT;
				}
			}
		}
		else if(m_fpHeight > 0)
		{
			m_fpHeight -= Core::GetInstance().GetSimInfo().m_stTimers[CORE_SYS_TIMER].m_fpRenderFrameTime * CONSOLE_TIME;
			m_fUIMoved = true;

			if(m_fpHeight <= 0)
			{
				m_pclOverlay->hide();
				m_fpHeight = 0;
			}
		}
		else
		{
			visible = false;
		}

		if(visible)
		{
			//pclTextBox->setPosition(0,(fHeight-1)*0.5);
			//pclRect->setCorners(-1,1+fHeight,1,1-fHeight);
			if(m_fUIMoved)
			{
				m_pclRect->setPosition(0, (m_fpHeight - CONSOLE_HEIGHT));
				m_fUIMoved = false;
			}

			if(m_fTextBufferChanged || m_fEditBoxChanged)
			{
				Ogre::String text;

				size_t i = 0;
				TextList_t::const_iterator end = this->ListTextEnd();
				for(TextList_t::const_iterator it = this->ListTextBegin(); it != end; ++it, ++i)
				{
					if(i == CONSOLE_LINE_COUNT)
						break;

					const String_t &str = (*it);

					if(str.length() > CONSOLE_LINE_LENGHT)
					{
						text.append(str, 0, CONSOLE_LINE_LENGHT);
					}
					else
						text += str;

					text += "\n";
				}

				text += "] ";
				text += this->EditBoxStr();

				try
				{

					m_pclTextBox->setCaption(text);
				}
				catch(...)
				{
					//log it
					//the fucking conversion to utf8 sometimes throw exceptions
				}

				m_fTextBufferChanged = m_fEditBoxChanged = false;
			}
		}
	}

	void OgreConsole::OnRenderReady(void)
    {
		using namespace Ogre;

		m_fpHeight = 0;

		try
		{  
			Render &render = Render::GetInstance();

			m_pclSceneManager = render.CreateSceneManager(Ogre::ST_GENERIC);
			m_pclCamera = m_pclSceneManager->createCamera("PH_ConsoleCamera");

			// Create background rectangle covering the whole screen
			OverlayManager& overlayManager = OverlayManager::getSingleton();

			m_pclTextBox=overlayManager.createOverlayElement("TextArea","ConsoleText");
			m_pclTextBox->setCaption("Phobos Engine Console");
			m_pclTextBox->setMetricsMode(GMM_RELATIVE);
			m_pclTextBox->setPosition(0,0);
			m_pclTextBox->setParameter("font_name","PH_Console");
			m_pclTextBox->setParameter("colour_top","1 1 1");
			m_pclTextBox->setParameter("colour_bottom","1 1 1");
			m_pclTextBox->setParameter("char_height","0.03");

			m_pclRect = static_cast<OverlayContainer *>(overlayManager.createOverlayElement("Panel", "panelName"));
			m_pclRect->setMetricsMode(Ogre::GMM_RELATIVE);
			m_pclRect->setPosition(0, -CONSOLE_HEIGHT);
			m_pclRect->setDimensions(1, CONSOLE_HEIGHT);
			m_pclRect->setMaterialName(m_varMaterialName.GetValue());


			m_pclOverlay=overlayManager.create("Console");
			m_pclOverlay->add2D(m_pclRect);
			m_pclRect->addChild(m_pclTextBox);

			m_pclOverlay->setZOrder(650);
			m_pclOverlay->show();

			m_pclRenderInfoOverlay = overlayManager.getByName("Core/RenderInfoOverlay");
			m_pclRenderInfoOverlay->show();

			render.AddViewport(m_pclCamera, DefaultViewportZOrder::CONSOLE);
		}
		catch(Ogre::Exception &)
		{
			//FIXME
			//LogOgreException("Console::OnRenderReady", e);
			throw;
		}
	}	

	void OgreConsole::OnToggleConsole()
	{
		if(this->IsActive() && m_pclOverlay)
		{			
			m_pclOverlay->show();
		}
	}					

	void OgreConsole::UpdateRenderInfo()
	{
		using namespace Ogre;

		static String currFps = "Current FPS: ";
		static String avgFps = "Average FPS: ";
		static String bestFps = "Best FPS: ";
		static String worstFps = "Worst FPS: ";
		static String tris = "Triangle Count: ";
		static String batches = "Batch Count: ";

		// update stats when necessary
		try
		{
			OverlayElement* guiAvg = OverlayManager::getSingleton().getOverlayElement("Core/AverageFps");
			OverlayElement* guiCurr = OverlayManager::getSingleton().getOverlayElement("Core/CurrFps");
			OverlayElement* guiBest = OverlayManager::getSingleton().getOverlayElement("Core/BestFps");
			OverlayElement* guiWorst = OverlayManager::getSingleton().getOverlayElement("Core/WorstFps");

			const RenderTarget::FrameStats& stats = Render::GetInstance().GetFrameStats();
			guiAvg->setCaption(avgFps + StringConverter::toString(stats.avgFPS));
			guiCurr->setCaption(currFps + StringConverter::toString(stats.lastFPS));
			guiBest->setCaption(bestFps + StringConverter::toString(stats.bestFPS)
				+" "+StringConverter::toString(stats.bestFrameTime)+" ms");
			guiWorst->setCaption(worstFps + StringConverter::toString(stats.worstFPS)
				+" "+StringConverter::toString(stats.worstFrameTime)+" ms");

			OverlayElement* guiTris = OverlayManager::getSingleton().getOverlayElement("Core/NumTris");
			guiTris->setCaption(tris + StringConverter::toString(stats.triangleCount));

			OverlayElement* guiBatches = OverlayManager::getSingleton().getOverlayElement("Core/NumBatches");
			guiBatches->setCaption(batches + StringConverter::toString(stats.batchCount));

			//OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
			//guiDbg->setCaption(mDebugText);
		}
		catch(...) { /* ignore */ }
	}		
}
