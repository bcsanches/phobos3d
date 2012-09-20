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
#include <PH_Error.h>
#include <PH_Kernel.h>

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
	OgreConsole_c &OgreConsole_c::CreateInstance(void)
	{		
		Console_c::UpdateInstance(OgreConsolePtr_t(PH_NEW OgreConsole_c()));		

		return static_cast<OgreConsole_c &>(Console_c::GetInstance());
	}
	
	OgreConsole_c::OgreConsole_c(void):
		Console_c("Console"),
		pclRect(NULL),
		pclTextBox(NULL),
		pclOverlay(NULL),
		pclRenderInfoOverlay(NULL),
		fpHeight(0),		
		varMaterialName("dvConsoleMaterialName", "PH_Console/Background"),
		varShowRenderInfo("dvShowRenderInfo", "0"),
		fEditBoxChanged(true),
		fTextBufferChanged(true)
	{				
		this->AddContextVar(varMaterialName);
		this->AddContextVar(varShowRenderInfo);		
	}

	OgreConsole_c::~OgreConsole_c(void)
	{
		//empty
	}	

	void OgreConsole_c::OnEditBoxChanged()
	{
		fEditBoxChanged = true;
	}
	
	void OgreConsole_c::OnTextListChanged()
	{
		fTextBufferChanged = true;
	}

	void OgreConsole_c::OnUpdate(void)
	{
		if(pclRenderInfoOverlay)
		{
			if(varShowRenderInfo.GetBoolean())
			{
				if(!pclRenderInfoOverlay->isVisible())
					pclRenderInfoOverlay->show();

				this->UpdateRenderInfo();
			}
			else if(pclRenderInfoOverlay->isVisible())
				pclRenderInfoOverlay->hide();
		}

		if(!pclTextBox)
			return;

		bool visible = true;

		if(this->IsActive())
		{
			if(fpHeight < CONSOLE_HEIGHT)
			{
				fpHeight += Core_c::GetInstance().GetSimInfo().stTimers[CORE_SYS_TIMER].fpRenderFrameTime * CONSOLE_TIME;
				fUIMoved = true;
				if(fpHeight >= CONSOLE_HEIGHT)
				{
					fpHeight = CONSOLE_HEIGHT;
				}
			}
		}
		else if(fpHeight > 0)
		{
			fpHeight -= Core_c::GetInstance().GetSimInfo().stTimers[CORE_SYS_TIMER].fpRenderFrameTime * CONSOLE_TIME;
			fUIMoved = true;

			if(fpHeight <= 0)
			{
				pclOverlay->hide();
				fpHeight = 0;
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
			if(fUIMoved)
			{
				pclRect->setPosition(0, (fpHeight - CONSOLE_HEIGHT));
				fUIMoved = false;
			}

			if(fTextBufferChanged || fEditBoxChanged)
			{
				Ogre::String text;

				size_t i = 0;
				TextList_t::const_iterator end = this->ListTextEnd();
				for(TextList_t::const_iterator it = this->ListTextBegin(); it != end; ++it, ++i)
				{
					if(i == CONSOLE_LINE_COUNT)
						break;

					const String_c &str = (*it);

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

					pclTextBox->setCaption(text);
				}
				catch(...)
				{
					//log it
					//the fucking conversion to utf8 sometimes throw exceptions
				}

				fTextBufferChanged = fEditBoxChanged = false;
			}
		}
	}

	void OgreConsole_c::OnRenderReady(void)
        {
		using namespace Ogre;

		fpHeight = 0;

		try
		{  
			Render_c &render = Render_c::GetInstance();

			pclSceneManager = render.CreateSceneManager(Ogre::ST_GENERIC);
			pclCamera = pclSceneManager->createCamera("PH_ConsoleCamera");

			// Create background rectangle covering the whole screen
			OverlayManager& overlayManager = OverlayManager::getSingleton();

			pclTextBox=overlayManager.createOverlayElement("TextArea","ConsoleText");
			pclTextBox->setCaption("Phobos Engine Console");
			pclTextBox->setMetricsMode(GMM_RELATIVE);
			pclTextBox->setPosition(0,0);
			pclTextBox->setParameter("font_name","PH_Console");
			pclTextBox->setParameter("colour_top","1 1 1");
			pclTextBox->setParameter("colour_bottom","1 1 1");
			pclTextBox->setParameter("char_height","0.03");

			pclRect = static_cast<OverlayContainer *>(overlayManager.createOverlayElement("Panel", "panelName"));
			pclRect->setMetricsMode(Ogre::GMM_RELATIVE);
			pclRect->setPosition(0, -CONSOLE_HEIGHT);
			pclRect->setDimensions(1, CONSOLE_HEIGHT);
			pclRect->setMaterialName(varMaterialName.GetValue());


			pclOverlay=overlayManager.create("Console");
			pclOverlay->add2D(pclRect);
			pclRect->addChild(pclTextBox);

			pclOverlay->setZOrder(650);
			pclOverlay->show();

			pclRenderInfoOverlay = overlayManager.getByName("Core/RenderInfoOverlay");
			pclRenderInfoOverlay->show();

			render.AddViewport(pclCamera, INT_MIN);
		}
		catch(Ogre::Exception &)
		{
			//FIXME
			//LogOgreException("Console_c::OnRenderReady", e);
			throw;
		}
	}	

	void OgreConsole_c::OnToggleConsole()
	{
		if(this->IsActive() && pclOverlay)
		{			
			pclOverlay->show();
		}
	}					

	void OgreConsole_c::UpdateRenderInfo()
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

			const RenderTarget::FrameStats& stats = Render_c::GetInstance().GetFrameStats();
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
