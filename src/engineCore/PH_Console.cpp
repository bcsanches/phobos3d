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

#include <boost/foreach.hpp>

#include <PH_Button.h>
#include <PH_ContextUtils.h>
#include <PH_Dictionary.h>
#include <PH_DictionaryManager.h>
#include <PH_Error.h>
#include <PH_Exception.h>
#include <PH_InputActions.h>
#include <PH_InputEvent.h>
#include <PH_InputManager.h>
#include <PH_Kernel.h>
#include <PH_Thumb.h>
#include <PH_Path.h>

#include <OgreOverlayContainer.h>
#include <OgreOverlayElement.h>
#include <OgreOverlayManager.h>
#include <OgreSceneManager.h>

#include "PH_Core.h"
#include "PH_Render.h"
#include "PH_OgreUtils.h"

#define CONSOLE_KEY '`'

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
		pclRect(NULL),	
		pclTextBox(NULL),
		pclOverlay(NULL),
		pclRenderInfoOverlay(NULL),
		fpHeight(0),
		lstText(CONSOLE_LINE_COUNT),
		lstHistory(CONSOLE_HISTORY_COUNT),
		varMaterialName("dvConsoleMaterialName", "PH_Console/Background"),
		varShowRenderInfo("dvShowRenderInfo", "0"),
		cmdLs("ls"),
		cmdCd("cd"),
		cmdDumpTable("dumpTable"),
		fIgnoreFirstChar(false),
		fIgnoredLastChar(false),
		fActive(true),
		strCurrentNodePathName("/")
	{
		Kernel_c &kernel = Kernel_c::GetInstance();
		
		InputManager_c::CreateInstance("InputManager")->AddListener(*this);		

		Kernel_c::GetInstance().AddLogListener(*this);

		ipInputMapper = InputMapper_c::Create("InputMapper", clContext);
		ipInputMapper->Disable();

		this->AddChild(ipInputMapper);

		cmdLs.SetProc(PH_CONTEXT_CMD_BIND(&Console_c::CmdLs, this));
		cmdCd.SetProc(PH_CONTEXT_CMD_BIND(&Console_c::CmdCd, this));
		cmdDumpTable.SetProc(PH_CONTEXT_CMD_BIND(&Console_c::CmdDumpTable, this));

		clContext.AddContextVar(varMaterialName);
		clContext.AddContextVar(varShowRenderInfo);	

		clContext.AddContextCmd(cmdLs);
		clContext.AddContextCmd(cmdCd);		
		clContext.AddContextCmd(cmdDumpTable);
	}

	Console_c::~Console_c(void)
	{		
		InputManager_c::ReleaseInstance();
	}

	void Console_c::OnFixedUpdate()
	{
		InputManager_c::GetInstance()->Update();
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
			fIgnoredLastChar = true;
			return;
		}

		if((fIgnoredLastChar) && (ch == CONSOLE_KEY))
		{
			fIgnoredLastChar = false;
			return;
		}

		fIgnoredLastChar = false;

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
		
		Kernel_c::GetInstance().LogStream() << "> " << cmdLine;

		clContext.Execute(cmdLine);		

		this->AddToHistory(cmdLine);

		clEditBox.Clear();
	}

	void Console_c::ToggleConsole(void)
	{
		const Char_t *msg;		

		//if just turning on, ignore first input
		fActive = !fActive;
		if(fActive)
		{
			fIgnoreFirstChar = true;			
			msg = "enabled";

			ipInputMapper->Disable();

			if(pclOverlay)
				pclOverlay->show();		
		}
		else
		{
			msg = "disabled";

			ipInputMapper->Enable();	
		}

		std::string tmp("Console ");
		tmp.append(msg);		
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
		
	void Console_c::Message(const String_c &msg)
	{
		this->Print(msg);
	}

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
	
	void Console_c::InputManagerEvent(const InputManagerEvent_s &event)
	{		
		std::stringstream stream;

		switch(event.eType)
		{
			case INPUT_MANAGER_EVENT_DEVICE_ATTACHED:
				stream << "[Console_c::InputManagerEvent] Device " << event.ipDevice->GetName() << " attached.";								
				if(event.ipDevice->GetDeviceType() == INPUT_DEVICE_KEYBOARD)				
					event.ipDevice->AddListener(*this);			
				break;

			case INPUT_MANAGER_EVENT_DEVICE_DETACHED:
				stream << "[Console_c::InputManagerEvent] Device " << event.ipDevice->GetName() << " detached.";
				break;
		}

		Kernel_c::GetInstance().LogMessage(stream.str());
	}

	void Console_c::InputEvent(const InputEvent_s &event)
	{			
		switch(event.eType)
		{
			case INPUT_EVENT_CHAR:
				if(this->IsActive())
					this->OnChar(static_cast<Char_t>(event.stChar.u16Char));			
				break;

			case INPUT_EVENT_BUTTON:	
				if(event.stButton.eState == BUTTON_STATE_DOWN)
				{
					if(event.stButton.uId == CONSOLE_KEY)
						this->ToggleConsole();
					else if(this->IsActive())
					{
						if(event.stButton.uId == KB_UP_ARROW)
							this->OnPreviousCommand();
						else if(event.stButton.uId == KB_DOWN_ARROW)
							this->OnNextCommand();
					}
				}
				break;
		}
	}
	
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
		
	void Console_c::CmdLs(const StringVector_t &args, Context_c &)
	{	
		Kernel_c	&kernel = Kernel_c::GetInstance();
		NodePtr_t	currentNode;

		if(args.size() > 1)
		{
			Path_c path(args[1]);			

			if(path.IsRelative())
			{
				path = strCurrentNodePathName;
				path.AddName(args[1]);
			}

			try
			{
				currentNode = kernel.LookupObject(path);
			}
			catch(ObjectNotFoundException_c &)
			{
				kernel.LogStream() << "[Console_c::CmdLs] Invalid path: " << path.GetStr();

				return;
			}
		}
		else
		{
			try
			{
				currentNode = kernel.LookupObject(Path_c(strCurrentNodePathName));
			}
			catch(ObjectNotFoundException_c &)
			{			
				kernel.LogStream() << "[Console_c::CmdLs] Invalid node (" << strCurrentNodePathName << "), going to root";

				strCurrentNodePathName = "/";				
				return;
			}
		}	
		
		std::stringstream stream;

		stream << "\t.\n\t..\n";
		BOOST_FOREACH(const Node_c::NodeMapPair_t &pair, currentNode->GetNodes())
		{
			stream << "\t" << pair.second->GetName() << "\n";
		}

		kernel.LogMessage(stream.str());			
	}

	void Console_c::CmdCd(const StringVector_t &args, Context_c &)
	{
		if(args.size() == 2)			
		{		
			Kernel_c	&kernel = Kernel_c::GetInstance();
			NodePtr_t	currentNode;
			Path_c		path(args[1]);
			
			if(path.IsRelative())
			{		
				path = strCurrentNodePathName;
				path.AddName(args[1]);
			}

			try
			{
				currentNode = kernel.LookupObject(path);						
				currentNode->GetThisPath(path);

				//if we are at root, we set the path manually, otherwise the path will be empty
				if(currentNode->GetParent() == NULL)
					strCurrentNodePathName = '/';
				else
					strCurrentNodePathName = path.GetStr();		
			}
			catch(const Exception_c &)
			{							
				kernel.LogStream() << "[Console_c::CmdCd] Invalid dir " << args[1];
			}
		}	
		else
			Kernel_c::GetInstance().LogMessage("[Console_c::CmdCd] Insuficient parameters");		
	}

	void Console_c::CmdDumpTable(const StringVector_t &args, Context_c &)
	{
		Log_c::Stream_c stream = Kernel_c::GetInstance().LogStream();

		try
		{
			DictionaryPtr_t dict;
			if(args.size() == 2)
			{
				dict = DictionaryManager_c::GetInstance()->GetDictionary(Path_c(args[1]));
				stream << "Values on " << args[1] << '\n';
			}
			else if(args.size() == 3)
			{
				dict = DictionaryManager_c::GetInstance()->GetDictionary(args[1], args[2]);
				stream << "Values on " << args[2];
			}
			else
			{
				stream << "[Console_c::CmdDumpTable] Insuficient parameters, usage: dumpTable <path>\n";
				return;
			}
			
			for(Dictionary_c::StringMapConstIterator_t it = dict->begin(), end = dict->end(); it != end; ++it)
			{
				stream << it->first << " = " << it->second << "\n";
			}
		}
		catch(std::exception &e)
		{
			stream << e.what();
		}
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
