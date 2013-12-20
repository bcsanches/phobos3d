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

#include "Phobos/Error.h"
#include "Phobos/String.h"

namespace Phobos
{
	/**
		This class is used to represent an unix style path name.

		Every path stored by this class does not have a slash at the end, example:

			sys/video
			sys/video/tex.btx
			/sys/video/tex.btx

	*/
	class PH_BASE_API Path
	{
		public:			
			class Iterator
			{
				public:
					inline Iterator(void);
					inline Iterator(const Iterator &other);

					inline ~Iterator(void);				

					inline bool Init(const Path &path, String_t *);
					inline bool InitAtEnd(const Path &path, String_t *);

					inline bool GetNext(String_t *);
					inline bool GetPrev(String_t *);

					inline bool GetCurrent(String_t &out);					

					inline Iterator &operator=(const Iterator &other);

				private:	
					inline void Contruct(void);

				private:
					const Path *m_pclPath;

					unsigned int m_uStart;
					unsigned int m_uEnd;
			};


			Path(void);
			Path(Path &&rhs);
			explicit Path(const Char_t str[]);
			explicit Path(StringRef_t str);	
			explicit Path(String_t &&ref);

			Path &operator=(const Path &rhs);
			Path &operator=(const String_t &rhs);
			Path &operator=(const Char_t str[]);

			void AddName(const String_t &name);
			void Add(const Path &path);		

			bool SetSubPath(Iterator it);

			bool HasExtension(void) const;

			bool GetExtension(String_t &out) const;
			bool GetFileName(String_t &out) const;		
			inline bool GetFileName(Path &out) const;		

			size_t GetNodeCount(void) const;

			bool ExtractPathAndFilename(Path *path, Path *fileName) const;

			void Clear(void);

			inline bool IsRelative(void) const;
			inline bool IsOnlyRoot(void) const;		

			inline const String_t &GetStr(void) const;

			inline Iterator GetBegin(void) const;
			inline Iterator GetEnd(void) const;

			//inline operator const IM_Char_t *() const;
			inline operator const String_t *() const;
			inline operator const String_t &() const;

			bool StripExtension(void);
			void SetExtension(const String_t &newExtension);

			static bool GetExtension(String_t &out, const String_t &path);
			static void ConvertToPlatform(String_t &out, const String_t &in);

		private:
			static bool FindExtensionPos(size_t &out, const String_t &path);

			void FixPath();

			static void FixPathSlash(String_t &path);

		private:
			String_t m_strPath;		

			friend class Path::Iterator;

	};

	inline bool Path::IsOnlyRoot(void) const
	{	
		return(((m_strPath.length() == 1) && (m_strPath[0] == '/') ? true : false));
	}

	inline bool Path::IsRelative(void) const
	{
		return(((!m_strPath.empty()) && (m_strPath[0] == '/')) ? false : true);
	}

	inline const String_t &Path::GetStr(void) const
	{
		return(m_strPath);
	}

	inline Path::Iterator Path::GetBegin(void) const
	{
		Iterator it;

		it.Init(*this, NULL);

		return(it);
	}

	inline Path::Iterator Path::GetEnd(void) const
	{
		Iterator it;

		it.InitAtEnd(*this, NULL);

		return(it);
	}

	/*
	inline Path::operator const IM_Char_t *() const
	{
		return(strPath.c_str());
	}
	*/

	inline Path::operator const String_t *() const
	{
		return(&m_strPath);
	}

	inline Path::operator const String_t &() const
	{
		return(m_strPath);
	}

	// =====================================================
	// ITERATOR
	// =====================================================

	inline void Path::Iterator::Contruct(void)
	{
		m_pclPath = NULL;

		m_uStart = 0;
		m_uEnd = 0;
	}

	inline Path::Iterator::Iterator(void)
	{
		this->Contruct();
	}

	inline Path::Iterator::Iterator(const Iterator &other)
	{
		this->Contruct();

		*this = other;
	}

	inline Path::Iterator::~Iterator(void)
	{

	}	

	inline Path::Iterator &Path::Iterator::operator=(const Iterator &other)
	{
		if(&other == this)
			return(*this);

		m_pclPath = other.m_pclPath;
		m_uStart = other.m_uStart;
		m_uEnd = other.m_uEnd;		

		return(*this);
	}

