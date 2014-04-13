Phobos 3D
=========

Phobos 3D is a 3D game engine created for testing game development concepts and for fun. 
Free for commercial use, open source under the ZLib License.

https://github.com/bcsanches/phobos3d

Building (Windows instructions)
--------

For building Phobos several libraries are required:

* Ogre3D (http://www.ogre3d.org/)
* Boost Libraries (http://www.boost.org/)
* SDL 1.2 (http://www.libsdl.org/)
* libRocket (http://librocket.com/)
* Bullet Physics (http://bulletphysics.org/wordpress/)
* rapidxml (http://rapidxml.sourceforge.net/) - soon we expect to drop this one
* rapidjson (https://code.google.com/p/rapidjson/)
* JsonCreator (https://github.com/bcsanches/JsonCreator)

But to make life easir, we created Phobos Dependencies project that include all libraries above, except for Boost and Ogre.

So for building we assume that you already have Ogre3d and Boost libraries on your system.

1. Clone and build Phobos Dependencies package, located at: https://github.com/bcsanches/phobos_deps
2. After building, create an environment variable called PHOBOS_DEPS_HOME, that points to the Phobos Dependencies package root directory.
3. Create an enviroment variable named OGRE_HOME that points to Ogre3d home directory.
4. Create an environement variable named BOOST_HOME that points to Boost Libraries home directory
5. Open Visual Studio 2013 and load Phobos 3d solution.
6. If all environment variables are defined ok, just hit "Build Solution" 

