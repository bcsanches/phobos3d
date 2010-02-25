/*
Phobos 3d
  version 0.0.1, January 2010

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

#ifndef PH_EXCEPTION_H
#define PH_EXCEPTION_H

#include <exception>

#include "PH_KernelAPI.h"
#include "PH_String.h"

namespace Phobos
{
	enum ExceptionTypes_e
	{
		ERROR_OBJECT_NOT_FOUND,
		ERROR_OBJECT_ALREADY_EXISTS,
		ERROR_INVALID_PARAMETER,
		ERROR_INVALID_OPERATION
	};

	class PH_KERNEL_API Exception_c: public std::exception
	{
		public:
			Exception_c(int errorCode, const String_c &typeName, const String_c &moduleName, const String_c &extraInfo, const char *fileName, unsigned int line);

			/** Copy constructor.
			*/
			Exception_c(const Exception_c& rhs);

			/// Needed for  compatibility with std::exception
			~Exception_c() throw() {}

			/** Assignment operator.
			*/
			void operator = (const Exception_c& rhs);
			
			/// Override std::exception::what
			const char* what() const throw();

		protected:
			unsigned int		iLine;
			int					iError;
			String_c			strTypeName;
			String_c			strDescription;
			String_c			strModuleName;
			String_c			strFile;      
			mutable String_c	strFullDesc;
	};

	class PH_KERNEL_API ObjectNotFoundException_c: public Exception_c
	{
		public:
			inline ObjectNotFoundException_c(int errorCode, const String_c &moduleName, const String_c &extraInfo, const char *fileName, unsigned int line):
				Exception_c(errorCode, "ObjectNotFoundException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_KERNEL_API ObjectAlreadyExistsException_c: public Exception_c
	{
		public:
			inline ObjectAlreadyExistsException_c(int errorCode, const String_c &moduleName, const String_c &extraInfo, const char *fileName, unsigned int line):
				Exception_c(errorCode, "ObjectAlreadyExistsException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_KERNEL_API InvalidParameterException_c: public Exception_c
	{
		public:
			inline InvalidParameterException_c(int errorCode, const String_c &moduleName, const String_c &extraInfo, const char *fileName, unsigned int line):
				Exception_c(errorCode, "InvalidParameterException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_KERNEL_API InvalidOperationException_c: public Exception_c
	{
		public:
			inline InvalidOperationException_c(int errorCode, const String_c &moduleName, const String_c &extraInfo, const char *fileName, unsigned int line):
				Exception_c(errorCode, "InvalidOperationException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	/**
		Template for creating a unique type for implementing a static factory
	*/
	template <int value>
	struct ExceptionErrorType_s
	{
		enum 
		{ 
			error = value 
		};
	};

	class ExceptionFactory_c
	{
		public:
			static ObjectNotFoundException_c Create(
				ExceptionErrorType_s<ERROR_OBJECT_NOT_FOUND> code, 
				const String_c &moduleName, 
				const String_c& extraInfo, 
				const char* file, long line)
			{
				return ObjectNotFoundException_c(code.error, moduleName, extraInfo, file, line);
			}

			static ObjectAlreadyExistsException_c Create(
				ExceptionErrorType_s<ERROR_OBJECT_ALREADY_EXISTS> code, 
				const String_c &moduleName, 
				const String_c& extraInfo, 
				const char* file, long line)
			{
				return ObjectAlreadyExistsException_c(code.error, moduleName, extraInfo, file, line);
			}

			static InvalidParameterException_c Create(
				ExceptionErrorType_s<ERROR_INVALID_PARAMETER> code, 
				const String_c &moduleName, 
				const String_c& extraInfo, 
				const char* file, long line)
			{
				return InvalidParameterException_c(code.error, moduleName, extraInfo, file, line);
			}

			static InvalidOperationException_c Create(
				ExceptionErrorType_s<ERROR_INVALID_OPERATION> code, 
				const String_c &moduleName, 
				const String_c& extraInfo, 
				const char* file, long line)
			{
				return InvalidOperationException_c(code.error, moduleName, extraInfo, file, line);
			}
	};
}

#	ifndef PH_RAISE
#		define PH_RAISE(error, module, extraInfo)	\
		throw Phobos::ExceptionFactory_c::Create(ExceptionErrorType_s<error>(), module, extraInfo, __FILE__, __LINE__);
#	endif

#endif
