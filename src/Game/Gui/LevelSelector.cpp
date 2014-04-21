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

#include "Phobos/Game/Gui/LevelSelector.h"

#include <boost/filesystem.hpp>

#include <vector>

#include <Rocket/Controls.h>
#include <Rocket/Controls/DataSource.h>

#include <Phobos/Shell/Utils.h>
#include <Phobos/Engine/Console.h>

#include <Phobos/Engine/Gui/Context.h>
#include <Phobos/Engine/Gui/Manager.h>

#include "Phobos/Game/MapLoaderFactory.h"
#include "Phobos/Game/WorldManager.h"

namespace Phobos
{
	namespace Game
	{
		namespace Gui
		{		
			class DataGridController: public Rocket::Core::EventListener
			{
				public:
					DataGridController():
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
		
			class LevelFileDataSource: public Rocket::Controls::DataSource
			{
				public:
					LevelFileDataSource(const std::list<String_t> &directories);

					virtual void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns);
					virtual int GetNumRows(const Rocket::Core::String& table);

					const String_t &GetFile(int index);

				private:
					std::vector<String_t> vecFiles;
			};

			class LevelSelectorEventListener: public Rocket::Core::EventListener
			{
				public:
					virtual void ProcessEvent(Rocket::Core::Event& event)
					{
						if(event.GetCurrentElement()->GetId() == "loadForm")
						{
							LevelSelector::GetInstance().OnLoadButtonClick();
						}
						else if(event.GetCurrentElement()->GetId() == "quitForm")
						{
							LevelSelector::GetInstance().OnQuitButtonClick();
						}
					}
			};

			static LevelSelectorEventListener clLevelSelectorEventListener_gl;
		}
	}
}

Phobos::Game::Gui::LevelFileDataSource::LevelFileDataSource(const std::list<String_t> &directories):
	Rocket::Controls::DataSource("LevelFileDataSource")
{		
	std::set<String_t> setExtensions;

	{
		auto mapFileExtensions = MapLoaderFactory::GetInstance().CreateMapFileExtensionsList();	

		for(String_t &ext : mapFileExtensions)
		{
			setExtensions.insert("." + ext);
		}
	}

	for(auto it = directories.begin(), end = directories.end(); it != end; ++it)
	{
		const String_t &dir = *it;

		if(!boost::filesystem::exists(dir))
		{
			LogMakeStream() << "[LevelFileDataSource] Path " << dir << " does not exists.";
			continue;
		}

		if(!boost::filesystem::is_directory(dir))
		{
			LogMakeStream() << "[LevelFileDataSource] Path " << dir << " is not a folder.";
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

				String_t ext = levelFilePath.extension().string();
				if(setExtensions.find(ext) != setExtensions.end())
					vecFiles.push_back(levelFilePath.string());
			}			
		}
	}	
}

void Phobos::Game::Gui::LevelFileDataSource::GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns)
{
	if(table == "files")
	{
		row.push_back(vecFiles[row_index].c_str());
	}
}
int Phobos::Game::Gui::LevelFileDataSource::GetNumRows(const Rocket::Core::String& table)
{
	if(table== "files")
		return vecFiles.size();
	else
		return 0;
}

const Phobos::String_t &Phobos::Game::Gui::LevelFileDataSource::GetFile(int index)
{
	return vecFiles[index];
}

PH_DEFINE_DEFAULT_SINGLETON2(Phobos::Game::Gui::LevelSelector, Engine::Console &);

Phobos::Game::Gui::LevelSelector::LevelSelector(Engine::Console &console):
	Module("GuiLevelSelector"),
	m_cmdAddLevelPath("addLevelPath"),
	m_fCloseRequested(false)
{	
	m_cmdAddLevelPath.SetProc(PH_CONTEXT_CMD_BIND(&Phobos::Game::Gui::LevelSelector::CmdAddLevelPath, this));	

	console.AddContextCommand(m_cmdAddLevelPath);
}

Phobos::Game::Gui::LevelSelector::~LevelSelector()
{
	//empty
}

void Phobos::Game::Gui::LevelSelector::OnFixedUpdate()
{
	if(m_fCloseRequested)
	{
		m_spGuiContext.reset();
		m_fCloseRequested = false;
	}
}

void Phobos::Game::Gui::LevelSelector::Open()
{
	m_fCloseRequested = false;

	m_upDataSource.reset( PH_NEW LevelFileDataSource(m_lstLevelPaths));
	m_upDataGridController.reset(PH_NEW(DataGridController));

	m_spGuiContext = Engine::Gui::Manager::GetInstance().CreateContext("LevelSelector");

	Rocket::Core::ElementDocument* cursor = m_spGuiContext->LoadMouseCursor("resources/gui/LevelSelector/cursor.rml");
	if (cursor)
		cursor->RemoveReference();

	Rocket::Core::ElementDocument* document = m_spGuiContext->LoadDocument("resources/gui/LevelSelector/main.rml");
	if (document)
	{
		document->SetTitle(Rocket::Core::String("Select Level"));
		document->Show();		

		document->RemoveReference();

		Rocket::Controls::ElementDataGrid *dataGrid = dynamic_cast<Rocket::Controls::ElementDataGrid*>(document->GetElementById("datagrid"));				

		int numRows = dataGrid->GetNumRows();

		dataGrid->AddEventListener("rowadd", m_upDataGridController.get());
		dataGrid->AddEventListener("keydown", m_upDataGridController.get());

		document->GetElementById("loadForm")->AddEventListener("submit", &clLevelSelectorEventListener_gl);
		document->GetElementById("quitForm")->AddEventListener("submit", &clLevelSelectorEventListener_gl);
	}
}

void Phobos::Game::Gui::LevelSelector::Close()
{
	m_fCloseRequested = true;
}

Phobos::Engine::EscAction Phobos::Game::Gui::LevelSelector::HandleEsc(Phobos::Engine::Gui::Form *&outForm)
{
	this->Close();

	outForm = NULL;

	return Engine::EscAction::SET_GUI;
}


void Phobos::Game::Gui::LevelSelector::OnFinalize()
{
	//make sure it is destroyed before gui is shutdown
	m_upDataSource.reset();
	m_spGuiContext.reset();
}

void Phobos::Game::Gui::LevelSelector::OnLoadButtonClick()
{
	int row = m_upDataGridController->GetSelectedRowIndex();
	if(row < 0)
		return;

	const String_t &levelFile = m_upDataSource->GetFile(row);
	WorldManager::GetInstance().LoadMap(levelFile);	
}

void Phobos::Game::Gui::LevelSelector::OnQuitButtonClick()
{
	Engine::Console::GetInstance().Execute("quit");
}

void Phobos::Game::Gui::LevelSelector::CmdAddLevelPath(const Shell::StringVector_t &args, Shell::Context &)
{
	if(args.size() < 2)
	{
		LogMessage("[Phobos::Game::Gui::LevelSelector::CmdAddLevelPath] ERROR: insuficient parameters, usage: addLevelPath <path>");

		return;
	}

	m_lstLevelPaths.push_back(args[1]);		
}
