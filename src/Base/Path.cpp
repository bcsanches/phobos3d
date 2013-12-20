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

#include "Phobos/Path.h"

#include "Phobos/Exception.h"

namespace Phobos
{
	Path::Path(void)
	{

	}

	Path::Path(Path &&rhs):
		m_strPath(std::move(rhs.m_strPath))
	{
		//empty
	}

	Path::Path(StringRef_t str):
		m_strPath(str.data())
	{
		this->FixPath();
	}

	Path::Path(String_t &&ref):
		m_strPath(std::move(ref))
	{
		this->FixPath();
	}

	Path::Path(const Char_t str[]):
		m_strPath(str)

	{
		this->FixPath();
	}

	void Path::FixPath()
	{
		Path::FixPathSlash(m_strPath);

		//If the user suplied only one char, it can be just a bar '/'
		size_t sz = m_strPath.length();
		if(sz <= 1)
		{
			//root path, ok just a one letter path, its fine
			return;
		}

		//Here we have a "big path", lets check it
		if(m_strPath[sz-1] == '/')
			m_strPath.erase(sz-1);
	}

	Path &Path::operator=(const Path &rhs)
	{
		if(&rhs == this)
			return *this;

		m_strPath = rhs.m_strPath;
		return *this;
	}

	Path &Path::operator=(const String_t &rhs)
	{
		m_strPath = rhs;

		this->FixPath();

		return *this;
	}

	Path &Path::operator=(const Char_t *str)
	{
		m_strPath = str;
		this->FixPath();

		return *this;
	}

	/**

		This methods concatenates the sub-path on name with the current path.

		The path is added to the end of this path. '/' are added if necessary.

		\param name the path-name to be added
	*/
	void Path::AddName(const String_t &n)
	{
		size_t len = n.length();
		if(len == 0)
			return;

		if((len == 1) && (n[0] == '/'))
			return;

		String_t name(n);

		Path::FixPathSlash(name);

		if(name[0] == '/')
		{
			//If this is only a root path
			if(this->IsOnlyRoot())
				//m_strPath.AddSub(&name, 1, name.GetLength() - 1);
				m_strPath.append(name.c_str()+1, name.length()-1);
			else
				//m_strPath.Add(&name);
				m_strPath.append(name);
		}
		else
		{
			//If this is only a root path
			if(this->IsOnlyRoot())
			{
				//m_strPath.Add(&name);
				m_strPath.append(name);
			}
			else
			{
				//is it an relative path ? Just add an bar to the end
				if(!m_strPath.empty())
					//m_strPath.Add(IM_CHAR('/'));
					m_strPath.append("/");

				//m_strPath.Add(&name);
				m_strPath.append(name);
			}
		}

		//if(m_strPath.GetAt(IM_UInt_t(m_strPath.GetLength() - 1)) == IM_CHAR('/'))
		//	m_strPath.RemoveBack(NULL);
		len = m_strPath.length()-1;
		if(m_strPath[len] == '/')
			m_strPath.erase(len);
	}

	void Path::Add(const Path &path)
	{
		if(path.GetStr().empty())
			return;

		if(path.m_strPath[0] == '/')
		{
			std::stringstream stream;
			stream << "Path " << path.m_strPath << " must be relative for concanetating to " << m_strPath;
			PH_RAISE(INVALID_PARAMETER_EXCEPTION, "Path::Add", stream.str());
		}

		if(!m_strPath.empty() && !this->IsOnlyRoot())
			m_strPath.append("/");

		m_strPath.append(path.m_strPath);
	}


	void Path::Clear()
	{
		m_strPath.clear();
	}

	bool Path::SetSubPath(Path::Iterator it)
	{
		String_t tmp;

		this->Clear();

		if(it.GetCurrent(tmp))
		{
			this->AddName(tmp);

			while(it.GetNext(&tmp))
			{
				this->AddName(tmp);
			}

			return(true);
		}
		else
			return(false);
	}

	bool Path::HasExtension(void) const
	{
		size_t pos;

		return(this->FindExtensionPos(pos, m_strPath));
	}

	bool Path::GetExtension(String_t &out, const String_t &path)
	{
		size_t pos;

		if(Path::FindExtensionPos(pos, path))
		{
			size_t sz = path.length();

			out.assign(path, pos+1, sz - (pos + 1));

			return(true);
		}

		return(false);
	}

