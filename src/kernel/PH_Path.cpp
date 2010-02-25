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

#include "PH_Path.h"

#include "PH_Exception.h"

namespace Phobos
{
	Path_c::Path_c(void)
	{

	}

	Path_c::Path_c(const String_c &str):
		strPath(str)
	{
		this->FixPath();
	}

	Path_c::Path_c(const Char_t str[]):
		strPath(str)

	{
		this->FixPath();
	}

	void Path_c::FixPath()
	{
		Path_c::FixPathSlash(strPath);	

		//If the user suplied only one char, it can be just a bar '/'
		size_t sz = strPath.length();
		if(sz <= 1)
		{
			//root path, ok just a one letter path, its fine
			return;
		}	
		
		//Here we have a "big path", lets check it
		if(strPath[sz-1] == '/')	
			strPath.erase(sz-1);	
	}

	Path_c &Path_c::operator=(const Path_c &rhs)
	{
		if(&rhs == this)
			return *this;

		strPath = rhs.strPath;
		return *this;
	}

	Path_c &Path_c::operator=(const String_c &rhs)
	{
		strPath = rhs;
				
		this->FixPath();

		return *this;
	}	

	Path_c &Path_c::operator=(const Char_t *str)
	{
		strPath = str;
		this->FixPath();

		return *this;
	}

	/**

		This methods concatenates the sub-path on name with the current path.

		The path is added to the end of this path. '/' are added if necessary.

		\param name the path-name to be added
	*/
	void Path_c::AddName(const String_c &n)
	{	
		size_t len = n.length();
		if(len == 0)
			return;

		if((len == 1) && (n[0] == '/'))
			return;

		String_c name(n);

		Path_c::FixPathSlash(name);

		if(name[0] == '/')
		{
			//If this is only a root path
			if(this->IsOnlyRoot())
				//strPath.AddSub(&name, 1, name.GetLength() - 1);
				strPath.append(name.c_str()+1, name.length()-1);
			else
				//strPath.Add(&name);
				strPath.append(name);
		}
		else
		{
			//If this is only a root path
			if(this->IsOnlyRoot())
			{
				//strPath.Add(&name);
				strPath.append(name);
			}
			else
			{
				//is it an relative path ? Just add an bar to the end
				if(!strPath.empty())
					//strPath.Add(IM_CHAR('/'));
					strPath.append("/");

				//strPath.Add(&name);
				strPath.append(name);
			}
		}

		//if(strPath.GetAt(IM_UInt_t(strPath.GetLength() - 1)) == IM_CHAR('/'))
		//	strPath.RemoveBack(NULL);
		len = strPath.length()-1;
		if(strPath[len] == '/')
			strPath.erase(len);	
	}

	void Path_c::Add(const Path_c &path)
	{			
		if(path.GetStr().empty())
			return;

		if(path.strPath[0] == '/')
		{
			std::stringstream stream;
			stream << "Path " << path.strPath << " must be relative for concanetating to " << strPath;
			PH_RAISE(ERROR_INVALID_PARAMETER, "Path_c::Add", stream.str());
		}
			
		if(!strPath.empty() && !this->IsOnlyRoot())
			strPath.append("/");

		strPath.append(path.strPath);
	}


	void Path_c::Clear()
	{
		strPath.clear();
	}

	bool Path_c::SetSubPath(Path_c::Iterator_c it)
	{		
		String_c tmp;

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

	bool Path_c::HasExtension(void) const
	{
		size_t pos;

		return(this->FindExtensionPos(pos));
	}

	bool Path_c::GetExtension(String_c &out) const
	{
		size_t pos;

		if(this->FindExtensionPos(pos))		
		{		
			size_t sz = strPath.length();

			out.assign(strPath, pos+1, sz - (pos + 1));
			
			return(true);
		}

		return(false);
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
	bool Path_c::GetFileName(String_c &out) const
	{		
		size_t sz = strPath.length();

		if(sz == 0)
			return(false);

		//Now, just go back the path until we found a '/' or the start of the path
		size_t pos = sz -1;	
		for(;;)
		{
			Char_t ch = strPath[pos];

			if(ch == '/')
			{
				if(pos == sz - 1)
					return(false);

				//out.assign(strPath.c_str(), pos+1);
				//out.Set(strPath.GetStr() + pos + 1);
				out = strPath.c_str() + pos+1;

				return(true);
			}
			else if(pos == 0)
			{								
				out = strPath.c_str() + pos;

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
	size_t Path_c::GetNodeCount(void) const
	{
		size_t sz = strPath.length();
		size_t pos = 0;

		size_t count = 0;
		bool got = false;
		Char_t ch;

		if(0 == sz)
			return(0);

		for(;pos < sz; ++pos)
		{
			ch = strPath[pos];

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

	bool Path_c::ExtractPathAndFilename(Path_c *path, Path_c *fileName) const
	{		
		PH_ASSERT_VALID(path || fileName);	

		if(NULL == path)
		{
			return(this->GetFileName(*fileName));
		}
		else
		{
			Path_c::Iterator_c it(this->GetBegin());

			String_c			currentName;
			String_c			nextName;			

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

	bool Path_c::FindExtensionPos(size_t &out) const
	{
		size_t len =	strPath.length();

		//No extension (empty path)?
		if(len < 1)
			return(false);

		for(size_t i = len; i > 0; --i)
		{
			Char_t ch = strPath[i];
			if(ch == '.')
			{
				out = i;

				return(true);
			}
			else if((ch == '/' || (ch == '\\')))
				return(false);
		}

		out = 0;
		return strPath[0] == '.' ? true : false;		
	}

	bool Path_c::StripExtension(void)
	{
		size_t pos;
		if(!this->FindExtensionPos(pos))
			return false;		

		//extension found, now remove it
		//strPath.RemoveAt(pos, strPath.GetLength() - pos);
		strPath.erase(pos, strPath.length() - pos);

		return(true);
	}

	void Path_c::SetExtension(const String_c &newExtension)
	{
		this->StripExtension();	

		if(newExtension[0] != '.')
			strPath.append(".");
		
		strPath.append(newExtension);
	}

	void Path_c::FixPathSlash(String_c &path)
	{		
		StringReplaceAll(path, '\\', '/');
	}

	// =====================================================
	// ITERATOR
	// =====================================================
}
