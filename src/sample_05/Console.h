/*
Phobos3d
March 2012
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

#ifndef CONSOLE_H
#define CONSOLE_H

#include <Phobos/Engine/Console.h>

PH_DECLARE_NODE_PTR(Console);

class Console: 
	public Phobos::Engine::Console		
{
	public:
		// =====================================================
		// PUBLIC METHODS
		// =====================================================
		static std::unique_ptr<Phobos::Engine::Module> CreateInstance(const Phobos::String_t &name);

		Console(const Phobos::String_t &name);
		~Console();

	protected:
		// =====================================================
		// PROTECTED METHODS
		// =====================================================			
		void OnUpdate();
		void OnRenderReady();

		virtual void OnToggleConsole();
		virtual void OnEditBoxChanged();
		virtual void OnTextListChanged();
	
	private:
		// =====================================================
		// PRIVATE ATTRIBUTES
		// =====================================================
		bool m_fEditBoxChanged;
		bool m_fTextBufferChanged;
};	

#endif