	inline bool Path::Iterator::GetCurrent(String_t &out)
	{
		if(m_uEnd == m_uStart)
		{
			//if(pclPath->strPath[uEnd] == '\0')
			if(m_pclPath->m_strPath.length() <= m_uEnd)
				return(false);		
		}

		//out.SetSub(&pclPath->strPath, uStart, uEnd - uStart);
		out.assign(m_pclPath->m_strPath, m_uStart, m_uEnd - m_uStart);

		return(true);
	}


	inline bool Path::Iterator::InitAtEnd(const Path &path, String_t *out)
	{		
		m_pclPath = &path;
		m_uStart = m_uEnd = (size_t) m_pclPath->GetStr().length();

		return(this->GetPrev(out));
	}

	inline bool Path::Iterator::Init(const Path &path, String_t *out)
	{		
		m_pclPath = &path;
		m_uStart = m_uEnd = 0;

		return(this->GetNext(out));
	}

	inline bool Path::Iterator::GetNext(String_t *str)
	{
		PH_ASSERT_MSG(m_pclPath != NULL, "Iterator not initialized");

		//we specially allow null strings because of path begin and end iterators build
		//IM_ASSERT(str != NULL);

		Char_t ch;
		const Char_t *cstr = m_pclPath->m_strPath.c_str();

		m_uStart = m_uEnd;
		for(;;)
		{
			ch = cstr[m_uEnd];
			switch(ch)
			{
				case '/':
					if(m_uEnd == m_uStart)
					{
						++m_uEnd;
						++m_uStart;
					}
					else
					{
						if(str)
							//str->SetSub(&pclPath->strPath, uStart, uEnd - uStart);
							str->assign(m_pclPath->m_strPath, m_uStart, m_uEnd-m_uStart);

						return(true);
					}
					break;

				//we reached the end of the path?
				case '\0':
					if(m_uEnd == m_uStart)
						return(false);

					if(str)
						//str->SetSub(&pclPath->strPath, uStart, uEnd - uStart);
						str->assign(m_pclPath->m_strPath, m_uStart, m_uEnd-m_uStart);
					return(true);
					break;

				default:
					++m_uEnd;
					break;
			}
		}
	}

	//FIXME doenst work and is useless
	inline bool Path::Iterator::GetPrev(String_t *out)
	{
		PH_ASSERT_MSG(m_pclPath != NULL, "Please initialize iterator first");

		PH_ASSERT_MSG(0, "NOT TESTED");

		Char_t ch;
			
		m_uEnd = m_uStart;

		if(m_uStart > 0)
			--m_uStart;

		const Char_t *cstr = m_pclPath->m_strPath.c_str();
		for(;;) 
		{
			ch = cstr[m_uStart];

			switch(ch)
			{
				case '/':
					if(m_uEnd == m_uStart)
					{
						if(m_uStart > 0)
						{
							--m_uStart;
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
						if(m_uStart > 0)
							++m_uStart;

						if(out)
							//out->SetSub(&pclPath->strPath, uStart, uEnd - uStart);
							out->assign(m_pclPath->m_strPath, m_uStart, m_uEnd - m_uStart);

						return(true);
					}
					break;

				//are we starting now?
				case '\0':
					//empty path?
					if(m_uStart == 0)
						return(false);
					else
						--m_uStart;
					break;						

				default:
					//we reached start?
					if(m_uStart == 0)
					{
						//ok, return the first node
						if(out != NULL)
							//out->SetSub(&pclPath->strPath, uStart, uEnd - uStart);
							out->assign(m_pclPath->m_strPath, m_uStart, m_uEnd - m_uStart);

						return(true);
					}
					--m_uStart;
					break;
			}
		}
	}


	inline bool Path::GetFileName(Path &out) const
	{
		String_t str;

		if(this->GetFileName(str))
		{
			out = str;

			return true;
		}

		return false;		
	}

	inline std::ostream& operator<< (std::ostream &out, const Path &path)
	{
		out << path.GetStr();

		return out;
	}
}

#endif
