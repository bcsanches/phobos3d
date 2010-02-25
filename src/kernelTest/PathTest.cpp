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

#include <boost/test/unit_test.hpp>

#include <PH_Exception.h>
#include <PH_Path.h>

using namespace Phobos;

BOOST_AUTO_TEST_CASE(path_basic)
{	
	BOOST_REQUIRE(Path_c("test").GetStr().compare("test") == 0);
	BOOST_REQUIRE(Path_c("test/").GetStr().compare("test") == 0);
	BOOST_REQUIRE(Path_c("/test").GetStr().compare("/test") == 0);
	BOOST_REQUIRE(Path_c("/test/").GetStr().compare("/test") == 0);

	BOOST_REQUIRE(Path_c("/").GetStr().compare("/") == 0);
	BOOST_REQUIRE(Path_c("a").GetStr().compare("a") == 0);

	Path_c a("/root"), b("folder");

	a = b;
	BOOST_REQUIRE(a.GetStr().compare("folder") == 0);

	a = "abc/";
	BOOST_REQUIRE(a.GetStr().compare("abc") == 0);

	a.Clear();
	BOOST_REQUIRE(a.GetStr().compare("") == 0);

	BOOST_REQUIRE(!Path_c().IsOnlyRoot());
	BOOST_REQUIRE(Path_c("/").IsOnlyRoot());
	BOOST_REQUIRE(!Path_c("/abc").IsOnlyRoot());

	BOOST_REQUIRE(Path_c("").IsRelative());
	BOOST_REQUIRE(Path_c("abc").IsRelative());
	BOOST_REQUIRE(Path_c("abc/bla").IsRelative());
	BOOST_REQUIRE(!Path_c("/").IsRelative());
	BOOST_REQUIRE(!Path_c("/abc/a").IsRelative());
}

BOOST_AUTO_TEST_CASE(path_AddName)
{
	Path_c a("/");

	a.AddName("system");	
	BOOST_REQUIRE(a.GetStr().compare("/system") == 0);

	a.AddName("sub");
	BOOST_REQUIRE(a.GetStr().compare("/system/sub") == 0);

	a.AddName("/root/");
	BOOST_REQUIRE(a.GetStr().compare("/system/sub/root") == 0);

	a = "/";
	a.AddName("/root/");
	BOOST_REQUIRE(a.GetStr().compare("/root") == 0);

	a.AddName("");
	BOOST_REQUIRE(a.GetStr().compare("/root") == 0);

	a.AddName("/");
	BOOST_REQUIRE(a.GetStr().compare("/root") == 0);

	a = "/";
	a.AddName("root");
	BOOST_REQUIRE(a.GetStr().compare("/root") == 0);

	a = "";
	a.AddName("root");
	BOOST_REQUIRE(a.GetStr().compare("root") == 0);
}

BOOST_AUTO_TEST_CASE(path_Add)
{
	Path_c a("/");

	a.Add(Path_c("system"));	
	BOOST_REQUIRE(a.GetStr().compare("/system") == 0);

	a.Add(Path_c("sub"));
	BOOST_REQUIRE(a.GetStr().compare("/system/sub") == 0);

	BOOST_REQUIRE_THROW(a.Add(Path_c("/root/")), InvalidParameterException_c);	

	a = "/";	

	a.Add(Path_c(""));
	BOOST_REQUIRE(a.GetStr().compare("/") == 0);
	
	BOOST_REQUIRE_THROW(a.Add(Path_c("/")), InvalidParameterException_c);

	a = "/";
	a.Add(Path_c("root"));
	BOOST_REQUIRE(a.GetStr().compare("/root") == 0);

	a = "";
	a.Add(Path_c("root"));
	BOOST_REQUIRE(a.GetStr().compare("root") == 0);
}

BOOST_AUTO_TEST_CASE(path_SetSubPath)
{
	Path_c a("root/system/bla");
	Path_c b;

	Path_c::Iterator_c it(a.GetBegin());
	
	it.GetNext(NULL);	

	BOOST_REQUIRE(b.SetSubPath(it));
	BOOST_REQUIRE(b.GetStr().compare("system/bla") == 0);

	it.GetNext(NULL);
	it.GetNext(NULL);

	BOOST_REQUIRE(!b.SetSubPath(it));
	BOOST_REQUIRE(b.GetStr().empty());
}

BOOST_AUTO_TEST_CASE(path_HasExtension)
{
	Path_c a("file");

	BOOST_CHECK(!a.HasExtension());

	a = "file.abc";
	BOOST_CHECK(a.HasExtension());

	a = "system.abc/file";
	BOOST_CHECK(!a.HasExtension());	

	a = ".";
	BOOST_CHECK(a.HasExtension());

	a = "/";
	BOOST_CHECK(!a.HasExtension());

	a = "a";
	BOOST_CHECK(!a.HasExtension());
}

BOOST_AUTO_TEST_CASE(path_GetExtension)
{
	Path_c a("file");
	String_c ext;

	BOOST_CHECK(!a.GetExtension(ext));

	a = "file.abc";
	BOOST_REQUIRE(a.GetExtension(ext));
	BOOST_CHECK(ext.compare("abc") == 0);

	a = "system.abc/file";
	BOOST_CHECK(!a.GetExtension(ext));

	a = ".";
	BOOST_CHECK(a.GetExtension(ext));

	a = "/";
	BOOST_CHECK(!a.GetExtension(ext));

	a = "a";
	BOOST_CHECK(!a.GetExtension(ext));
}

