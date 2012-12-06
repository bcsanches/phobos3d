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

#include "Gui/PH_LevelSelector.h"

#include <boost/filesystem.hpp>
#include <boost/foreach.hpp>

#include <vector>

#include <Rocket/Controls.h>
#include <Rocket/Controls/DataSource.h>

#include <PH_ContextUtils.h>
#include <PH_Console.h>
#include <PH_Kernel.h>

#include <Gui/PH_Context.h>
#include <Gui/PH_Manager.h>

#include "PH_MapLoaderFactory.h"
#include "PH_WorldManager.h"

namespace Phobos
{
	namespace Gui
	{		
		class DataGridController_c: public Rocket::Core::EventListener
		{
			public:
				DataGridController_c():
					pclSelectedRow(NULL)
				{
					//empty
				}

				void ProcessEvent(Rocket::Core::Event& event)
				{
					if(event.GetType() == "rowadd")
					{						
						Rocket::Controls::ElementDataGrid *dataGrid = dynamic_cast<Rocket::Controls::ElementDataGrid*>(event.GetCurrentElement());

						for(int i = 0, len = dataGrid->GetNumRows();i < len; ++i)
						{
							Rocket::Controls::ElementDataGridRow *row = dataGrid->GetRow(i);

							//Remove to make sure we did not add it twice
							row->RemoveEventListener("click", this);
							row->RemoveEventListener("dblclick", this);
							row->AddEventListener("click", this);
							row->AddEventListener("dblclick", this);
						}
					}
					else if(event.GetType() == "click")
					{
						this->SetSelectedRow(event.GetCurrentElement());
					}
					else if(event.GetType() == "dblclick")
					{
						this->SetSelectedRow(event.GetCurrentElement());
					}
					/*
					else if(event.GetType() == "keydown")
					{
						Rocket::Controls::ElementDataGrid *dataGrid = dynamic_cast<Rocket::Controls::ElementDataGrid*>(event.GetCurrentElement());
						if(dataGrid->GetNumRows() == 0)
								return;

						if(pclSelectedRow == NULL)
						{
							event.GetParameter<int>("key_identifier", 
							this->SetSelectedRow(dataGrid->GetRow(0));
						}
					}*/
				}

				int GetSelectedRowIndex()
				{
					if(pclSelectedRow == NULL)
						return -1;
					else
						return pclSelectedRow->GetTableRelativeIndex();
				}

			private:
				void SetSelectedRow(Rocket::Core::Element *element)
				{
					if(pclSelectedRow != NULL)
						pclSelectedRow->SetClass("selected", false);

					pclSelectedRow = dynamic_cast<Rocket::Controls::ElementDataGridRow *>(element);
					pclSelectedRow->SetClass("selected", true);					
				}

			private:
				Rocket::Controls::ElementDataGridRow *pclSelectedRow;
		};
		
		class LevelFileDataSource_c: public Rocket::Controls::DataSource
		{
			public:
				LevelFileDataSource_c(const std::list<String_c> &directories);

				virtual void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns);
				virtual int GetNumRows(const Rocket::Core::String& table);

				const String_c &GetFile(int index);

			private:
				std::vector<String_c> vecFiles;
		};

		class LevelSelectorEventListener_c: public Rocket::Core::EventListener
		{
			public:
				virtual void ProcessEvent(Rocket::Core::Event& event)
				{
					if(event.GetCurrentElement()->GetId() == "loadForm")
					{
						LevelSelector_c::GetInstance().OnLoadButtonClick();
					}
					else if(event.GetCurrentElement()->GetId() == "quitForm")
					{
						LevelSelector_c::GetInstance().OnQuitButtonClick();
					}
				}
		};

		static LevelSelectorEventListener_c clLevelSelectorEventListener_gl;
	}
}

Phobos::Gui::LevelFileDataSource_c::LevelFileDataSource_c(const std::list<String_c> &directories):
	Rocket::Controls::DataSource("LevelFileDataSource")
{		
	std::set<String_c> setExtensions;

	{
		std::list<String_c> mapFileExtensions = MapLoaderFactory_c::GetInstance().CreateMapFileExtensionsList();	

		BOOST_FOREACH(String_c &ext, mapFileExtensions)
		{
			setExtensions.insert("." + ext);
		}
	}

	for(std::list<String_c>::const_iterator it = directories.begin(), end = directories.end(); it != end; ++it)
	{
		const String_c &dir = *it;

		if(!boost::filesystem::exists(dir))
		{
			Kernel_c::GetInstance().LogStream() << "[LevelFileDataSource] Path " << dir << " does not exists.";
			continue;
		}

		if(!boost::filesystem::is_directory(dir))
		{
			Kernel_c::GetInstance().LogStream() << "[LevelFileDataSource] Path " << dir << " is not a folder.";
			continue;
		}

		boost::filesystem::directory_iterator endDirectory;
		for(boost::filesystem::directory_iterator it(dir);it != endDirectory; ++it)
		{
			boost::filesystem::path levelDirPath = it->path();

			if(!boost::filesystem::is_directory(levelDirPath))
				continue;
			
			for(boost::filesystem::directory_iterator levelDir(levelDirPath);levelDir != endDirectory; ++levelDir)
			{
				boost::filesystem::path levelFilePath = levelDir->path();

				if(!boost::filesystem::is_regular_file(levelFilePath))
					continue;

				String_c ext = levelFilePath.extension().string();
				if(setExtensions.find(ext) != setExtensions.end())
					vecFiles.push_back(levelFilePath.string());
			}			
		}
	}	
}

