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
	${OBJECTDIR}/_ext/1103679714/main.o \
	${OBJECTDIR}/_ext/1103679714/DictionaryTest.o


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
LDLIBSOPTIONS=-L../../../bin -lPH_Kerneld -lPH_Dictionaryd -lboost_system -lboost_filesystem -lboost_unit_test_framework

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk dist/Debug/GNU-Linux-x86/dictionarytest

dist/Debug/GNU-Linux-x86/dictionarytest: ${OBJECTFILES}
	${MKDIR} -p dist/Debug/GNU-Linux-x86
	${LINK.cc} -o ${CND_DISTDIR}/${CND_CONF}/${CND_PLATFORM}/dictionarytest ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/1103679714/main.o: ../../../src/dictionaryTest/main.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1103679714
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_GCC -DPH_SDL -DPH_LINUX -DPH_DEBUG -DPH_CHECK_ASSERT -DBOOST_TEST_DYN_LINK -I../../../include/dictionary -I../../../include/kernel -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1103679714/main.o ../../../src/dictionaryTest/main.cpp

${OBJECTDIR}/_ext/1103679714/DictionaryTest.o: ../../../src/dictionaryTest/DictionaryTest.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/1103679714
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_GCC -DPH_SDL -DPH_LINUX -DPH_DEBUG -DPH_CHECK_ASSERT -DBOOST_TEST_DYN_LINK -I../../../include/dictionary -I../../../include/kernel -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/1103679714/DictionaryTest.o ../../../src/dictionaryTest/DictionaryTest.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} dist/Debug/GNU-Linux-x86/dictionarytest

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