BOOST_AUTO_TEST_CASE(path_GetFileName)
{
	Path_c a("file");
	String_c file;

	BOOST_REQUIRE(a.GetFileName(file));
	BOOST_CHECK(file.compare("file") == 0);

	a = "file.abc";
	BOOST_REQUIRE(a.GetFileName(file));
	BOOST_CHECK(file.compare("file.abc") == 0);

	a = "system.abc/file";
	BOOST_REQUIRE(a.GetFileName(file));
	BOOST_CHECK(file.compare("file") == 0);

	a = ".";
	BOOST_REQUIRE(a.GetFileName(file));
	BOOST_CHECK(file.compare(".") == 0);

	a = "/";
	BOOST_CHECK(!a.GetFileName(file));

	a = "a";
	BOOST_REQUIRE(a.GetFileName(file));
	BOOST_CHECK(file.compare("a") == 0);

	a = "/a";
	BOOST_REQUIRE(a.GetFileName(file));
	BOOST_CHECK(file.compare("a") == 0);

	a = "";
	BOOST_REQUIRE(!a.GetFileName(file));	
}

BOOST_AUTO_TEST_CASE(path_GetNodeCount)
{
	BOOST_CHECK(Path_c("").GetNodeCount() == 0);
	BOOST_CHECK(Path_c("/sys/video/").GetNodeCount() == 2);
	BOOST_CHECK(Path_c("/sys/").GetNodeCount() == 1);
	BOOST_CHECK(Path_c("/").GetNodeCount() == 0);
	BOOST_CHECK(Path_c("sys/video").GetNodeCount() == 2);
	BOOST_CHECK(Path_c("/sys/video").GetNodeCount() == 2);
	BOOST_CHECK(Path_c("video").GetNodeCount() == 1);
	BOOST_CHECK(Path_c("/video").GetNodeCount() == 1);
	BOOST_CHECK(Path_c("/video/tex.bmp").GetNodeCount() == 2);
	BOOST_CHECK(Path_c(".").GetNodeCount() == 1);
	BOOST_CHECK(Path_c("/.").GetNodeCount() == 1);
}

BOOST_AUTO_TEST_CASE(path_ExtractPathAndFilename)
{	
	Path_c file, path;
	BOOST_CHECK(!Path_c().ExtractPathAndFilename(&path, &file));
	BOOST_CHECK(!Path_c("/").ExtractPathAndFilename(&path, &file));

	BOOST_REQUIRE(Path_c("a").ExtractPathAndFilename(&path, &file));
	BOOST_CHECK(path.GetStr().empty());
	BOOST_CHECK(file.GetStr().compare("a") == 0);

	BOOST_REQUIRE(Path_c("sys/text.txt").ExtractPathAndFilename(&path, &file));
	BOOST_CHECK(path.GetStr().compare("sys")==0);
	BOOST_CHECK(file.GetStr().compare("text.txt") == 0);

	BOOST_REQUIRE(Path_c("/sys/text.txt").ExtractPathAndFilename(&path, &file));
	BOOST_CHECK(path.GetStr().compare("/sys")==0);
	BOOST_CHECK(file.GetStr().compare("text.txt") == 0);

	BOOST_REQUIRE(Path_c("/sys/files/bla/text.txt").ExtractPathAndFilename(&path, &file));
	BOOST_CHECK(path.GetStr().compare("/sys/files/bla")==0);
	BOOST_CHECK(file.GetStr().compare("text.txt") == 0);

	BOOST_REQUIRE(Path_c("/text.txt").ExtractPathAndFilename(&path, &file));
	BOOST_CHECK(path.GetStr().compare("/")==0);
	BOOST_CHECK(file.GetStr().compare("text.txt") == 0);
}

BOOST_AUTO_TEST_CASE(path_StripExtension)
{
	BOOST_CHECK(!Path_c().StripExtension());
	BOOST_CHECK(!Path_c("/").StripExtension());
	BOOST_CHECK(!Path_c("/file").StripExtension());
	BOOST_CHECK(!Path_c("file").StripExtension());

	Path_c a("file.abc");
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("file") == 0);

	a = "file.abc.xyz";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("file.abc") == 0);

	a = "/file.abc.xyz";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("/file.abc") == 0);

	a = "system/bla/file.abc.xyz";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("system/bla/file.abc") == 0);

	a = "/system/bla/file.abc.xyz";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("/system/bla/file.abc") == 0);

	a = ".xyz";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().empty());

	a = "/.xyz";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("/") == 0);

	a = ".";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().empty());

	a = "/.";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("/") == 0);

	a = "file.";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("file") == 0);

	a = "file.abc.";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("file.abc") == 0);

	a = "/file.abc.";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("/file.abc") == 0);

	a = "system/bla/file.abc.";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("system/bla/file.abc") == 0);

	a = "/system/bla/file.abc.";
	BOOST_REQUIRE(a.StripExtension());
	BOOST_CHECK(a.GetStr().compare("/system/bla/file.abc") == 0);
}

BOOST_AUTO_TEST_CASE(path_SetExtension)
{
	Path_c a;

	a.SetExtension("abc");
	BOOST_CHECK(a.GetStr().compare(".abc") == 0);

	a.SetExtension(".abc");
	BOOST_CHECK(a.GetStr().compare(".abc") == 0);

	a = "file.";
	a.SetExtension(".abc");
	BOOST_CHECK(a.GetStr().compare("file.abc") == 0);

	a.SetExtension("xyz");
	BOOST_CHECK(a.GetStr().compare("file.xyz") == 0);

	a = "/";
	a.SetExtension("xyz");
	BOOST_CHECK(a.GetStr().compare("/.xyz") == 0);
}
