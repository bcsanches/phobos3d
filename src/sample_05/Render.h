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

#ifndef RENDER_H
#define RENDER_H

#include <PH_ContextCmd.h>
#include <PH_ContextVar.h>
#include <PH_CoreModule.h>
#include <PH_Singleton.h>
#include <PH_Window.h>

PH_DECLARE_SINGLETON_PTR(Render);

class Render_c: public Phobos::CoreModule_c
{
	PH_DECLARE_SINGLETON_METHODS(Render);

	protected:
		Render_c();

		void OnBoot();
		void OnPrepareToBoot();

	private:
		void CmdScreenshot(const Phobos::StringVector_t &args, Phobos::Context_c &);

	private:
		// =====================================================
		// PRIVATE ATTRIBUTES
		// =====================================================		
		Phobos::WindowPtr_t ipWindow;

		Phobos::ContextVar_c varRScreenX;
		Phobos::ContextVar_c varRScreenY;
		Phobos::ContextVar_c varRVSync;
		Phobos::ContextVar_c varRFullScreen;

		Phobos::ContextCmd_c cmdScreenshot;
};

#endif
