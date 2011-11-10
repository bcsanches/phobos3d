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
CND_CONF=Release
CND_DISTDIR=dist

# Include project Makefile
include Makefile

# Object Directory
OBJECTDIR=build/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/_ext/258580531/CoreTest.o \
	${OBJECTDIR}/_ext/258580531/AccelerometerTest.o \
	${OBJECTDIR}/_ext/258580531/main.o


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
LDLIBSOPTIONS=-L../../../bin -Wl,-rpath /home/cronosts/Programming/Projects/phobos3d/work/engineMain -lboost_unit_test_framework -lboost_filesystem -lboost_system -lPH_Kernel -lPH_Input -lPH_EgineCore -lPH_Dictionary

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Release.mk dist/Release/GNU-Linux-x86/enginecoretest

dist/Release/GNU-Linux-x86/enginecoretest: ${OBJECTFILES}
	${MKDIR} -p dist/Release/GNU-Linux-x86
	g++ -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/enginecoretest ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/258580531/CoreTest.o: ../../../src/engineCoreTest/CoreTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/258580531
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I/usr/include/OGRE -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/258580531/CoreTest.o ../../../src/engineCoreTest/CoreTest.cpp

${OBJECTDIR}/_ext/258580531/AccelerometerTest.o: ../../../src/engineCoreTest/AccelerometerTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/258580531
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I/usr/include/OGRE -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/258580531/AccelerometerTest.o ../../../src/engineCoreTest/AccelerometerTest.cpp

${OBJECTDIR}/_ext/258580531/main.o: ../../../src/engineCoreTest/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/258580531
	${RM} $@.d
	$(COMPILE.cc) -O2 -DPH_GCC -DPH_LINUX -DPH_SDL -DBOOST_TEST_DYN_LINK -DPH_RELEASE -DPH_IGNORE_ASSERT -I../../../include/kernel -I../../../include/input -I../../../include/dictionary -I../../../include/engineCore -I/usr/include/OGRE -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/258580531/main.o ../../../src/engineCoreTest/main.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Release
	${RM} dist/Release/GNU-Linux-x86/enginecoretest

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
