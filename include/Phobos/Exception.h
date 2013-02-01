/*
Phobos 3d
January 2010
Copyright (c) 2005-2011 Bruno Sanches  http://code.google.com/p/phobos3d

This software is provided 'as-is', without any express or implied warranty.
In no event will the authors be held liable for any damages arising from the use of this software.
Permission is granted to anyone to use this software for any purpose, 
including commercial applications, and to alter it and redistribute it freely, 
subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must not claim that you wrote the original software. If you use this software in a product, an acknowledgment in the product documentation would be appreciated but is not required.
2. Altered source versions must be plainly marked as such, and must not be misrepresented as being the original software.
3. This notice may not be removed or altered from any source distribution.
*/

#ifndef PH_EXCEPTION_H
#define PH_EXCEPTION_H

#include <exception>

#include "Phobos/BaseAPI.h"
#include "Phobos/String.h"

namespace Phobos
{
	enum ExceptionTypes
	{		
		OBJECT_NOT_FOUND_EXCEPTION,
		OBJECT_ALREADY_EXISTS_EXCEPTION,
		INVALID_PARAMETER_EXCEPTION,
		INVALID_OPERATION_EXCEPTION,
		NATIVE_API_FAILED_EXCEPTION,
		FILE_NOT_FOUND_EXCEPTION,
		PARSER_EXCEPTION
	};

	class PH_BASE_API Exception: public std::exception
	{
		public:
			Exception(int errorCode, const String_t &typeName, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line);

			/** Copy constructor.
			*/
			Exception(const Exception& rhs);

			/// Needed for  compatibility with std::exception
			~Exception() throw() {}

			/** Assignment operator.
			*/
			void operator = (const Exception& rhs);
			
			/// Override std::exception::what
			const char* what() const throw();

		protected:
			unsigned int		m_iLine;
			int					m_iError;
			String_t			m_strTypeName;
			String_t			m_strDescription;
			String_t			m_strModuleName;
			String_t			m_strFile;      
			mutable String_t	m_strFullDesc;
	};

	class PH_BASE_API ObjectNotFoundException: public Exception
	{
		public:
			inline ObjectNotFoundException(int errorCode, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line):
				Exception(errorCode, "ObjectNotFoundException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_BASE_API ObjectAlreadyExistsException: public Exception
	{
		public:
			inline ObjectAlreadyExistsException(int errorCode, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line):
				Exception(errorCode, "ObjectAlreadyExistsException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_BASE_API InvalidParameterException: public Exception
	{
		public:
			inline InvalidParameterException(int errorCode, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line):
				Exception(errorCode, "InvalidParameterException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_BASE_API InvalidOperationException: public Exception
	{
		public:
			inline InvalidOperationException(int errorCode, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line):
				Exception(errorCode, "InvalidOperationException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_BASE_API NativeAPIFailedException: public Exception
	{
		public:
			inline NativeAPIFailedException(int errorCode, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line):
				Exception(errorCode, "NativeAPIFailedException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_BASE_API FileNotFoundException: public Exception
	{
		public:
			inline FileNotFoundException(int errorCode, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line):
				Exception(errorCode, "FileNotFoundException", moduleName, extraInfo, fileName, line)
				{
				}
	};

	class PH_BASE_API ParserException: public Exception
	{
		public:
			inline ParserException(int errorCode, const String_t &moduleName, const String_t &extraInfo, const char *fileName, unsigned int line):
				Exception(errorCode, "ParserException", moduleName, extraInfo, fileName, line)
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
			static ObjectNotFoundException Create(
				ExceptionErrorType_s<OBJECT_NOT_FOUND_EXCEPTION> code, 
				const String_t &moduleName, 
				const String_t& extraInfo, 
				const char* file, long line)
			{
				return ObjectNotFoundException(code.error, moduleName, extraInfo, file, line);
			}

			static ObjectAlreadyExistsException Create(
				ExceptionErrorType_s<OBJECT_ALREADY_EXISTS_EXCEPTION> code, 
				const String_t &moduleName, 
				const String_t& extraInfo, 
				const char* file, long line)
			{
				return ObjectAlreadyExistsException(code.error, moduleName, extraInfo, file, line);
			}

			static InvalidParameterException Create(
				ExceptionErrorType_s<INVALID_PARAMETER_EXCEPTION> code, 
				const String_t &moduleName, 
				const String_t& extraInfo, 
				const char* file, long line)
			{
				return InvalidParameterException(code.error, moduleName, extraInfo, file, line);
			}

			static InvalidOperationException Create(
				ExceptionErrorType_s<INVALID_OPERATION_EXCEPTION> code, 
				const String_t &moduleName, 
				const String_t& extraInfo, 
				const char* file, long line)
			{
				return InvalidOperationException(code.error, moduleName, extraInfo, file, line);
			}

			static NativeAPIFailedException Create(
				ExceptionErrorType_s<NATIVE_API_FAILED_EXCEPTION> code, 
				const String_t &moduleName, 
				const String_t& extraInfo, 
				const char* file, long line)
			{
				return NativeAPIFailedException(code.error, moduleName, extraInfo, file, line);
			}

			static FileNotFoundException Create(
				ExceptionErrorType_s<FILE_NOT_FOUND_EXCEPTION> code, 
				const String_t &moduleName, 
				const String_t &extraInfo, 
				const char* file, long line)
			{
				return FileNotFoundException(code.error, moduleName, extraInfo, file, line);
			}

			static ParserException Create(
				ExceptionErrorType_s<PARSER_EXCEPTION> code, 
				const String_t &moduleName, 
				const String_t &extraInfo, 
				const char* file, long line)
			{
				return ParserException(code.error, moduleName, extraInfo, file, line);
			}
	};
}

#	ifndef PH_RAISE
#		define PH_RAISE(error, module, extraInfo)	\
		throw Phobos::ExceptionFactory_c::Create(Phobos::ExceptionErrorType_s<error>(), module, extraInfo, __FILE__, __LINE__);
#	endif

#endif
