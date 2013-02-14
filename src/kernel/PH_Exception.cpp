/*
Phobos 3d
January 2010
Copyright (c) 2005-2013 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#include "PH_Exception.h"

#include <sstream>

#include "PH_Kernel.h"

namespace Phobos
{
	using namespace std;

	Exception_c::Exception_c(int errorCode, const String_c &typeName, const String_c &moduleName, const String_c &extraInfo, const char *fileName, unsigned int line):
		iLine(line),
		iError(errorCode),
		strTypeName(typeName),
        strDescription(extraInfo),
		strModuleName(moduleName),
		strFile(fileName)
	{
		Kernel_c::GetInstance().LogMessage("[" + strTypeName + "] " + strDescription);
	}

	Exception_c::Exception_c(const Exception_c &rhs):
		iLine(rhs.iLine),
		iError(rhs.iError),
		strTypeName(rhs.strTypeName),
		strDescription(rhs.strDescription),
		strModuleName(rhs.strModuleName),
		strFile(rhs.strFile),
		strFullDesc(rhs.strFullDesc)
	{
		//empty
	}

	void Exception_c::operator =(const Exception_c &rhs)
	{
		iLine = rhs.iLine;
		iError = rhs.iError;
		strTypeName = rhs.strTypeName;
		strDescription = rhs.strDescription;
		strModuleName = rhs.strModuleName;
		strFile = rhs.strFile;
		strFullDesc = rhs.strFullDesc;
	}

	const char *Exception_c::what() const throw()
	{
		if(strFullDesc.empty())
		{
			stringstream stream;

			stream << "[" << strModuleName << "; " << strFile << ":" << iLine << "] " << strTypeName << ": " << strDescription << endl;

			strFullDesc = stream.str();
		}

		return strFullDesc.c_str();
	}
}