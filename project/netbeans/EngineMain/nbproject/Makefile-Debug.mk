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
	${OBJECTDIR}/_ext/1409335989/PH_EngineMain.o


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
LDLIBSOPTIONS=-L../../../bin -lPH_GameEngined -lBulletCollision -lPH_Kerneld -lPH_Inputd -lPH_Dictionaryd -lPH_EgineCored -lPH_GameEngined -lOgreMain -lOgreTerrain

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/enginemain

dist/Debug/GNU-Linux-x86/enginemain: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/enginemain ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1409335989/PH_EngineMain.o: ../../../src/engineMain/PH_EngineMain.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1409335989
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_DEBUG -DPH_CHECK_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/gameEngine -I../../../include/engineCore -I../../../include/dictionary -I/usr/include/OGRE -I/usr/include/bullet -I/usr/include/OGRE/Terrain -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1409335989/PH_EngineMain.o ../../../src/engineMain/PH_EngineMain.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/enginemain

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
