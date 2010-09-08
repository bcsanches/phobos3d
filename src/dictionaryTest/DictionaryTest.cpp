/*
Phobos 3d
  September 2010

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

#include <boost/test/unit_test.hpp>

#include <PH_DictionaryManager.h>
#include <PH_Kernel.h>

using namespace Phobos;
using namespace std;

struct KernelInstance_s
{
	KernelInstance_s()
	{
		Kernel_c::CreateInstance("dictionarytest.log");	
		DictionaryManager_c::CreateInstance();
	}

	~KernelInstance_s()
	{		
		DictionaryManager_c::ReleaseInstance();

		Kernel_c::ReleaseInstance();
	}
};

BOOST_AUTO_TEST_CASE(dictionary_basic)
{
	KernelInstance_s instance;

	stringstream stream;

	stream << "EntityDef InfoPlayerStart" << endl;
	stream << "{" << endl;
	stream <<	"ClassName=Entity" << endl;
	stream <<	"Health=100" << endl;
	stream <<	"Description=\"bla bla\"" << endl;
	stream << "}" << endl;
	stream << endl << endl;
	stream << "RigidBody PlayerCapsule" << endl;
	stream << "{" << endl;
	stream <<	"Height=1.0" << endl;
	stream <<	"Radius=2.0" << endl;
	stream << "}";

	DictionaryManagerPtr_t manager = DictionaryManager_c::GetInstance();

	manager->Load(stream);
}