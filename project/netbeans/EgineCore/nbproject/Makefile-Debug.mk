#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=gcc
CCC=g++
CXX=g++
FC=
AS=as

# Macros
CND_PLATFORM=GNU-Linux-x86
CND_CONF=Debug
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/1409051803/PH_Plugin.o \
	${OBJECTDIR}/_ext/1409051803/PH_Console.o \
	${OBJECTDIR}/_ext/1409051803/PH_CoreModuleManager.o \
	${OBJECTDIR}/_ext/1409051803/PH_Render.o \
	${OBJECTDIR}/_ext/1409051803/PH_CoreModule.o \
	${OBJECTDIR}/_ext/1409051803/PH_OgreUtils.o \
	${OBJECTDIR}/_ext/1409051803/PH_BootModule.o \
	${OBJECTDIR}/_ext/1409051803/PH_PluginManager.o \
	${OBJECTDIR}/_ext/1409051803/PH_Core.o \
	${OBJECTDIR}/_ext/1409051803/PH_EventManagerModule.o


# C Compiler Flags
CFLAGS=

# CC Compiler Flags
CCFLAGS=
CXXFLAGS=

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-L../../../bin -lPH_Kerneld -lPH_Inputd -lPH_Dictionaryd -lOgreMain -lOgreTerrain

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk ../../../bin/libPH_EgineCored.so

../../../bin/libPH_EgineCored.so: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -shared -o ../../../bin/libPH_EgineCored.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1409051803/PH_Plugin.o: ../../../src/engineCore/PH_Plugin.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_Plugin.o ../../../src/engineCore/PH_Plugin.cpp

${OBJECTDIR}/_ext/1409051803/PH_Console.o: ../../../src/engineCore/PH_Console.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_Console.o ../../../src/engineCore/PH_Console.cpp

${OBJECTDIR}/_ext/1409051803/PH_CoreModuleManager.o: ../../../src/engineCore/PH_CoreModuleManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_CoreModuleManager.o ../../../src/engineCore/PH_CoreModuleManager.cpp

${OBJECTDIR}/_ext/1409051803/PH_Render.o: ../../../src/engineCore/PH_Render.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_Render.o ../../../src/engineCore/PH_Render.cpp

${OBJECTDIR}/_ext/1409051803/PH_CoreModule.o: ../../../src/engineCore/PH_CoreModule.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_CoreModule.o ../../../src/engineCore/PH_CoreModule.cpp

${OBJECTDIR}/_ext/1409051803/PH_OgreUtils.o: ../../../src/engineCore/PH_OgreUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_OgreUtils.o ../../../src/engineCore/PH_OgreUtils.cpp

${OBJECTDIR}/_ext/1409051803/PH_BootModule.o: ../../../src/engineCore/PH_BootModule.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_BootModule.o ../../../src/engineCore/PH_BootModule.cpp

${OBJECTDIR}/_ext/1409051803/PH_PluginManager.o: ../../../src/engineCore/PH_PluginManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_PluginManager.o ../../../src/engineCore/PH_PluginManager.cpp

${OBJECTDIR}/_ext/1409051803/PH_Core.o: ../../../src/engineCore/PH_Core.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_Core.o ../../../src/engineCore/PH_Core.cpp

${OBJECTDIR}/_ext/1409051803/PH_EventManagerModule.o: ../../../src/engineCore/PH_EventManagerModule.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409051803
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/engineCore -I../../../include/kernel -I/usr/include/OGRE -I../../../include/input -I../../../include/dictionary -I/usr/include/OGRE/Terrain -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409051803/PH_EventManagerModule.o ../../../src/engineCore/PH_EventManagerModule.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} ../../../bin/libPH_EgineCored.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