void Phobos::Gui::LevelFileDataSource_c::GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns)
{
	if(table == "files")
	{
		row.push_back(vecFiles[row_index].c_str());
	}
}
int Phobos::Gui::LevelFileDataSource_c::GetNumRows(const Rocket::Core::String& table)
{
	if(table== "files")
		return vecFiles.size();
	else
		return 0;
}

const Phobos::String_c &Phobos::Gui::LevelFileDataSource_c::GetFile(int index)
{
	return vecFiles[index];
}

PH_DEFINE_DEFAULT_SINGLETON(Phobos::Gui::LevelSelector);

Phobos::Gui::LevelSelector_c::LevelSelector_c():
	CoreModule_c("GuiLevelSelector"),
	cmdAddLevelPath("addLevelPath"),
	fCloseRequested(false)
{	
	cmdAddLevelPath.SetProc(PH_CONTEXT_CMD_BIND(&Phobos::Gui::LevelSelector_c::CmdAddLevelPath, this));	
}

Phobos::Gui::LevelSelector_c::~LevelSelector_c()
{
	//empty
}

void Phobos::Gui::LevelSelector_c::OnPrepareToBoot()
{
	Console_c &console = Console_c::GetInstance();

	console.AddContextCmd(cmdAddLevelPath);
}

void Phobos::Gui::LevelSelector_c::OnFixedUpdate()
{
	if(fCloseRequested)
	{
		spGuiContext.reset();
		fCloseRequested = false;
	}
}

void Phobos::Gui::LevelSelector_c::Open()
{
	fCloseRequested = false;

	upDataSource.reset( PH_NEW LevelFileDataSource_c(lstLevelPaths));
	upDataGridController.reset(PH_NEW(DataGridController_c));

	spGuiContext = Gui::Manager_c::GetInstance().CreateContext("LevelSelector");

	Rocket::Core::ElementDocument* cursor = spGuiContext->LoadMouseCursor("resources/gui/LevelSelector/cursor.rml");
	if (cursor)
		cursor->RemoveReference();

	Rocket::Core::ElementDocument* document = spGuiContext->LoadDocument("resources/gui/LevelSelector/main.rml");
	if (document)
	{
		document->SetTitle(Rocket::Core::String("Select Level"));
		document->Show();		

		document->RemoveReference();

		Rocket::Controls::ElementDataGrid *dataGrid = dynamic_cast<Rocket::Controls::ElementDataGrid*>(document->GetElementById("datagrid"));				

		int numRows = dataGrid->GetNumRows();

		dataGrid->AddEventListener("rowadd", upDataGridController.get());
		dataGrid->AddEventListener("keydown", upDataGridController.get());

		document->GetElementById("loadForm")->AddEventListener("submit", &clLevelSelectorEventListener_gl);
		document->GetElementById("quitForm")->AddEventListener("submit", &clLevelSelectorEventListener_gl);
	}
}

void Phobos::Gui::LevelSelector_c::Close()
{
	fCloseRequested = true;
}

Phobos::EscAction::Enum Phobos::Gui::LevelSelector_c::HandleEsc(Phobos::Gui::Form_c *&outForm)
{
	this->Close();

	outForm = NULL;

	return EscAction::SET_GUI;
}


void Phobos::Gui::LevelSelector_c::OnFinalize()
{
	//make sure it is destroyed before gui is shutdown
	upDataSource.reset();
	spGuiContext.reset();
}

void Phobos::Gui::LevelSelector_c::OnLoadButtonClick()
{
	int row = upDataGridController->GetSelectedRowIndex();
	if(row < 0)
		return;

	const String_c &levelFile = upDataSource->GetFile(row);
	WorldManager_c::GetInstance().LoadMap(levelFile);	
}

void Phobos::Gui::LevelSelector_c::OnQuitButtonClick()
{
	Console_c::GetInstance().Execute("quit");
}

void Phobos::Gui::LevelSelector_c::CmdAddLevelPath(const Phobos::StringVector_t &args, Phobos::Context_c &)
{
	if(args.size() < 2)
	{
		Kernel_c::GetInstance().LogMessage("[Phobos::Gui::LevelSelector_c::CmdAddLevelPath] ERROR: insuficient parameters, usage: addLevelPath <path>");

		return;
	}

	lstLevelPaths.push_back(args[1]);		
}