	bool Path::GetExtension(String_t &out) const
	{
		return Path::GetExtension(out, m_strPath);
	}

	/**

		This method copies the last "node" of the pathname, for example:
			/sys/dev/video

		It will return "video"

		And:
			/sys/dev/rmanager/tex.btx

			it will return "tex.btx"

		And:
			tex.btx

			it will return "tex.btx"

		And:

			screen

			it will return "screen"

		and:

			/sys/video/

			it will return ERROR, no filename

		and
			/

			it will return ERROR, no filename



	*/
	bool Path::GetFileName(String_t &out) const
	{
		size_t sz = m_strPath.length();

		if(sz == 0)
			return(false);

		//Now, just go back the path until we found a '/' or the start of the path
		size_t pos = sz -1;
		for(;;)
		{
			Char_t ch = m_strPath[pos];

			if(ch == '/')
			{
				if(pos == sz - 1)
					return(false);

				//out.assign(m_strPath.c_str(), pos+1);
				//out.Set(m_strPath.GetStr() + pos + 1);
				out = m_strPath.c_str() + pos+1;

				return(true);
			}
			else if(pos == 0)
			{
				out = m_strPath.c_str() + pos;

				return(true);
			}

			--pos;
		}
	}

	/**

		It counts the "depth" of the pathname, for example:

			"/sys/video/" -> returns 2

			"/sys/" -> returns 1

			"/" -> returns 0

			"" -> returns 0

			"sys/video" -> returns 2

			"/sys/video" -> returns 2

			"video" -> return 1

			"/video" -> return 1

			"video/" -> returns 1

			"/video/" -> returns 1

	*/
	size_t Path::GetNodeCount(void) const
	{
		size_t sz = m_strPath.length();
		size_t pos = 0;

		size_t count = 0;
		bool got = false;
		Char_t ch;

		if(0 == sz)
			return(0);

		for(;pos < sz; ++pos)
		{
			ch = m_strPath[pos];

			if(ch == '/')
			{
				got = false;
				continue;
			}

			if(false == got)
			{
				got = true;

				++count;
			}
		}

		return(count);
	}

	bool Path::ExtractPathAndFilename(Path *path, Path *fileName) const
	{
		PH_ASSERT_VALID(path || fileName);

		if(NULL == path)
		{
			return(this->GetFileName(*fileName));
		}
		else
		{
			Path::Iterator it(this->GetBegin());

			String_t			currentName;
			String_t			nextName;

			path->Clear();

			if(NULL != fileName)
				fileName->Clear();

			if(!it.GetCurrent(currentName))
				return false;

			if(!this->IsRelative())
				*path = "/";

			for(;;)
			{
				if(!it.GetNext(&nextName))
				{
					if(fileName != NULL)
						*fileName = currentName;

					return(true);
				}
				else
				{
					path->AddName(currentName);

					currentName = nextName;
				}
			}
		}
	}

	bool Path::FindExtensionPos(size_t &out, const String_t &path)
	{
		size_t len =	path.length();

		//No extension (empty path)?
		if(len < 1)
			return(false);

		for(size_t i = len; i > 0; --i)
		{
			Char_t ch = path[i];
			if(ch == '.')
			{
				out = i;

				return(true);
			}
			else if((ch == '/' || (ch == '\\')))
				return(false);
		}

		out = 0;
		return path[0] == '.' ? true : false;
	}

	bool Path::StripExtension(void)
	{
		size_t pos;
		if(!this->FindExtensionPos(pos, m_strPath))
			return false;

		//extension found, now remove it
		//m_strPath.RemoveAt(pos, m_strPath.GetLength() - pos);
		m_strPath.erase(pos, m_strPath.length() - pos);

		return(true);
	}

	void Path::SetExtension(const String_t &newExtension)
	{
		this->StripExtension();

		if(newExtension[0] != '.')
			m_strPath.append(".");

		m_strPath.append(newExtension);
	}

	void Path::FixPathSlash(String_t &path)
	{
		StringReplaceAll(path, '\\', '/');
	}

	void Path::ConvertToPlatform(String_t &out, const String_t &in)
	{
#ifdef PH_WIN32
		out = in;
		StringReplaceAll(out, '/', '\\');
#else

    #ifdef PH_LINUX
        //linux does not need slash changes
        out = in;

    #else
        #error "Platform not supported"
    #endif

#endif
	}

	// =====================================================
	// ITERATOR
	// =====================================================
}
