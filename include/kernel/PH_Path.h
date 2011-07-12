/*
Phobos 3d
  January 2010

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

#ifndef PH_PATH_H
#define PH_PATH_H

#include <ostream>

#include "PH_Error.h"
#include "PH_String.h"

namespace Phobos
{
	/**
		This class is used to represent an unix style path name.

		Every path stored by this class does not have a slash at the end, example:

			sys/video
			sys/video/tex.btx
			/sys/video/tex.btx

	*/
	class PH_KERNEL_API Path_c
	{
		public:			
			class Iterator_c
			{
				public:
					inline Iterator_c(void);
					inline Iterator_c(const Iterator_c &other);

					inline ~Iterator_c(void);				

					inline bool Init(const Path_c &path, String_c *);
					inline bool InitAtEnd(const Path_c &path, String_c *);

					inline bool GetNext(String_c *);
					inline bool GetPrev(String_c *);

					inline bool GetCurrent(String_c &out);					

					inline Iterator_c &operator=(const Iterator_c &other);

				private:	
					inline void Contruct(void);

				private:
					const Path_c *pclPath;

					unsigned int uStart;
					unsigned int uEnd;
			};


			Path_c(void);
			explicit Path_c(const Char_t str[]);
			explicit Path_c(const String_c &str);	

			Path_c &operator=(const Path_c &rhs);
			Path_c &operator=(const String_c &rhs);
			Path_c &operator=(const Char_t str[]);

			void AddName(const String_c &name);
			void Add(const Path_c &path);		

			bool SetSubPath(Iterator_c it);

			bool HasExtension(void) const;

			bool GetExtension(String_c &out) const;
			bool GetFileName(String_c &out) const;		
			inline bool GetFileName(Path_c &out) const;		

			size_t GetNodeCount(void) const;

			bool ExtractPathAndFilename(Path_c *path, Path_c *fileName) const;

			void Clear(void);

			inline bool IsRelative(void) const;
			inline bool IsOnlyRoot(void) const;		

			inline const String_c &GetStr(void) const;

			inline Iterator_c GetBegin(void) const;
			inline Iterator_c GetEnd(void) const;

			//inline operator const IM_Char_t *() const;
			inline operator const String_c *() const;
			inline operator const String_c &() const;

			bool StripExtension(void);
			void SetExtension(const String_c &newExtension);

			static bool GetExtension(String_c &out, const String_c &path);
			static void ConvertToPlatform(String_c &out, const String_c &in);

		private:
			static bool FindExtensionPos(size_t &out, const String_c &path);

			void FixPath();

			static void FixPathSlash(String_c &path);

		private:
			String_c strPath;		

			friend class Path_c::Iterator_c;

	};

	inline bool Path_c::IsOnlyRoot(void) const
	{	
		return(((strPath.length() == 1) && (strPath[0] == '/') ? true : false));
	}

	inline bool Path_c::IsRelative(void) const
	{
		return(((!strPath.empty()) && (strPath[0] == '/')) ? false : true);
	}

	inline const String_c &Path_c::GetStr(void) const
	{
		return(strPath);
	}

	inline Path_c::Iterator_c Path_c::GetBegin(void) const
	{
		Iterator_c it;

		it.Init(*this, NULL);

		return(it);
	}

	inline Path_c::Iterator_c Path_c::GetEnd(void) const
	{
		Iterator_c it;

		it.InitAtEnd(*this, NULL);

		return(it);
	}

	/*
	inline Path_c::operator const IM_Char_t *() const
	{
		return(strPath.c_str());
	}
	*/

	inline Path_c::operator const String_c *() const
	{
		return(&strPath);
	}

	inline Path_c::operator const String_c &() const
	{
		return(strPath);
	}

	// =====================================================
	// ITERATOR
	// =====================================================

	inline void Path_c::Iterator_c::Contruct(void)
	{
		pclPath = NULL;

		uStart = 0;
		uEnd = 0;
	}

	inline Path_c::Iterator_c::Iterator_c(void)
	{
		this->Contruct();
	}

	inline Path_c::Iterator_c::Iterator_c(const Iterator_c &other)
	{
		this->Contruct();

		*this = other;
	}

	inline Path_c::Iterator_c::~Iterator_c(void)
	{

	}	

	inline Path_c::Iterator_c &Path_c::Iterator_c::operator=(const Iterator_c &other)
	{
		if(&other == this)
			return(*this);

		pclPath = other.pclPath;
		uStart = other.uStart;
		uEnd = other.uEnd;		

		return(*this);
	}

	inline bool Path_c::Iterator_c::GetCurrent(String_c &out)
	{
		if(uEnd == uStart)
		{
			//if(pclPath->strPath[uEnd] == '\0')
			if(pclPath->strPath.length() <= uEnd)
				return(false);		
		}

		//out.SetSub(&pclPath->strPath, uStart, uEnd - uStart);
		out.assign(pclPath->strPath, uStart, uEnd - uStart);

		return(true);
	}


	inline bool Path_c::Iterator_c::InitAtEnd(const Path_c &path, String_c *out)
	{		
		pclPath = &path;
		uStart = uEnd = (size_t) pclPath->GetStr().length();

		return(this->GetPrev(out));
	}

	inline bool Path_c::Iterator_c::Init(const Path_c &path, String_c *out)
	{		
		pclPath = &path;
		uStart = uEnd = 0;

		return(this->GetNext(out));
	}

	inline bool Path_c::Iterator_c::GetNext(String_c *str)
	{
		PH_ASSERT_MSG(pclPath != NULL, "Iterator not initialized");

		//we specially allow null strings because of path begin and end iterators build
		//IM_ASSERT(str != NULL);

		Char_t ch;
		const Char_t *cstr = pclPath->strPath.c_str();

		uStart = uEnd;
		for(;;)
		{
			ch = cstr[uEnd];
			switch(ch)
			{
				case '/':
					if(uEnd == uStart)
					{
						++uEnd;
						++uStart;
					}
					else
					{
						if(str)
							//str->SetSub(&pclPath->strPath, uStart, uEnd - uStart);
							str->assign(pclPath->strPath, uStart, uEnd-uStart);

						return(true);
					}
					break;

				//we reached the end of the path?
				case '\0':
					if(uEnd == uStart)
						return(false);

					if(str)
						//str->SetSub(&pclPath->strPath, uStart, uEnd - uStart);
						str->assign(pclPath->strPath, uStart, uEnd-uStart);
					return(true);
					break;

				default:
					++uEnd;
					break;
			}
		}
	}

	//FIXME doenst work and is useless
	inline bool Path_c::Iterator_c::GetPrev(String_c *out)
	{
		PH_ASSERT_MSG(pclPath != NULL, "Please initialize iterator first");

		PH_ASSERT_MSG(0, "NOT TESTED");

		Char_t ch;
			
		uEnd = uStart;

		if(uStart > 0)
			--uStart;

		const Char_t *cstr = pclPath->strPath.c_str();
		for(;;) 
		{
			ch = cstr[uStart];

			switch(ch)
			{
				case '/':
					if(uEnd == uStart)
					{
						if(uStart > 0)
						{
							--uStart;
							continue;
						}
						//else only root path
						else 
						{
							//we fail
							return(false);
						}

					}
					else
					{
						if(uStart > 0)
							++uStart;

						if(out)
							//out->SetSub(&pclPath->strPath, uStart, uEnd - uStart);
							out->assign(pclPath->strPath, uStart, uEnd - uStart);

						return(true);
					}
					break;

				//are we starting now?
				case '\0':
					//empty path?
					if(uStart == 0)
						return(false);
					else
						--uStart;
					break;						

				default:
					//we reached start?
					if(uStart == 0)
					{
						//ok, return the first node
						if(out != NULL)
							//out->SetSub(&pclPath->strPath, uStart, uEnd - uStart);
							out->assign(pclPath->strPath, uStart, uEnd - uStart);

						return(true);
					}
					--uStart;
					break;
			}
		}
	}


	inline bool Path_c::GetFileName(Path_c &out) const
	{
		String_c str;

		if(this->GetFileName(str))
		{
			out = str;

			return true;
		}

		return false;		
	}

	inline std::ostream& operator<< (std::ostream &out, const Path_c &path)
	{
		out << path.GetStr();

		return out;
	}
}

#endif
