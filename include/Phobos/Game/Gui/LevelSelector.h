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

#ifndef PH_GUI_LEVEL_SELECTOR_H
#define PH_GUI_LEVEL_SELECTOR_H

#include <list>

#include <Phobos/Engine/ConsoleFwd.h>
#include <Phobos/Engine/Module.h>
#include <Phobos/Singleton.h>
#include <Phobos/Shell/Command.h>

#include <Phobos/Engine/Gui/ContextFwd.h>
#include <Phobos/Engine/Gui/Form.h>

#include "Phobos/Game/GameAPI.h"

namespace Phobos
{
	namespace Game
	{
		namespace Gui
		{
			PH_DECLARE_SINGLETON_PTR(LevelSelector);

			class DataGridController;
			class LevelFileDataSource;		

			class PH_GAME_API LevelSelector: public Engine::Module, public Engine::Gui::Form
			{
				PH_DECLARE_SINGLETON_METHODS2(LevelSelector, Engine::Console &);

				public:					
					virtual void Open();
					virtual void Close();

					virtual Engine::EscAction HandleEsc(Engine::Gui::Form *&outForm);				

				protected:										
					virtual void OnFinalize() override;
					virtual void OnFixedUpdate() override;

				private:
					friend class LevelSelectorEventListener;

					LevelSelector(Engine::Console &);
					~LevelSelector();

					void OnLoadButtonClick();
					void OnQuitButtonClick();

					void CmdAddLevelPath(const Shell::StringVector_t &args, Shell::Context &);

				private:
					std::list<Phobos::String_t> m_lstLevelPaths;

					Engine::Gui::ContextPtr_t	m_spGuiContext;

					Shell::Command		m_cmdAddLevelPath;

					bool				m_fCloseRequested;

					std::unique_ptr<LevelFileDataSource> m_upDataSource;
					std::unique_ptr<DataGridController> m_upDataGridController;
			};
		}
	}
}


#endif