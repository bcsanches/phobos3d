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

namespace Phobos
{
	namespace Gui
	{
		class LevelFileDataSource_c: public Rocket::Controls::DataSource
		{
			public:
				LevelFileDataSource_c(const std::list<String_c> &directories);

				virtual void GetRow(Rocket::Core::StringList& row, const Rocket::Core::String& table, int row_index, const Rocket::Core::StringList& columns);
				virtual int GetNumRows(const Rocket::Core::String& table);

			private:
				std::vector<String_c> vecFiles;
		};
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

PH_DEFINE_DEFAULT_SINGLETON(Phobos::Gui::LevelSelector);

Phobos::Gui::LevelSelector_c::LevelSelector_c():
	CoreModule_c("GuiLevelSelector"),
	cmdAddLevelPath("addLevelPath")
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

void Phobos::Gui::LevelSelector_c::Open()
{
	spDataSource.reset( PH_NEW LevelFileDataSource_c(lstLevelPaths));

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
	}
}

void Phobos::Gui::LevelSelector_c::Close()
{
	spGuiContext.reset();
}

void Phobos::Gui::LevelSelector_c::OnFinalize()
{
	//make sure it is destroyed before gui is shutdown
	spDataSource.reset();
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
