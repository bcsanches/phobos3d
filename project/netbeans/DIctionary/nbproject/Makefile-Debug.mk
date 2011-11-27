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
	${OBJECTDIR}/_ext/416591920/PH_DictionaryUtils.o \
	${OBJECTDIR}/_ext/416591920/PH_Dictionary.o \
	${OBJECTDIR}/_ext/416591920/PH_DictionaryManager.o \
	${OBJECTDIR}/_ext/416591920/PH_DictionaryHive.o


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
LDLIBSOPTIONS=-L../../../bin -lboost_filesystem -lboost_system -lPH_Kerneld

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-Debug.mk ../../../bin/libPH_Dictionaryd.so

../../../bin/libPH_Dictionaryd.so: ${OBJECTFILES}
	${MKDIR} -p ../../../bin
	${LINK.cc} -shared -o ../../../bin/libPH_Dictionaryd.so -fPIC ${OBJECTFILES} ${LDLIBSOPTIONS} 

${OBJECTDIR}/_ext/416591920/PH_DictionaryUtils.o: ../../../src/dictionary/PH_DictionaryUtils.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/416591920
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_CHECK_ASSERT -DPH_DEBUG -I../../../include/dictionary -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/416591920/PH_DictionaryUtils.o ../../../src/dictionary/PH_DictionaryUtils.cpp

${OBJECTDIR}/_ext/416591920/PH_Dictionary.o: ../../../src/dictionary/PH_Dictionary.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/416591920
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_CHECK_ASSERT -DPH_DEBUG -I../../../include/dictionary -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/416591920/PH_Dictionary.o ../../../src/dictionary/PH_Dictionary.cpp

${OBJECTDIR}/_ext/416591920/PH_DictionaryManager.o: ../../../src/dictionary/PH_DictionaryManager.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/416591920
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_CHECK_ASSERT -DPH_DEBUG -I../../../include/dictionary -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/416591920/PH_DictionaryManager.o ../../../src/dictionary/PH_DictionaryManager.cpp

${OBJECTDIR}/_ext/416591920/PH_DictionaryHive.o: ../../../src/dictionary/PH_DictionaryHive.cpp 
	${MKDIR} -p ${OBJECTDIR}/_ext/416591920
	${RM} $@.d
	$(COMPILE.cc) -g -DPH_LINUX -DPH_GCC -DPH_SDL -DPH_CHECK_ASSERT -DPH_DEBUG -I../../../include/dictionary -I../../../include/kernel -fPIC  -MMD -MP -MF $@.d -o ${OBJECTDIR}/_ext/416591920/PH_DictionaryHive.o ../../../src/dictionary/PH_DictionaryHive.cpp

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/Debug
	${RM} ../../../bin/libPH_Dictionaryd.so

# Subprojects
.clean-subprojects:

# Enable dependency checking
.dep.inc: .depcheck-impl

include .dep.inc
