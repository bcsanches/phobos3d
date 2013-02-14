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

#include "Phobos/Exception.h"

#include <sstream>

#include "Phobos/Log.h"

namespace Phobos
{
	using namespace std;

	Exception::Exception(int errorCode, const String_t &typeName, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line):
		m_iLine(line),
		m_iError(errorCode),
		m_strTypeName(typeName),
        m_strDescription(extraInfo),
		m_strModuleName(moduleName),
		m_strFile(fileName)
	{
		LogMessage("[" + m_strTypeName + "] " + m_strDescription);
	}

	Exception::Exception(const Exception &rhs):
		m_iLine(rhs.m_iLine),
		m_iError(rhs.m_iError),
		m_strTypeName(rhs.m_strTypeName),
		m_strDescription(rhs.m_strDescription),
		m_strModuleName(rhs.m_strModuleName),
		m_strFile(rhs.m_strFile),
		m_strFullDesc(rhs.m_strFullDesc)
	{
		//empty
	}

	void Exception::operator =(const Exception &rhs)
	{
		m_iLine = rhs.m_iLine;
		m_iError = rhs.m_iError;
		m_strTypeName = rhs.m_strTypeName;
		m_strDescription = rhs.m_strDescription;
		m_strModuleName = rhs.m_strModuleName;
		m_strFile = rhs.m_strFile;
		m_strFullDesc = rhs.m_strFullDesc;
	}

	const char *Exception::what() const throw()
	{
		if(m_strFullDesc.empty())
		{
			stringstream stream;

			stream << "[" << m_strModuleName << "; " << m_strFile << ":" << m_iLine << "] " << m_strTypeName << ": " << m_strDescription << endl;

			m_strFullDesc = stream.str();
		}

		return m_strFullDesc.c_str();
	}
}
