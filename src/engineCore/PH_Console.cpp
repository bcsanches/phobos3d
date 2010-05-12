/*
Phobos 3d
  May 2010

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

#include "PH_Console.h"

#include <limits.h>

#include <PH_Error.h>
#include <PH_InputDefs.h>
//FIXME
//#include <PH_InputDevice.h>
//#include <PH_InputManager.h>
//#include <PH_InputMapper.h>
#include <PH_Kernel.h>
#include <PH_Path.h>

#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreSceneManager.h>

#include "PH_Core.h"
#include "PH_Render.h"
#include "PH_OgreUtils.h"

#define CONSOLE_KEY IM_CHAR('`')

#define CONSOLE_TIME 2.5f

#define CONSOLE_LINE_COUNT 15
#define CONSOLE_LINE_LENGHT 120

#define CONSOLE_HISTORY_COUNT 32

#define CONSOLE_HEIGHT 0.5f

namespace Phobos
{
	ConsolePtr_t Console_c::ipInstance_gl;

	ConsolePtr_t &Console_c::CreateInstance(void)
	{
		PH_ASSERT(!ipInstance_gl);

		ipInstance_gl.reset(new Console_c());

		return ipInstance_gl;
	}

	ConsolePtr_t &Console_c::GetInstance(void)
	{
		return ipInstance_gl;
	}

	void Console_c::ReleaseInstance(void)
	{
		ipInstance_gl.reset();
	}

	Console_c::Console_c(void):	
		CoreModule_c("Console"),
		//FIXME
		//pclInputManager(NULL),
		pclRect(NULL),	
		pclTextBox(NULL),
		pclOverlay(NULL),
		pclRenderInfoOverlay(NULL),
		fpHeight(0),
		lstText(CONSOLE_LINE_COUNT),
		lstHistory(CONSOLE_HISTORY_COUNT),
		varMaterialName("dvConsoleMaterialName", "PH_Console/Background"),
		varShowRenderInfo("dvShowRenderInfo", "0"),
		cmdLsO("lso"),
		cmdCdO("cdo")
	{
		/*
		//FIXME
		clInputManagerListener.SetProc(IM_SubjectListenerProc_t(this, &IM_Console_c::InputManagerListenerProc));
		clInputDeviceKeyboardListener.SetProc(IM_SubjectListenerProc_t(this, &IM_Console_c::InputDeviceKeyboardListenerProc));	
		*/		

		//FIXME
		//IM_Kernel_c::GetInstance().SetLogProc(IM_KernelLogProc_t(this, &IM_Console_c::LogProc));

		clContext.AddContextVar(varMaterialName);
		clContext.AddContextVar(varShowRenderInfo);		

		//FIXME
		//clShell.CreateDefaultCmds(clContext);
	}

	Console_c::~Console_c(void)
	{		
		//FIXME
		//IM_Kernel_c::GetInstance().SetLogProc(IM_KernelLogProc_t());
	}

	void Console_c::OnFixedUpdate()
	{
		//FIXME
		//pclInputManager->Update();
	}

	void Console_c::OnInit(void)
	{
		Kernel_c &kernel = Kernel_c::GetInstance();

		/*
		FIXME
		pclInputManager = static_cast<IM_InputManager_c *>(kernel->LookupNode(IM_DEFAULT_PATH_NAME_INPUT_MANAGER, NULL));

		if(NULL == pclInputManager)
		{
			IM_KernelPushErrorf(IM_ERROR_NODE_NOT_FOUND, "IM_Console_c::OnInit", "Cant find IM_InputMapper");
			return(IM_CORE_EVENT_ERROR);
		}	
	
		pclInputManager->AddListener(clInputManagerListener);		
		*/		
	}

	void Console_c::OnUpdate(void)
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

		if(fActive)		
		{
			if(fpHeight < CONSOLE_HEIGHT)
			{
				fpHeight += Core_c::GetInstance()->GetSimInfo().stTimers[CORE_SYS_TIMER].fpRenderFrameTime * CONSOLE_TIME;
				fUIMoved = true;
				if(fpHeight >= CONSOLE_HEIGHT)
				{					
					fpHeight = CONSOLE_HEIGHT;
				}
			}		
		}
		else if(fpHeight > 0)
		{
			fpHeight -= Core_c::GetInstance()->GetSimInfo().stTimers[CORE_SYS_TIMER].fpRenderFrameTime * CONSOLE_TIME;
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
				TextList_t::iterator end = lstText.end();
				for(TextList_t::iterator it = lstText.begin(); it != end; ++it, ++i)
				{
					if(i == CONSOLE_LINE_COUNT)
						break;		

					const String_c &str = (*it);

					if(str.length() > CONSOLE_LINE_LENGHT)
					{
						//IM_String_c tmp;
						//tmp.SetSub(&str, 0, CONSOLE_LINE_LENGHT);					

						//text += tmp.GetStr();
						text.append(str, 0, CONSOLE_LINE_LENGHT);
					}
					else
						text += str;

					text += "\n";			
				}
			
				text += "] ";
				text += clEditBox.GetStr();

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

	void Console_c::OnRenderReady(void)
	{
		using namespace Ogre;	

		fpHeight = 0;

		try
		{
			RenderPtr_t render = Render_c::GetInstance();

			pclSceneManager = render->CreateSceneManager(Ogre::ST_GENERIC);
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
		
			render->AddViewport(pclCamera, INT_MIN);
		}
		catch(Ogre::Exception &)
		{
			//FIXME
			//LogOgreException("Console_c::OnRenderReady", e);
			throw;
		}
	}	

	void Console_c::OnChar(Char_t ch)
	{
		if(fIgnoreFirstChar)
		{
			fIgnoreFirstChar = false;
			return;
		}

		switch(ch)
		{
			case KB_BACKSPACE:
				clEditBox.Backspace();
				break;

			case KB_ENTER:
				this->OnEnter();
				break;

			default:
				clEditBox.AddChar(ch);
				break;
		}

		fEditBoxChanged = true;
	}

	void Console_c::Execute(const String_c &cmdLine)
	{
		clContext.Execute(cmdLine);
	}

	void Console_c::ExecuteFromFile(const String_c &fileName)
	{
		clContext.ExecuteFromFile(fileName);
	}

	void Console_c::OnEnter(void)
	{		
		const String_c &cmdLine = clEditBox.GetStr();

		PH_ASSERT_MSG(false, "Not implemented");
		//FIXME
		//clShell.Execute(clContext, cmdLine, IM_SHELL_EXECUTE_MODE_VERBOSE);

		this->AddToHistory(cmdLine);

		clEditBox.Clear();
	}

	void Console_c::ToggleConsole(void)
	{
		const Char_t *msg;

		/*
		FIXME
		IM_InputMapper_c *mapper = IM_LookupNode<IM_InputMapper_c>(IM_DEFAULT_PATH_NAME_INPUT_MAPPER);
		IM_ASSERT_VALID(mapper);
		*/

		//if just turning on, ignore first input
		fActive = !fActive;
		if(fActive)
		{
			fIgnoreFirstChar = true;			
			msg = "enabled";

			//FIXME
			//mapper->Disable();

			if(pclOverlay)
				pclOverlay->show();		
		}
		else
		{
			msg = "disabled";
			//FIXME
			//mapper->Enable();		
		}

		std::string tmp("Console ");
		tmp.append(msg);
		tmp.append("\n");
		Kernel_c::GetInstance().LogMessage(tmp);
	}

	void Console_c::Print(const String_c &str)
	{
		String_c line;
		size_t splitPos = 0;
			
		while(StringSplitBy(line, str, '\n', splitPos, &splitPos))
		{
			lstText.push_back(line);
		} 	
	
		fTextBufferChanged = true;		
	}

	/*
	FIXME
	void Console_c::LogProc(IM_UInt8_t , const IM_SInt8_t *text)
	{
		this->Print(text);
	}
	*/

	void Console_c::AddToHistory(const String_c &str)
	{	
		if(!lstHistory.empty() && lstHistory.front().compare(str) == 0)
			return;

		lstHistory.push_front(str);
		itPrevCmd = lstHistory.begin();
	}

	bool Console_c::GetPreviousCommand(String_c &str)
	{
		if(!lstHistory.empty() && (itPrevCmd != lstHistory.end()))
		{
			str = *itPrevCmd;
			++itPrevCmd;

			return(true);
		}

		return(false);
	}

	bool Console_c::GetNextCommand(String_c &str)
	{
		if(!lstHistory.empty() && (itPrevCmd != lstHistory.begin()))
		{		
			--itPrevCmd;
			str = *itPrevCmd;

			return(true);
		}

		return(false);
	}

	void Console_c::OnPreviousCommand(void)
	{
		String_c tmp;

		if(this->GetPreviousCommand(tmp))
		{
			fEditBoxChanged = true;			
			clEditBox.SetStr(tmp);
		}
	}

	void Console_c::OnNextCommand(void)
	{
		String_c tmp;

		if(this->GetNextCommand(tmp))
		{
			fEditBoxChanged = true;
			clEditBox.SetStr(tmp);
		}
	}

	/*
	FIXME
	IM_ErrorHandler_t IM_Console_c::InputManagerListenerProc(IM_Handler_t input, IM_Handler_t param)
	{
		IM_InputManager_c::InputManagerEvent_s *event = (IM_InputManager_c::InputManagerEvent_s *) input;

		switch(event->eType)
		{
			case IM_InputManager_c::INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
				IM_KernelPrintf(
					IM_KERNEL_LOG_NORMAL_FLAGS, 
					"[IM_Console_c::InputManagerListenerProc] Device %s attached\n", 
					event->rclDevice.GetName().c_str()
				);

				if(event->rclDevice.GetDeviceType() == IM_InputManager_c::DEVICE_KEYBOARD)			
					event->rclDevice.AddListener(clInputDeviceKeyboardListener);			

				break;

			case IM_InputManager_c::INPUT_MANAGER_EVENT_DEVICE_DETACHED:
				IM_KernelPrintf(
					IM_KERNEL_LOG_NORMAL_FLAGS, 
					"[IM_Console_c::InputManagerListenerProc] Device %s detached\n", 
					event->rclDevice.GetName().c_str()
				);
				break;
		}

		return(IM_SUCCESS);
	}

	IM_ErrorHandler_t IM_Console_c::InputDeviceKeyboardListenerProc(IM_Handler_t input, IM_Handler_t param)
	{
		IM_InputManager_c::InputEvent_s *event = static_cast<IM_InputManager_c::InputEvent_s *>(input);
	
		switch(event->eType)
		{
			case IM_InputManager_c::INPUT_EVENT_CHAR:
				if(this->IsActive())
					this->OnChar(static_cast<IM_Char_t>(event->stChar.u16Char));			
				break;

			case IM_InputManager_c::INPUT_EVENT_BUTTON:	
				if(event->stButton.eState == IM_InputManager_c::BUTTON_STATE_DOWN)
				{
					if(event->stButton.uId == CONSOLE_KEY)
						this->ToggleConsole();
					else if(this->IsActive())
					{
						if(event->stButton.uId == IM_KB_UP_ARROW)
							this->OnPreviousCommand();
						else if(event->stButton.uId == IM_KB_DOWN_ARROW)
							this->OnNextCommand();
					}
				}
				break;
		}

		return(IM_SUCCESS);
	}
	*/

	void Console_c::UpdateRenderInfo()
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

			const RenderTarget::FrameStats& stats = Render_c::GetInstance()->GetFrameStats();
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

	//
	//
	//EDIT BOX
	//
	//
	inline Console_c::EditBox_c::EditBox_c(void):
		uCursorPos(0)
	{

	}

	inline void Console_c::EditBox_c::AddChar(Char_t ch)
	{
		strStr.insert(uCursorPos, 1, ch);

		++uCursorPos;
	}


	inline void Console_c::EditBox_c::RetreatCursor(void)
	{
		if(uCursorPos > 0)
			--uCursorPos;
	}

	inline void Console_c::EditBox_c::AdvanceCursor(void)
	{
		if(uCursorPos <= strStr.length())
			++uCursorPos;
	}

	inline void Console_c::EditBox_c::Backspace(void)
	{
		if(uCursorPos > 0)
		{
			--uCursorPos;
			strStr.erase(uCursorPos, 1);
		}
	}

	inline void Console_c::EditBox_c::Clear(void)
	{		
		strStr.clear();
		uCursorPos = 0;
	}

	inline void Console_c::EditBox_c::SetStr(const String_c &str)
	{
		strStr = str;

		uCursorPos = strStr.length();
	}

	inline const String_c &Console_c::EditBox_c::GetStr(void) const
	{
		return(strStr);
	}
}